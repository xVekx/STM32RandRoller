#ifndef __TIMER_H
#define __TIMER_H
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
typedef struct {
	TIM_HandleTypeDef htim;
	void (*period_elapsed_callback)(TIM_HandleTypeDef*);
} TIM_Base_Device;
//------------------------------------------------------------------------------
void TIM_Base_InitDev(TIM_Base_Device *dev);
void TIM_Base_DeInitDev(TIM_Base_Device *dev);
//------------------------------------------------------------------------------
void TIM_Base_Start_IT(TIM_Base_Device *dev);
void TIM_Base_Stop_IT(TIM_Base_Device *dev);
//------------------------------------------------------------------------------
void TIM_TestInit();
//------------------------------------------------------------------------------
#endif
