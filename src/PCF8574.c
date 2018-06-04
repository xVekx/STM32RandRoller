#include "PCF8574.h"
//------------------------------------------------------------------------------
void PCF8574_InitDev(PCF8574_Device *dev)
{
	I2C_InitDev(&dev->i2c);
	printf("PCF8574_InitDev OK\n");
}
//------------------------------------------------------------------------------
void PCF8574_DeInitDev(PCF8574_Device *dev)
{
	I2C_DeInitDev(&dev->i2c);
	printf("I2C_DeInitDev OK\n");
}
//------------------------------------------------------------------------------
HAL_StatusTypeDef PCF8574_Write(PCF8574_Device *dev, uint8_t *data,int size) {
	HAL_StatusTypeDef ret;
	ret = I2C_Write(&dev->i2c,(dev->i2c_addr << 1),data,size );
	return ret;
}
//------------------------------------------------------------------------------
HAL_StatusTypeDef PCF8574_Read(PCF8574_Device *dev, uint8_t *data,int size) {
	HAL_StatusTypeDef ret;
	ret = I2C_Read(&dev->i2c,(dev->i2c_addr << 1) | ( 1u << 0 ),data,size);
	return ret;
}
#if 0
//------------------------------------------------------------------------------
static GPIO_InitDevTypeDef GPIO_PCF8574PinInit[] = {
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
static PCF8574_Device PCF8574_DevInit = {

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
		.pins = GPIO_PCF8574PinInit,
		.pins_size = ARRAY_SIZE(GPIO_PCF8574PinInit),
	},
	.i2c_addr = 0x27,
};
//------------------------------------------------------------------------------
void PCF8574_TestInit()
{
	PCF8574_InitDev(&PCF8574_DevInit);
}
//------------------------------------------------------------------------------
void PCF8574_TestLoop()
{
	uint8_t data[1];
	data[0] = BIT(3);

	HAL_Delay(1000);
	HAL_StatusTypeDef ret;
	ret = PCF8574_Write(&PCF8574_DevInit,data,1);
	printf("Ret:%i\n",ret);

	data[0] = 0;
	HAL_Delay(1000);
	ret = PCF8574_Write(&PCF8574_DevInit,data,1);
	printf("Ret:%i\n",ret);
}
//------------------------------------------------------------------------------
#endif
