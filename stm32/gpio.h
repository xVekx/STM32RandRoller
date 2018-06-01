#ifndef __GPIO_H
#define __GPIO_H
//------------------------------------------------------------------------------
#if defined ( BOARD_STM32F103C8T6 )
#include "stm32f1xx_hal.h"
//------------------------------------------------------------------------------
typedef enum {
	TLED_PC13	= 0,
	TALL,
} Led_TypeDef;
//------------------------------------------------------------------------------
#elif defined ( BOARD_STM32F429I_DISCO )
#include "stm32f4xx_hal.h"
//------------------------------------------------------------------------------
typedef enum {
	TLED_3		= 0,
	TLED_4		= 1,
	TPin0		= 2,
	TALL,
} Led_TypeDef;
//------------------------------------------------------------------------------
#elif defined ( BOARD_STM32F746G_DISCO )
#include "stm32f7xx_hal.h"
//------------------------------------------------------------------------------
typedef enum {
	TLED_GREEN = 0,
	TALL,
} Led_TypeDef;
//------------------------------------------------------------------------------
#else
	#warning "no select board"
#endif
//------------------------------------------------------------------------------
typedef struct {
	GPIO_InitTypeDef	gpio_inittypedef;
	GPIO_TypeDef		*gpio_typedef;
} GPIO_Device;
//------------------------------------------------------------------------------
void GPIO_InitPin(const GPIO_Device *pins,int size);
void GPIO_DeInitPin(const GPIO_Device *pins,int size);
void GPIO_WritePin(GPIO_Device pin ,GPIO_PinState state);
GPIO_PinState GPIO_ReadPin(GPIO_Device pin);
void GPIO_Test_Init(Led_TypeDef Led);
void GPIO_Test_On(Led_TypeDef Led);
void GPIO_Test_Off(Led_TypeDef Led);
void GPIO_Test_Toggle(Led_TypeDef Led);
//------------------------------------------------------------------------------
#endif
