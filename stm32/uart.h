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

#include "gpio.h"

/*
typedef struct {
	GPIO_InitTypeDef	gpio_inittypedef;
	GPIO_TypeDef		*gpio_typedef;
} GPIO_InitPinTypeDef;
*/

/*static const USART_InitTypeDef UART1_InitConfig = {
	.BaudRate		= 115200,
	.WordLength		= USART_WORDLENGTH_8B,
	.StopBits		= USART_STOPBITS_1,
	.Parity			= USART_PARITY_NONE,
	.Mode			= USART_MODE_TX_RX,
	.CLKPolarity	= USART_POLARITY_LOW,
	.CLKPhase		= USART_PHASE_1EDGE,
	.CLKLastBit		= USART_LASTBIT_DISABLE,
};*/
/*
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
	_Error_Handler(__FILE__, __LINE__);
  }
*/
typedef struct {
	UART_HandleTypeDef		huart;
	GPIO_Device*			pins;
	const int				pins_size;
} UART_InitDevTypeDev;

void UART_InitDev(UART_InitDevTypeDev* uart);


void UART_TestInit();
void UART_TestLoop();

//void UART1_Init(void);
//USART_HandleTypeDef *UART1_Get();
//------------------------------------------------------------------------------
#endif
