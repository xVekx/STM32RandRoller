#include "i2c.h"
#include "def.h"
//------------------------------------------------------------------------------
extern void _Error_Handler(char *file, int line);
//------------------------------------------------------------------------------
#define I2C_CLK(_port,_port_sel,_stat_sel)									\
					{if(_port == _port_sel)									\
						if(_stat_sel) {										\
						 __HAL_RCC_##_port##_FORCE_RESET();					\
						 __HAL_RCC_##_port##_RELEASE_RESET();				\
						 __HAL_RCC_##_port##_CLK_ENABLE();}					\
						 else												\
						 __HAL_RCC_##_port##_CLK_DISABLE();}
//------------------------------------------------------------------------------
static void I2C_Clk(I2C_TypeDef *i2c,FunctionalState stat)
{
#if defined(I2C1)
	I2C_CLK(I2C1,i2c,stat);
#endif

#if defined(I2C2)
	I2C_CLK(I2C2,i2c,stat);
#endif

#if defined(I2C3)
	I2C_CLK(I2C3,i2c,stat);
#endif

#if defined(I2C4)
	I2C_CLK(I2C4,i2c,stat);
#endif
}
//------------------------------------------------------------------------------
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
}
//------------------------------------------------------------------------------
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
}
//------------------------------------------------------------------------------
void I2C_InitDev(I2C_Device *dev)
{
	I2C_Clk(dev->hi2c.Instance,ENABLE);
	GPIO_InitPin(dev->pins,dev->pins_size);
	if (HAL_I2C_Init(&dev->hi2c) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	printf("I2C_InitDev\n");
}
//------------------------------------------------------------------------------
void I2C_DeInitDev(I2C_Device *dev)
{
	if (HAL_I2C_Init(&dev->hi2c) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	GPIO_DeInitPin(dev->pins,dev->pins_size);
	I2C_Clk(dev->hi2c.Instance,DISABLE);
	printf("I2C_DeInitDev\n");
}
//------------------------------------------------------------------------------
HAL_StatusTypeDef I2C_Write	(	I2C_Device *dev,
								uint16_t addr,
								uint8_t *data,
								uint16_t size )
{
	HAL_StatusTypeDef ret;
	while (HAL_I2C_GetState(&dev->hi2c) != HAL_I2C_STATE_READY);
	ret = HAL_I2C_Master_Transmit(&dev->hi2c,addr,data,size,0xFF);
	return ret ;
}
//------------------------------------------------------------------------------
HAL_StatusTypeDef I2C_Read	(	I2C_Device *dev,
								uint16_t addr,
								uint8_t *data,
								uint16_t size )
{
	HAL_StatusTypeDef ret;
	while (HAL_I2C_GetState(&dev->hi2c) != HAL_I2C_STATE_READY);
	ret = HAL_I2C_Master_Transmit(&dev->hi2c,addr,data,size,0xFF);
	return ret ;
}
//------------------------------------------------------------------------------
#if 0
static GPIO_InitDevTypeDef GPIO_I2cPinInit[] = {
	{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			//.Pin		=	GPIO_PIN_10,
			.Pin		=	GPIO_PIN_7,
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			//.Pin		=	GPIO_PIN_11,
			.Pin		=	GPIO_PIN_6,
		},
		.gpio_typedef	=	GPIOB,
	}
};
//------------------------------------------------------------------------------
static I2C_InitDevTypeDev I2C_DevInit = {
		.hi2c = {
			.Instance = I2C1,
			//.Instance = I2C2,
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
		.pins = GPIO_I2cPinInit,
		.pins_size = ARRAY_SIZE(GPIO_I2cPinInit),
};
//------------------------------------------------------------------------------
//S0100A2A1A00A
void I2C_TestInit()
{
	I2C_InitDev(&I2C_DevInit);
}
//------------------------------------------------------------------------------
void I2C_GPIO_TEST(I2C_InitDevTypeDev *dev)
{
	for(int i = 0; i< dev->pins_size;i++) {
		dev->pins[i].gpio_inittypedef.Mode = GPIO_MODE_OUTPUT_OD;
	}

	GPIO_InitPin(dev->pins,dev->pins_size);

	HAL_Delay(1);
	for(int i = 0; i< dev->pins_size;i++) {
		GPIO_WritePin(dev->pins[i],GPIO_PIN_SET);
	}

	HAL_Delay(1);
	for(int i = 0; i< dev->pins_size;i++) {
		GPIO_WritePin(dev->pins[i],GPIO_PIN_RESET);
	}

	GPIO_DeInitPin(I2C_DevInit.pins,I2C_DevInit.pins_size);

	for(int i = 0; i< dev->pins_size;i++) {
		dev->pins[i].gpio_inittypedef.Mode = GPIO_MODE_AF_OD;
	}
}
//------------------------------------------------------------------------------
void I2C_TestLoop()
{
	I2C_HandleTypeDef *hi2c = &I2C_DevInit.hi2c;

	uint8_t data[1];

	uint16_t dev_addr = 0x4E;

	//4e 4f 27

	//HAL_I2C_DeInit(hi2c);

	HAL_Delay(1000);

	//HAL_I2C_Init(hi2c);

	HAL_StatusTypeDef ret;

	data[0] = BIT(3);
	ret = I2C_Write(&I2C_DevInit,dev_addr,data,1);

	printf("Ret:%i\n",ret);
	HAL_Delay(1000);

	data[0] = 0x00;
	ret = I2C_Write(&I2C_DevInit,dev_addr,data,1);
	printf("Ret:%i\n",ret);
}
#else
void I2C_TestInit()
{

}

void I2C_TestLoop()
{

}
#endif
