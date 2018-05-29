#include "i2c.h"
#include "def.h"
//------------------------------------------------------------------------------
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
#if defined( I2C1 )
	if( hi2c->Instance == I2C1 ) {
		__HAL_RCC_I2C1_CLK_ENABLE();
	}
#endif

#if defined( I2C2 )
	if( hi2c->Instance == I2C2 ) {
		__HAL_RCC_I2C2_CLK_ENABLE();
	}
#endif

#if defined( I2C3 )
	if( hi2c->Instance == I2C3 ) {
		__HAL_RCC_I2C3_CLK_ENABLE();
	}
#endif

#if defined( I2C4 )
	if( hi2c->Instance == I2C4) {
		__HAL_RCC_I2C4_CLK_ENABLE();
	}
#endif
}
//------------------------------------------------------------------------------
void I2C_InitDev(I2C_InitDevTypeDev *dev)
{
	__HAL_RCC_I2C1_CLK_ENABLE();
	GPIO_InitPin(dev->pins,dev->pins_size);
	 __HAL_AFIO_REMAP_I2C1_ENABLE();
	if (HAL_I2C_Init(&dev->hi2c) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
}
//------------------------------------------------------------------------------
/*    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
	_Error_Handler(__FILE__, __LINE__);
  }
*/

static const GPIO_InitDevTypeDef GPIO_I2cPinInit[] = {
	{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pin		=	GPIO_PIN_8,
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pin		=	GPIO_PIN_9,
		},
		.gpio_typedef	=	GPIOB,
	}
};

static I2C_InitDevTypeDev I2C_DevInit = {
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
		.pins = GPIO_I2cPinInit,
		.pins_size = ARRAY_SIZE(GPIO_I2cPinInit),
};

/*static I2C_InitTypeDef I2C3_InitType = {
	.ClockSpeed       = 100000,
	.DutyCycle        = I2C_DUTYCYCLE_2,
	.OwnAddress1      = 0,
	.AddressingMode   = I2C_ADDRESSINGMODE_7BIT,
	.DualAddressMode  = I2C_DUALADDRESS_DISABLED,
	.OwnAddress2      = 0,
	.GeneralCallMode  = I2C_GENERALCALL_DISABLED,
	.NoStretchMode    = I2C_NOSTRETCH_DISABLED,
};*/
//S0100A2A1A00A
void I2C_TestInit()
{
	I2C_InitDev(&I2C_DevInit);
}
//------------------------------------------------------------------------------
void I2C_TestLoop()
{
	static int test = 0;

	uint8_t a = 0xFF;

	uint8_t b = 0x00;

	uint8_t data[1];
	data[0] = 0x00;

	uint16_t dev_addr = 0x40;

	I2C_HandleTypeDef *hi2c = &I2C_DevInit.hi2c;

	__HAL_RCC_I2C1_CLK_DISABLE();
	HAL_I2C_DeInit(hi2c);
	HAL_I2C_Init(hi2c);

	if(test == 0) {
		test = 1;
		data[0] = 0x00;
		//GPIO_WritePin(GPIO_I2cPinInit[0],GPIO_PIN_SET);
		while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
		HAL_I2C_Master_Transmit(hi2c,dev_addr,data,1,0xFFF);

		/*if(ret != HAL_OK) {
			printf("I2C Ret: %i\n",ret);
		}*/
	} else {
		test = 0;
		data[0] = 0xFF;
		//GPIO_WritePin(GPIO_I2cPinInit[0],GPIO_PIN_RESET);
		HAL_I2C_Init(hi2c);
		while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
		HAL_I2C_Master_Transmit(hi2c,dev_addr,data,1,0xFFF);
	}

	//HAL_Delay(1000);
}
