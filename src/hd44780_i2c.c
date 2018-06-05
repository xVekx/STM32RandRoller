//------------------------------------------------------------------------------
#include "hd44780_i2c.h"
#include <string.h>
#include <stdarg.h>
//------------------------------------------------------------------------------
#define LCD_A0_SET()			(dev->data.bit.a0 = 1)
#define LCD_A0_CLR()			(dev->data.bit.a0 = 0)

#define LCD_RW_SET()			(dev->data.bit.rw = 1)
#define LCD_RW_CLR()			(dev->data.bit.rw = 0)

#define LCD_E_SET()				(dev->data.bit.e = 1)
#define LCD_E_CLR()				(dev->data.bit.e = 0)

#define LCD_LED_SET()			(dev->data.bit.led = 1)
#define LCD_LED_CLR()			(dev->data.bit.led = 0)

#define LCD_DATA_CLEAN()		(dev->data.bit.d = 0x00)
#define LCD_DATA_SET(_b)		(dev->data.bit.d = (_b))
//------------------------------------------------------------------------------
static int CharUtf8ToUnc(char *text,int *size)
{
	int ret = -1;

	if(TBIT0(text[0],7)) {
		ret = text[0];
		if(size != NULL) {
			*size += 1;
		}
	} else if(	TBIT1(text[0],7) && TBIT1(text[0],6) && TBIT0(text[0],5)) {

		uint8_t u0 = text[0]; uint8_t u1 = text[1];

		CMASK(u0,(BIT(7)|BIT(6)));
		CMASK(u1,(BIT(7)));
		ret = 0;
		SMASK(ret,(u0 << 6u));
		SMASK(ret,u1);
		if(size != NULL) {
			*size += 2;
		}
	} else if(	TBIT1(text[0],7) && TBIT1(text[0],6) && TBIT1(text[0],5) &&
				TBIT0(text[0],4)) {

		uint8_t u0 = text[0]; uint8_t u1 = text[1]; uint8_t u2 = text[2];

		CMASK(u0,(BIT(7)|BIT(6)|BIT(5)));
		CMASK(u1,(BIT(7)));
		CMASK(u2,(BIT(7)));

		ret = 0;

		SMASK(ret,(u0 << 12u));
		SMASK(ret,(u1 << 6u));
		SMASK(ret,u2);

		if(size != NULL) {
			*size += 3;
		}
	} else if(	TBIT1(text[0],7) && TBIT1(text[0],6) && TBIT1(text[0],5) &&
				TBIT1(text[0],4) && TBIT0(text[0],3)) {
		ret = -1;

		if(size != NULL) {
			*size += 4;
		}
	}

	if(ret < 0) {
		fprintf(stderr,"CharUtf8ToUnc NOT CONV\n");
	}

	return ret;
}
//------------------------------------------------------------------------------
static void HD44780_I2C_Write(HD44780_I2C_Device *dev)
{
	if(!dev->error) {
		HAL_StatusTypeDef ret;
		ret = PCF8574_Write(&dev->pcf8574,&dev->data.data,1);
		if(ret != HAL_OK) {
			printf("HD44780_I2C_Write Ret:%i\n",ret);
			dev->error = 1;
		}
	}
}
//------------------------------------------------------------------------------
static void HD44780_I2C_Enable(HD44780_I2C_Device *dev)
{
	HD44780_I2C_Write(dev);
	LCD_E_SET();
	HD44780_I2C_Write(dev);
	LCD_E_CLR();
	HD44780_I2C_Write(dev);
}
//------------------------------------------------------------------------------
void HD44780_I2C_LedOn(HD44780_I2C_Device *dev)
{
	LCD_LED_SET();
	HD44780_I2C_Write(dev);
}
//------------------------------------------------------------------------------
void HD44780_I2C_LedOff(HD44780_I2C_Device *dev)
{
	LCD_LED_CLR();
	HD44780_I2C_Write(dev);
}
//------------------------------------------------------------------------------
static void HD44780_I2C_SetData(HD44780_I2C_Device *dev,uint8_t data)
{
	LCD_RW_CLR();
	LCD_E_SET();

	dev->data.bit.d = data >> 4;
	HD44780_I2C_Enable(dev);

	dev->data.bit.d = data;
	HD44780_I2C_Enable(dev);
}
//------------------------------------------------------------------------------
void HD44780_I2C_Command(HD44780_I2C_Device *dev,uint8_t data)
{
	//LcdReady(dev);
	//_delay_us(10);
	LCD_A0_CLR();
	HD44780_I2C_SetData(dev,data);
}
//------------------------------------------------------------------------------
void HD44780_I2C_Data(HD44780_I2C_Device *dev,uint8_t data)
{
	//LcdReady();
	//_delay_us(10);
	LCD_A0_SET();
	HD44780_I2C_SetData(dev,data);
}
//------------------------------------------------------------------------------
void HD44780_I2C_DisplayClear(HD44780_I2C_Device *dev)
{
	HD44780_I2C_Command(dev,HD44780_Comm_DisplayClear);
	HAL_Delay(2);
}
//------------------------------------------------------------------------------
void HD44780_I2C_ReturnHome(HD44780_I2C_Device *dev)
{
	HD44780_I2C_Command(dev,HD44780_Comm_ReturnHome);
}
//------------------------------------------------------------------------------
void HD44780_I2C_EntryModeSet(HD44780_I2C_Device *dev,HD44780_EntryModeSet ems)
{
	HD44780_I2C_Command(dev,HD44780_Comm_EntryModeSet | ems);
}
//------------------------------------------------------------------------------
void HD44780_I2C_DisplayControl(HD44780_I2C_Device *dev,HD44780_DisplayControl dc)
{
	HD44780_I2C_Command(dev,HD44780_Comm_DisplayControl | dc);
}
//------------------------------------------------------------------------------
void HD44780_I2C_CursOrDispShift(HD44780_I2C_Device *dev,HD44780_CursOrDispShift cods)
{
	HD44780_I2C_Command(dev,HD44780_Comm_CursOrDispShift | cods);
}
//------------------------------------------------------------------------------
void HD44780_I2C_FunctionSet(HD44780_I2C_Device *dev,HD44780_FunctionSet fs)
{
	HD44780_I2C_Command(dev,HD44780_Comm_FunctionSet | fs);
}
//------------------------------------------------------------------------------
void HD44780_I2C_SetDDRAM(HD44780_I2C_Device *dev,HD44780_DDRAM addr)
{
	HD44780_I2C_Command(dev,HD44780_Comm_DDRAM | addr);
}
//------------------------------------------------------------------------------
int HD44780_I2C_vsnprintf(	HD44780_I2C_Device *dev,
							HD44780_DDRAM addr,
							const char *text,
							...)
{
	HD44780_I2C_SetDDRAM(dev,addr);

	const int buff_size = 16;
	char buff[buff_size + 2];
	memset(buff,0,buff_size + 2);

	va_list vl;
	va_start(vl,text);
	int ret = vsnprintf(buff,buff_size,text,vl);
	va_end(vl);

	int size = strlen(buff);

	for(int i=0;i<size;) {
		int cc = CharUtf8ToUnc(&buff[i],&i);
		//printf("0x%.2x\n",cc);
		if((cc < 0) || (cc == '\0'))
			break;
		if(cc != '\n' && cc != '\r') {
			HD44780_I2C_Data(dev,cc);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
void HD44780_I2C_InitDev(HD44780_I2C_Device *dev)
{
	PCF8574_InitDev(&dev->pcf8574);
	dev->error = 0;
	printf("HD44780_I2C_InitDev OK\n");

	HAL_Delay(40);

	LCD_DATA_CLEAN();
	LCD_LED_SET();

	LCD_E_CLR();
	LCD_A0_CLR();
	LCD_RW_CLR();

	LCD_DATA_SET((HD44780_Comm_FunctionSet | HD44780_FunctionSet_8bit) >> 4);
	HD44780_I2C_Enable(dev);
	HAL_Delay(1);

	HD44780_I2C_Enable(dev);
	HAL_Delay(1);

	HD44780_I2C_Enable(dev);
	HAL_Delay(1);

	LCD_DATA_CLEAN();
	LCD_DATA_SET(HD44780_Comm_FunctionSet>>4);
	HD44780_I2C_Enable(dev);
	HAL_Delay(1);

	//HD44780_I2C_FunctionSet(dev,HD44780_FunctionSet_Page);
	HD44780_I2C_DisplayControl(dev,HD44780_DisplayControl_DOn);
	HD44780_I2C_DisplayClear(dev);
}
//------------------------------------------------------------------------------
void HD44780_I2C_DeInitDev(HD44780_I2C_Device *dev)
{
	PCF8574_DeInitDev(&dev->pcf8574);
	printf("HD44780_I2C_DeInitDev OK\n");
}
#if 0
//------------------------------------------------------------------------------
static GPIO_Device GPIO_HD44780_I2C_PinInit[] = {
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
					.ClockSpeed       = 100000,
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
static void HD44780_I2C_Test1(HD44780_I2C_Device *dev)
{
	HD44780_I2C_DisplayClear(dev);

	static int i=0;

	HD44780_I2C_vsnprintf(dev,HD44780_DDRAM_Line0,"Time:%i",i);
	HD44780_I2C_vsnprintf(dev,HD44780_DDRAM_Line1,"Time:%i",i+10058);

	i++;

	HAL_Delay(100);
}
//------------------------------------------------------------------------------
void HD44780_I2C_TestInit()
{
	HD44780_I2C_InitDev(&HD44780_I2C_Dev);
}
//------------------------------------------------------------------------------
void HD44780_I2C_TestLed()
{
	uint8_t data[1];
	data[0] = BIT(3);

	HAL_Delay(1000);
	HAL_StatusTypeDef ret;
	ret = PCF8574_Write(&HD44780_I2C_Dev.pcf8574,data,1);
	//printf("Ret:%i\n",ret);

	data[0] = 0;
	HAL_Delay(1000);
	ret = PCF8574_Write(&HD44780_I2C_Dev.pcf8574,data,1);
	//printf("Ret:%i\n",ret);
}
//------------------------------------------------------------------------------
void HD44780_I2C_TestLoop()
{
	HD44780_I2C_Test1(&HD44780_I2C_Dev);
}
//------------------------------------------------------------------------------
#endif
