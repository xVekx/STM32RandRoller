#include "max2719.h"
#include "def.h"
#include "stdio.h"
#include "string.h"
//------------------------------------------------------------------------------
void MAX2719_InitDev(MAX2719_DevDevice *dev)
{
	SPI_InitDev(&dev->spi);
}
//------------------------------------------------------------------------------
void MAX2719_DeInitDev(MAX2719_DevDevice *dev)
{
	SPI_DeInitDev(&dev->spi);
}
//------------------------------------------------------------------------------
static void MAX2719_Write(MAX2719_DevDevice *dev,uint8_t addres,uint8_t command)
{
	if(dev == NULL || dev->spi.pins == NULL)
		return;

	uint16_t pack = (addres << 8) | command;

	SPI_HandleTypeDef *hspi = &dev->spi.hspi;
	const GPIO_Device *gpio = dev->spi.pins;

	uint8_t nss = dev->NSS;
	GPIO_Device pin_nss = gpio[nss];
	GPIO_WritePin(pin_nss,GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi,(uint8_t*)&pack,1,0xFFF);
	GPIO_WritePin(pin_nss,GPIO_PIN_SET);
}
//------------------------------------------------------------------------------
void MAX2719_CommNOOP(MAX2719_DevDevice *dev)
{
	MAX2719_Write(dev,MAX2719_Addr_NOOP,MAX2719_Addr_NOOP);
}
//------------------------------------------------------------------------------
void MAX2719_CommDecodeMode(MAX2719_DevDevice *dev,MAX2719_DecodeMode dm)
{
	MAX2719_Write(dev,MAX2719_Addr_DecodeMode,dm);
}
//------------------------------------------------------------------------------
void MAX2719_CommIntensity(MAX2719_DevDevice *dev,MAX2719_Intensity i)
{
	MAX2719_Write(dev,MAX2719_Addr_Intensity,i);
}
//------------------------------------------------------------------------------
void MAX2719_CommScanLimit(MAX2719_DevDevice *dev,MAX2719_ScanLimit sl)
{
	MAX2719_Write(dev,MAX2719_Addr_ScanLimit,sl);
}
//------------------------------------------------------------------------------
void MAX2719_CommShutdown(MAX2719_DevDevice *dev,MAX2719_Shutdown sd)
{
	MAX2719_Write(dev,MAX2719_Addr_Shutdown,sd);
}
//------------------------------------------------------------------------------
void MAX2719_CommDisplayTest(MAX2719_DevDevice *dev,MAX2719_DisplayTest dt)
{
	MAX2719_Write(dev,MAX2719_Addr_DisplayTest,dt);
}
//------------------------------------------------------------------------------
void MAX2719_SetSymbolAll(MAX2719_DevDevice *dev,uint8_t ch,MAX2719_Addr start,MAX2719_Addr begin)
{
	for(uint8_t i = start;i < begin + 1; i++) {
		MAX2719_Write(dev,i,ch);
	}
}
//------------------------------------------------------------------------------
void MAX2719_SetSymbol(MAX2719_DevDevice *dev,uint8_t *buff,MAX2719_Addr start,MAX2719_Addr begin)
{
	for(uint8_t i = start,b = 0;i < begin + 1; i++,b++) {
		MAX2719_Write(dev,(begin + 1) - i,buff[b]);
	}
}
//------------------------------------------------------------------------------
void MAX2719_SymbolDigitConvert(uint8_t *dest,uint8_t *src,int len)
{
	for(int i=0;i<len;i++) {
		if((0x30 <= src[i]) && (src[i] <= 0x39)) {
			dest[i] = src[i] - 0x30;
		} else {
			dest[i] = MAX2719_FontCode_Blank;
		}
	}
}
//------------------------------------------------------------------------------
void MAX2719_SetUint32(MAX2719_DevDevice *dev,uint32_t i)
{
	MAX2719_CommNOOP(dev);
	MAX2719_CommDisplayTest(dev,MAX2719_DisplayTestOff);
	MAX2719_CommDecodeMode(dev,MAX2719_DecodeModeFULL);
	MAX2719_CommIntensity(dev,dev->intens);
	MAX2719_CommShutdown(dev,MAX2719_ShutdownOn);
	MAX2719_CommScanLimit(dev,dev->scanlimit);

	char buff[16];
	uint8_t fontbuff[16];
	for(int i=0;i<16;i++) {
		buff[i] = 0;
		fontbuff[i] = MAX2719_FontCode_Blank;
	}

	sprintf(buff,"%04lu",(uint32_t)(i));

	MAX2719_SymbolDigitConvert(fontbuff,(uint8_t*)&buff[1],3);
	MAX2719_SetSymbol(dev,fontbuff,dev->digitbegin,dev->digitend);
}
#if 0
//------------------------------------------------------------------------------
static const GPIO_Device GPIO_SpiPinInit[] = {
	{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_OUTPUT_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_12,	//SPI2_NSS
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_13 |	//SPI2_SCK
							GPIO_PIN_15,	//SPI2_MOSI
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_INPUT,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_14,	//SPI2_MISO
		},
		.gpio_typedef	=	GPIOB,
	}
};
//------------------------------------------------------------------------------
static MAX2719_DevDevice	max2719 = {
	.spi =  {
		.hspi = {
			.Instance = SPI2,
			.Init = {
				.Mode				= SPI_MODE_MASTER,
				.Direction			= SPI_DIRECTION_2LINES,
				.DataSize			= SPI_DATASIZE_16BIT,
				.CLKPolarity		= SPI_POLARITY_LOW,
				.CLKPhase			= SPI_PHASE_1EDGE,
				.NSS				= SPI_NSS_SOFT,
				.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_256,
				.FirstBit			= SPI_FIRSTBIT_MSB,
				.TIMode				= SPI_TIMODE_DISABLE,
				.CRCCalculation		= SPI_CRCCALCULATION_DISABLE,
				.CRCPolynomial		= 10,
			}
		},
		.pins = GPIO_SpiPinInit,
		.pins_size = ARRAY_SIZE(GPIO_SpiPinInit),
	},
	.NSS = 0,
};
//------------------------------------------------------------------------------
void MAX2719_Test1(MAX2719_DevDevice *dev)
{
	MAX2719_CommNOOP(dev);
	MAX2719_CommDisplayTest(dev,MAX2719_DisplayTestOff);
	MAX2719_CommShutdown(dev,MAX2719_ShutdownOn);
	MAX2719_CommDecodeMode(dev,MAX2719_DecodeModeFULL);
	MAX2719_CommIntensity(dev,MAX2719_Intensity_03_32);
	MAX2719_CommScanLimit(dev,MAX2719_ScanLimit_01234567);

	MAX2719_Write(dev,0x01,0x07);
	MAX2719_Write(dev,0x02,0x07);
	MAX2719_Write(dev,0x03,0x07);
	MAX2719_Write(dev,0x03,0x07);
	//MAX2719_SetSymbolAll(dev,MAX2719_Addr_DIG_0,MAX2719_Addr_DIG_3,MAX2719_FontCode_0);
	HAL_Delay(1000);
}
//------------------------------------------------------------------------------
void MAX2719_TestInit()
{
	max2719.intens		= MAX2719_Intensity_11_32;
	max2719.scanlimit	= MAX2719_ScanLimit_01234567;
	max2719.digitbegin	= MAX2719_Addr_DIG_0;
	max2719.digitend	= MAX2719_Addr_DIG_3;
	MAX2719_InitDev(&max2719);
}
//------------------------------------------------------------------------------
void MAX2719_TestLoop()
{
	static int test = 0;
	if(test > 9999) {
		test = 0;
	}
	test++;
	MAX2719_SetUint32(&max2719,test);
	HAL_Delay(10);
}
//------------------------------------------------------------------------------
#else
void MAX2719_TestInit();
void MAX2719_TestLoop();
#endif
