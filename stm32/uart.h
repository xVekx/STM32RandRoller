#ifndef __UART_H
#define __UART_H
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
	UART_HandleTypeDef		huart;
	const GPIO_Device*		pins;
	const int				pins_size;
} UART_Device;
//------------------------------------------------------------------------------
void UART_InitDev(UART_Device* dev);
void UART_DeInitDev(UART_Device* dev);
//------------------------------------------------------------------------------
void UART_TestInit();
void UART_TestLoop();
//------------------------------------------------------------------------------
#endif
