//------------------------------------------------------------------------------
#include "hd44780_i2c.h"
//------------------------------------------------------------------------------
void HD44780_I2C_InitDev(HD44780_I2C_Device *dev)
{
	PCF8574_InitDev(&dev->pcf8574);
	printf("HD44780_I2C_InitDev OK\n");
}
//------------------------------------------------------------------------------
void HD44780_I2C_DeInitDev(HD44780_I2C_Device *dev)
{
	HD44780_I2C_DeInitDev(&dev->pcf8574);
	printf("HD44780_I2C_DeInitDev OK\n");
}
//------------------------------------------------------------------------------
void HD44780_I2C_Write(HD44780_I2C_Device *dev)
{
	HAL_StatusTypeDef ret;
	ret = PCF8574_Write(&dev->pcf8574,&dev->data,1);
	printf("HD44780_I2C_Write Ret:%i\n",ret);
}
//------------------------------------------------------------------------------
static GPIO_InitDevTypeDef GPIO_HD44780_I2C_PinInit[] = {
	{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pin		=	GPIO_PIN_7,
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pin		=	GPIO_PIN_6,
		},
		.gpio_typedef	=	GPIOB,
	}
};
//------------------------------------------------------------------------------
static HD44780_I2C_Device HD44780_I2C_Dev = {
	.pcf8574 = {
		.i2c = {
			.hi2c = {
					.Instance = I2C1,
					.Init = {
					.ClockSpeed       = 10000,
					.DutyCycle        = I2C_DUTYCYCLE_2,
					.OwnAddress1      = 0,
					.AddressingMode   = I2C_ADDRESSINGMODE_7BIT,
					.DualAddressMode  = I2C_DUALADDRESS_DISABLE,
					.OwnAddress2      = 0,
					.GeneralCallMode  = I2C_GENERALCALL_DISABLE,
					.NoStretchMode    = I2C_NOSTRETCH_DISABLE,
				}
			},
			.pins = GPIO_HD44780_I2C_PinInit,
			.pins_size = ARRAY_SIZE(GPIO_HD44780_I2C_PinInit),
		},
		.i2c_addr = 0x27,
	}
};
//------------------------------------------------------------------------------
#define LCD_COM_A0				(0)
#define LCD_COM_RW				(1)
#define LCD_COM_E				(2)
#define LCD_LED					(3)

#define LCD_A0_SET()			SBIT(dev->data,LCD_COM_A0)
#define LCD_A0_CLR()			CBIT(dev->data,LCD_COM_A0)

#define LCD_RW_SET()			SBIT(dev->data,LCD_COM_RW)
#define LCD_RW_CLR()			CBIT(dev->data,LCD_COM_RW)

#define LCD_E_SET()				SBIT(dev->data,LCD_COM_E)
#define LCD_E_CLR()				CBIT(dev->data,LCD_COM_E)

#define LCD_LED_SET()			SBIT(dev->data,LCD_LED)
#define LCD_LED_CLR()			CBIT(dev->data,LCD_LED)

#define LCD_DATA_4				(4)
#define LCD_DATA_5				(5)
#define LCD_DATA_6				(6)
#define LCD_DATA_7				(7)

#define LCD_DATA_4_SET()		SBIT(dev->data,LCD_DATA_4)
#define LCD_DATA_5_SET()		SBIT(dev->data,LCD_DATA_5)
#define LCD_DATA_6_SET()		SBIT(dev->data,LCD_DATA_6)
#define LCD_DATA_7_SET()		SBIT(dev->data,LCD_DATA_7)

#define LCD_DATA_4_CLR()		CBIT(dev->data,LCD_DATA_4)
#define LCD_DATA_5_CLR()		CBIT(dev->data,LCD_DATA_5)
#define LCD_DATA_6_CLR()		CBIT(dev->data,LCD_DATA_6)
#define LCD_DATA_7_CLR()		CBIT(dev->data,LCD_DATA_7)

#define LCD_CLEAN_DATA()		CMASK(dev->data,(BIT(LCD_DATA_4)| BIT(LCD_DATA_5)|\
													BIT(LCD_DATA_6)| BIT(LCD_DATA_7)))


//---------------------------------------------------------------------------------------------------
//Clear Display Очищает индикатор и помещает курсор в самую левую позицию
#define LCD_CLR_DIS			BIT(0)
//---------------------------------------------------------------------------------------------------
//Return Home Перемещает курсор в левую позицию
#define LCD_RET_HOME		BIT(1)
//---------------------------------------------------------------------------------------------------
//Entry Mode Set Установка направления сдвига курсора
#define LCD_MOD_SET			BIT(2)
//ID=0/1—влево/вправо
#define LCD_MOD_SET_ID		BIT(1)
//разрешение сдвига дисплея (SH=1) при за писи в DDRAM
#define LCD_MOD_SET_SH		BIT(0)
//---------------------------------------------------------------------------------------------------
//Display ON/OFF control
#define LCD_DIS_CON			BIT(3)
//Включает индикатор (D=1)
#define LCD_DIS_CON_D		BIT(2)
//Выбирает тип курсора (C, B)
#define LCD_DIS_CON_C		BIT(1)
#define LCD_DIS_CON_B		BIT(0)
//---------------------------------------------------------------------------------------------------
//Cursor or Display Shift
//Выполняет сдвиг дисплея или курсора
//(SC=0/1—курсор/дисплей, RL=0/1—влево/вправо)
#define LCD_CUR_SH			BIT(4)
#define LCD_CUR_SH_SC		BIT(3)
#define LCD_CUR_SH_RL		BIT(2)
//---------------------------------------------------------------------------------------------------
//Function Set Установка разрядности интерфейса (DL=0/1—4/8 бита) и
//страницы знакогенератора P
#define LCD_FUN_SET			(BIT(5) | BIT(3))
#define LCD_FUN_SET_DL		BIT(4)
#define LCD_FUN_SET_P		BIT(2)

//Set CGRAM Address
#define LCD_SET_CGRAM		BIT(6)


//Set DDRAM Address
#define LCD_SET_DDRAM 		BIT(7)
//------------------------------------------------------------------------------
static void LcdEnable(HD44780_I2C_Device *dev)
{
	printf("0x%.2x\n",dev->data);
	HD44780_I2C_Write(dev);
	HAL_Delay(1);
	LCD_E_SET();
	HAL_Delay(1);
	HD44780_I2C_Write(dev);
	HAL_Delay(1);
	LCD_E_CLR();
	HAL_Delay(1);
	HD44780_I2C_Write(dev);
}
//------------------------------------------------------------------------------
static void LcdSetData(HD44780_I2C_Device *dev,uint8_t data)
{
	//LcdSetDataOut();
	LCD_RW_CLR();

	//uint8_t highnib=data & 0xf0;
	//uint8_t lownib=(data<<4)&0xf0;

	LCD_E_SET();
	HD44780_I2C_Write(dev);
	HAL_Delay(1);

	TBIT1(data,4)?LCD_DATA_4_SET():LCD_DATA_4_CLR();
	TBIT1(data,5)?LCD_DATA_5_SET():LCD_DATA_5_CLR();
	TBIT1(data,6)?LCD_DATA_6_SET():LCD_DATA_6_CLR();
	TBIT1(data,7)?LCD_DATA_7_SET():LCD_DATA_7_CLR();

	HD44780_I2C_Write(dev);
	HAL_Delay(1);

	LcdEnable(dev);
	//_delay_us(4);
	//LCD_E_CLR();
	//_delay_us(4);
	//LCD_E_SET();

	LCD_E_SET();
	HD44780_I2C_Write(dev);
	HAL_Delay(1);

	TBIT1(data,0)?LCD_DATA_4_SET():LCD_DATA_4_CLR();
	TBIT1(data,1)?LCD_DATA_5_SET():LCD_DATA_5_CLR();
	TBIT1(data,2)?LCD_DATA_6_SET():LCD_DATA_6_CLR();
	TBIT1(data,3)?LCD_DATA_7_SET():LCD_DATA_7_CLR();

	HD44780_I2C_Write(dev);
	HAL_Delay(1);

	LcdEnable(dev);
	//_delay_us(4);
	//LCD_E_CLR();
	//_delay_us(4);*/

	//dev->data

}

void LcdCommand(HD44780_I2C_Device *dev,uint8_t data)
{
	//LcdReady(dev);
	//_delay_us(10);
	LCD_A0_CLR();
	HD44780_I2C_Write(dev);
	LcdSetData(dev,data);
	HAL_Delay(50);
}
//------------------------------------------------------------------------------
static void hd_test_init(HD44780_I2C_Device *dev)
{
	dev->data = 0x00;

	LCD_LED_SET();

	LCD_E_CLR();
	LCD_A0_CLR();
	LCD_RW_CLR();

	HD44780_I2C_Write(dev);
	HAL_Delay(100);

	LCD_DATA_4_SET();
	LCD_DATA_5_SET();

	HD44780_I2C_Write(dev);
	HAL_Delay(1);

	LcdEnable(dev);
	HAL_Delay(50);

	LcdEnable(dev);
	HAL_Delay(50);

	LcdEnable(dev);
	HAL_Delay(1);

	LCD_CLEAN_DATA();
	LCD_DATA_5_SET();

	HD44780_I2C_Write(dev);
	HAL_Delay(1);

	LcdEnable(dev);

	//LcdEnable(dev);

	HAL_Delay(150);

	LcdCommand(dev,LCD_FUN_SET | LCD_FUN_SET_P);
	LcdCommand(dev,LCD_DIS_CON | LCD_DIS_CON_D);
	LcdCommand(dev,LCD_CLR_DIS);

	LcdCommand(dev,LCD_SET_DDRAM | 0x00);
	LcdCommand(dev,LCD_DIS_CON | LCD_DIS_CON_D | LCD_DIS_CON_B );

}
//------------------------------------------------------------------------------
void HD44780_I2C_TestInit()
{
	HD44780_I2C_InitDev(&HD44780_I2C_Dev);

	hd_test_init(&HD44780_I2C_Dev);


	/*LcdSetDataOut();
	LcdComPinInit();

	LCD_E_CLR();
	LCD_A0_CLR();
	LCD_RW_CLR();

	uint8_t s = 0;

	LcdCleanDataPin();
	SBIT(LCD_DATA_4_PORT,LCD_DATA_4_P);
	SBIT(LCD_DATA_5_PORT,LCD_DATA_5_P);

	while (s<3) {
		LcdEnable();
		_delay_ms(50);
		s++;
	}

	LcdCleanDataPin();
	SBIT(LCD_DATA_5_PORT,LCD_DATA_5_P);

	LcdEnable();
	LcdEnable();

	for(s=0;s<10;s++)
		LcdReady();

	LcdCommand(LCD_FUN_SET | LCD_FUN_SET_P);
	LcdCommand(LCD_DIS_CON | LCD_DIS_CON_D);
	LcdCommand(LCD_CLR_DIS);*/



}
//------------------------------------------------------------------------------
void HD44780_I2C_TestLed()
{
	uint8_t data[1];
	data[0] = BIT(3);

	HAL_Delay(1000);
	HAL_StatusTypeDef ret;
	ret = PCF8574_Write(&HD44780_I2C_Dev.pcf8574,data,1);
	printf("Ret:%i\n",ret);

	data[0] = 0;
	HAL_Delay(1000);
	ret = PCF8574_Write(&HD44780_I2C_Dev.pcf8574,data,1);
	printf("Ret:%i\n",ret);
}
//------------------------------------------------------------------------------
void HD44780_I2C_TestLoop()
{
	//HD44780_I2C_TestLed();

	//hd_test_init(&HD44780_I2C_Dev);
}
//------------------------------------------------------------------------------
