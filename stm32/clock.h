#ifndef __CLOCK_H
#define __CLOCK_H
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
typedef struct
{
	RCC_ClkInitTypeDef clk;//RCC_OscInitTypeDef
	RCC_OscInitTypeDef osc;
	RCC_PeriphCLKInitTypeDef periphclk;
} RCC_ClkTypeDef;
//------------------------------------------------------------------------------
void SystemClock_Config(void);
//------------------------------------------------------------------------------
#endif
