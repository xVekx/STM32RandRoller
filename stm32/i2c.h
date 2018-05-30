#ifndef __I2C_H
#define __I2C_H
//------------------------------------------------------------------------------
#if defined ( BOARD_STM32F103C8T6 )
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
//------------------------------------------------------------------------------
#elif defined ( BOARD_STM32F429I_DISCO )
#include "stm32f4xx_hal.h"
//------------------------------------------------------------------------------
#elif defined ( BOARD_STM32F746G_DISCO )
#include "stm32f7xx_hal.h"
//------------------------------------------------------------------------------
#else
	#warning "no select board"
#endif
//------------------------------------------------------------------------------
#include "gpio.h"
//------------------------------------------------------------------------------
typedef struct {
	I2C_HandleTypeDef		hi2c;
	GPIO_InitDevTypeDef*	pins;
	const int				pins_size;
} I2C_Device;
//------------------------------------------------------------------------------
void I2C_InitDev(I2C_Device* dev);
void I2C_DeInitDev(I2C_Device *dev);
//------------------------------------------------------------------------------
HAL_StatusTypeDef I2C_Write	(	I2C_Device *dev,
								uint16_t addr,
								uint8_t *data,
								uint16_t size );

HAL_StatusTypeDef I2C_Read	(	I2C_Device *dev,
								uint16_t addr,
								uint8_t *data,
								uint16_t size );
//------------------------------------------------------------------------------
void I2C_TestInit();
void I2C_TestLoop();
//------------------------------------------------------------------------------
#endif

