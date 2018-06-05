#include "dev.h"
#include "usb_device.h"

#include <stdio.h>
#include <time.h>
//------------------------------------------------------------------------------
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
//------------------------------------------------------------------------------
void DEVICE_Init(Struct_Device *dev)
{
	//Проинициализировали RCC контроллера
	RCC_InitDev(dev->rcc);

	//Инициализация UART для отладки
	UART_InitDev(dev->uart_debug);
	DEV_PRINT_SetPtrDebugUart(dev->uart_debug);

	//Инициализация USB
	MX_USB_DEVICE_Init();

	//Инициализация LED устройства
	dev->max2719->intens		= MAX2719_Intensity_11_32;
	dev->max2719->scanlimit		= MAX2719_ScanLimit_0123;
	dev->max2719->digitbegin	= MAX2719_Addr_DIG_0;
	dev->max2719->digitend		= MAX2719_Addr_DIG_3;

	MAX2719_InitDev(dev->max2719);
	MAX2719_SetUint32(dev->max2719,0);

	//Инициализация кнопк
	BUTTOM_InitDev(dev->buttom);

	//LCD дисплей
	HD44780_I2C_InitDev(dev->hd44780_i2c);

	//Инициализация часов реального времени
	RTC_InitDev(dev->rtc);
	RTC_InitDevSecondIT(dev->rtc);

	//Инициализация таймера
	TIM_Base_InitDev(dev->tim3_base);
	TIM_Base_Start_IT(dev->tim3_base);

	//Инициализация светодиодов
	GPIO_Test_Init(TLED_PC13);
	GPIO_InitPin(dev->led,1);

	GPIO_Test_Off(TLED_PC13);
	GPIO_WritePin(*dev->led,DISABLE);
}
//------------------------------------------------------------------------------
static uint8_t flag_rand_start = 0;
static uint8_t flag_rand_clean = 0;
//------------------------------------------------------------------------------
#define BUTTOM_START	0
#define BUTTOM_CLEAN	1
//------------------------------------------------------------------------------
void DEVICE_Loop(Struct_Device *dev)
{
	if(dev->flag_update_1hz) {

		//Формируем строку со временем
		char buff[64];
		memset(buff,0x00,sizeof(char)*64);
		time_t RTC_Time = RTC_Read(&dev->rtc->hrtc);

		struct tm * stm = localtime( &RTC_Time );
		strftime(buff,64,"Time:%X\n",stm);

		//Очистка LCD дисплея
		HD44780_I2C_DisplayClear(dev->hd44780_i2c);

		//Вывод на LCD дисплей строки со временем
		HD44780_I2C_vsnprintf(	dev->hd44780_i2c,
								HD44780_DDRAM_Line0,
								"%s",
								buff);

		//Переключаем уровень светодиода
		GPIO_Test_Toggle(TLED_PC13);

		//Выводим строку по uart
		printf(buff);

		int len = strlen(buff) + 1;
		buff[len-1] = '\r';

		//Выводим строку по usb
		CDC_Transmit_FS((uint8_t*)buff,len);

		//Установим семя рандома по времени rtc
		time_t Time = RTC_Read(&dev->rtc->hrtc);
		srand(Time);

		//Обнуляем флаг 1гц
		dev->flag_update_1hz = 0;
	}

	if(dev->flag_update_2hz) {
		//Переключаем уровень светодиода
		GPIO_TogglePin(*dev->led);

		//Обнуляем флаг 2гц
		dev->flag_update_2hz = 0;
	}

	if(dev->flag_update_10hz) {

		//Если запустили рулетку
		if(flag_rand_start) {

			static int speed = 0;
			static int tick = 0;

			if(tick >= speed - 1) {

				tick = 0;

				//Читаем сгенерированное число и скорость
				int rand = RandAlg_Step(dev->ra,&speed);

				//Установим семя рандома
				srand(HAL_GetTick());

				if(speed >= 0) {

					//Выведем число на LED дисплей
					MAX2719_SetUint32(dev->max2719,rand);
				}
				printf("speed:%i rand:%i\n",speed,rand);

			} else {
				tick++;
			}

			if(speed < 0) {

				//Закончили обнуляем значения
				flag_rand_start = 0;
				speed = 0;
				tick = 0;
			}
		}

		//Обнуляем флаг 10гц
		dev->flag_update_10hz = 0;
	}

	if(dev->flag_update_100hz) {

		//Сканируем кнопки
		BUTTOM_Scan(dev->buttom);

		//Если сработал флаг готовности чтеня кнопок
		if(GET_BUTTOM_IS_READY(dev->buttom)) {

			//Опрашиваем кнопки
			for(int i = 0; i < GET_BUTTOM_SIZE(dev->buttom); i++) {

				//Сработал тригер на нажатие и удержание
				if(GET_BUTTOM_TRIG_DOWN_TO_UP(dev->buttom,i)) {
					printf("BUTTOM TrigDownToUp buttom %d\n",i);

					//Если была зажата клавиша старт
					if(i == BUTTOM_START) {

						//Проверяем состояние
						if(!flag_rand_start && !flag_rand_clean) {

							//Проинициализируем структуру алгоритма обработки рандома
							RandAlg_InitDev(dev->ra);

							RandAlg_SetMinMax(dev->ra,5,100);

							//Установим флаг генерации и вывода на LED дисплей числа
							flag_rand_start = 1;

							//Установим флаг стирания при нажатии сброса
							flag_rand_clean = 1;
						}
					}
					if(i == BUTTOM_CLEAN ) {

						//Если чет закончен и есть флаг на стирание
						if(flag_rand_clean  && !flag_rand_start ) {

							//Установим значение на LED дисплее 0
							MAX2719_SetUint32(dev->max2719,0);

							//Отчистим флаги
							flag_rand_start = 0;
							flag_rand_clean = 0;
						}
					}
				}
			}
			//Отчищаем тригеры срабатывания
			BUTTOM_Clean(dev->buttom);
		}

		//Обнуляем флаг 100гц
		dev->flag_update_100hz = 0;
	}
}
//------------------------------------------------------------------------------
