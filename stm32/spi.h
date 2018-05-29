#ifndef __SPI_H
#define __SPI_H
//------------------------------------------------------------------------------
#if defined ( BOARD_STM32F103C8T6 )
#include "stm32f1xx_hal.h"
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
	SPI_HandleTypeDef		hspi;
	GPIO_InitDevTypeDef*	pins;
	const int				pins_size;
} SPI_InitDevTypeDev;
//------------------------------------------------------------------------------
void SPI_InitDev(SPI_InitDevTypeDev* dev);
void SPI_DeInitDev(SPI_InitDevTypeDev* dev);
//------------------------------------------------------------------------------
void SPI_TestInit();
void SPI_TestLoop();
//------------------------------------------------------------------------------
#endif
