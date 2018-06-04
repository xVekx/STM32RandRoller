//------------------------------------------------------------------------------
#include "uart.h"
#include "def.h"
//------------------------------------------------------------------------------
#define UART_CLK(_port,_port_sel,_stat_sel)									\
					{if(_port == _port_sel)									\
						if(_stat_sel) 										\
						 __HAL_RCC_##_port##_CLK_ENABLE();					\
						 else												\
						 __HAL_RCC_##_port##_CLK_DISABLE();}
//------------------------------------------------------------------------------
static void UART_Clk(SPI_TypeDef *spi,FunctionalState stat)
{
#if defined(USART1)
	UART_CLK(USART1,spi,stat);
#endif

#if defined(USART2)
	UART_CLK(USART2,spi,stat);
#endif

#if defined(USART3)
	UART_CLK(USART3,spi,stat);
#endif

#if defined(USART4)
	UART_CLK(USART4,spi,stat);
#endif
}
//------------------------------------------------------------------------------
void HAL_UART_MspInit(UART_HandleTypeDef* huart) { }
//------------------------------------------------------------------------------
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart) { }
//------------------------------------------------------------------------------
void UART_InitDev(UART_Device *dev)
{
	UART_Clk(dev->huart.Instance,ENABLE);
	GPIO_InitPin(dev->pins,dev->pins_size);
	if (HAL_UART_Init(&dev->huart) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
}
//------------------------------------------------------------------------------
void UART_DeInitDev(UART_Device *dev)
{
	if (HAL_UART_DeInit(&dev->huart) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	GPIO_DeInitPin(dev->pins,dev->pins_size);
	UART_Clk(dev->huart.Instance,ENABLE);
}
//------------------------------------------------------------------------------
#if 0
static const GPIO_Device GPIO_Test[] = {
	{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_AF_PP,
			.Speed		= GPIO_SPEED_FREQ_HIGH,
			.Pull		= GPIO_NOPULL,
			.Pin		= GPIO_PIN_9,
		},
		.gpio_typedef	= GPIOA,
	},{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_INPUT,
			.Speed		= GPIO_SPEED_FREQ_HIGH,
			.Pull		= GPIO_NOPULL,
			.Pin		= GPIO_PIN_10,
		},
		.gpio_typedef	= GPIOA,
	}
};

static UART_InitDevTypeDev uarttest = {

	.huart = {
		.Instance = USART1,
		.Init = {
			.BaudRate = 115200,
			.WordLength = UART_WORDLENGTH_8B,
			.StopBits = UART_STOPBITS_1,
			.Parity = UART_PARITY_NONE,
			.Mode = UART_MODE_TX_RX,
			.HwFlowCtl = UART_HWCONTROL_NONE,
			.OverSampling = UART_OVERSAMPLING_16,
		}
	},
	.pins = GPIO_Test,
	.pins_size = ARRAY_SIZE(GPIO_Test),
};


void UART_TestInit()
{
	//UART_InitDev(&uarttest);
}

void UART_TestLoop()
{
	//HAL_UART_Transmit(&uarttest.uart_handletypedef,"123\r\n",5,0xFFF);
}
#endif
