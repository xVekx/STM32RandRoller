//------------------------------------------------------------------------------
//#include "gpio.h"
//#include "def.h"
//#include "string.h"

#include "uart.h"
#include "def.h"



//------------------------------------------------------------------------------
//extern void Error_Handler(void);
//------------------------------------------------------------------------------
//static USART_HandleTypeDef huart1;
//------------------------------------------------------------------------------
/*static const GPIO_InitDevTypeDef UART_DEBUG_InitPins[] =
{
#if defined(BOARD_KFK3_STM32_PROTO) || \
	defined(BOARD_KFK3_STM32_EXP01)
	{	//LCD DATA PIN
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_AF_PP,
			.Speed		= GPIO_SPEED_FREQ_VERY_HIGH,
			.Pull		= GPIO_PULLUP,
			.Alternate	= GPIO_AF7_USART3,
			.Pin		= GPIO_PIN_8 | GPIO_PIN_9,
		},
		.gpio_typedef	= GPIOD,
	}
#elif defined(BOARD_KFK3_STM32_EXP02)
	{	//LCD DATA PIN
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_AF_PP,
			.Speed		= GPIO_SPEED_FREQ_VERY_HIGH,
			.Pull		= GPIO_PULLUP,
			.Alternate	= GPIO_AF7_USART1,
			.Pin		= GPIO_PIN_6,
		},
		.gpio_typedef	= GPIOB,
	},{	//LCD DATA PIN
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_AF_PP,
			.Speed		= GPIO_SPEED_FREQ_VERY_HIGH,
			.Pull		= GPIO_PULLUP,
			.Alternate	= GPIO_AF7_USART1,
			.Pin		= GPIO_PIN_10,
		},
		.gpio_typedef	= GPIOA,
	}
#else
	{	//LCD DATA PIN
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_AF_PP,
			.Speed		= GPIO_SPEED_FREQ_VERY_HIGH,
			.Pull		= GPIO_PULLUP,
			.Alternate	= GPIO_AF7_USART1,
			.Pin		= GPIO_PIN_9 | GPIO_PIN_10,
		},
		.gpio_typedef	= GPIOA,
	}
#endif
};*/
//------------------------------------------------------------------------------
/*static const GPIO_InitPinTypeDef UART_OUT_InitPins[] = {
	{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_OUTPUT_PP,
			.Speed		= GPIO_SPEED_FAST,
			.Pull		= GPIO_PULLUP,
			.Pin		= GPIO_PIN_12,
		},
		.gpio_typedef	= GPIOB,
	}
};
//------------------------------------------------------------------------------
void HAL_USART_MspInit(USART_HandleTypeDef* huart)
{

	if(huart->Instance == USART3) {
		__HAL_RCC_USART3_CLK_ENABLE();
	}

	if(huart->Instance == USART1) {
		__HAL_RCC_USART1_CLK_ENABLE();

		printf("USART1 Init pins\n");

		//HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
		//HAL_NVIC_EnableIRQ(USART3_IRQn);
		//__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
	}
	GPIO_InitPin(UART_DEBUG_InitPins,ARRAY_SIZE(UART_DEBUG_InitPins));
}
//------------------------------------------------------------------------------
void HAL_USART_MspDeInit(USART_HandleTypeDef* huart)
{
	if(huart->Instance==USART3) {
		__HAL_RCC_USART3_CLK_DISABLE();
	}

	if(huart->Instance==USART1) {
		__HAL_RCC_USART1_CLK_DISABLE();
	}

	GPIO_DeInitPin(UART_DEBUG_InitPins,ARRAY_SIZE(UART_DEBUG_InitPins));
}
//------------------------------------------------------------------------------
static const USART_InitTypeDef UART1_InitConfig = {
	.BaudRate		= 115200,
	.WordLength		= USART_WORDLENGTH_8B,
	.StopBits		= USART_STOPBITS_1,
	.Parity			= USART_PARITY_NONE,
	.Mode			= USART_MODE_TX_RX,
	.CLKPolarity	= USART_POLARITY_LOW,
	.CLKPhase		= USART_PHASE_1EDGE,
	.CLKLastBit		= USART_LASTBIT_DISABLE,
};
//------------------------------------------------------------------------------
void UART1_Init(void)
{
#if defined(BOARD_KFK3_STM32_PROTO) || \
	defined(BOARD_KFK3_STM32_EXP01)
	huart1.Instance = USART3;
#else
	huart1.Instance = USART1;
#endif
	huart1.Init = UART1_InitConfig;
	if(HAL_USART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	} else {
		printf("=========================================\n");
		printf("UART1 Init\n");
	}
}
//------------------------------------------------------------------------------
USART_HandleTypeDef* UART1_Get()
{
	return &huart1;
}
//------------------------------------------------------------------------------
int _write (int fd, char *ptr, int len)
{
	UNUSED(fd);
	for(int i=0;i<len;i++) {
		HAL_USART_Transmit(&huart1,(uint8_t*)&ptr[i],1,0xFFFF);
		if(ptr[i] == '\n') {
			HAL_USART_Transmit(&huart1,(uint8_t*)"\r",1,0xFFFF);
		}
	}
	return len;
}
//------------------------------------------------------------------------------
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	printf("HAL_UART_ErrorCallback\n");
}
//------------------------------------------------------------------------------
#define BUFF_SIZE_RX 64 //размер буфера приема
#define BUFF_SIZE_TX 64 //размер буфера передачи
//------------------------------------------------------------------------------
static char buff_rx [BUFF_SIZE_RX];
//static char buff_tx [BUFF_SIZE_TX];
//------------------------------------------------------------------------------
static volatile uint8_t count_rx = 0;
static volatile uint8_t count_tx = 0;
//------------------------------------------------------------------------------
static volatile uint8_t flag_rx = 0;
static volatile uint8_t flag_tx = 0;
//------------------------------------------------------------------------------
#define UART_ECHO		0
#define UART_READY		1
#define UART_SEND_PRINT	2
//------------------------------------------------------------------------------
void UartSetFlagRxEcho(uint8_t f)
{
	f ? SBIT(flag_rx,UART_ECHO) : CBIT(flag_rx,UART_ECHO);
}
//------------------------------------------------------------------------------
void UartSetFlagSendPrint(uint8_t f)
{
	f ? SBIT(flag_rx,UART_SEND_PRINT) : CBIT(flag_rx,UART_SEND_PRINT);
}
//------------------------------------------------------------------------------
uint8_t UartCheckFlagSendPrint(void)
{
	return TBIT1(flag_rx,UART_SEND_PRINT);
}
//------------------------------------------------------------------------------
uint8_t UartRxCheckReady(void)
{
	return TBIT1(flag_rx,UART_READY);
}
//------------------------------------------------------------------------------
char *UartRxData(void)
{
	return buff_rx;
}
//------------------------------------------------------------------------------
uint8_t UartRxCount(void)
{
	return count_rx;
}
//------------------------------------------------------------------------------
void UartDataClean(void)
{
	count_rx = 0;
}
//------------------------------------------------------------------------------
void UART_Transmit(USART_TypeDef *itd, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	if(itd == USART1)
		HAL_USART_Transmit(&huart1,pData,Size,Timeout);
}
//------------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
	UartSetFlagRxEcho(1);
	char crx = 0;
	if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET))
	{
		crx = (char)(huart1.Instance->DR & (uint8_t)0x00FF);
		//printf("%x\n",crx);
	}
	__HAL_UART_CLEAR_PEFLAG(&huart1);

	char btx[10];
	memset(btx,0,10);

	printf("1\n");

	if(TBIT0(flag_rx,UART_READY)) {
		if(TBIT1(flag_rx,UART_ECHO))
			sprintf(btx,"%c",crx);
		if(crx == '\n' || crx == '\r') {
			buff_rx[count_rx] = '\0';
			SBIT(flag_rx,UART_READY);
			if(count_rx != 0 && TBIT1(flag_rx,UART_ECHO)) {
				sprintf(btx,"%s\n",btx);
			}
		} else {
			buff_rx[count_rx] = crx;
			if(count_rx < BUFF_SIZE_RX)
				count_rx++;
			else
				SBIT(flag_rx,UART_READY);
		}
	} else {
		count_rx = 0;
		CBIT(flag_rx,UART_READY);
	}
	//PrintHexStr(btx);
	HAL_USART_Transmit(&huart1,(uint8_t*)&btx,strlen(btx),0x0001);
}*/



void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

#if defined(USART1)
	if(huart->Instance==USART1) {
		__HAL_RCC_USART1_CLK_ENABLE();
	}
#endif

#if defined(USART2)
	if(huart->Instance==USART2) {
		__HAL_RCC_USART2_CLK_ENABLE();
	}
#endif

#if defined(USART3)
	if(huart->Instance==USART3) {
		__HAL_RCC_USART3_CLK_ENABLE();
	}
#endif

#if defined(USART4)
	if(huart->Instance==USART4) {
		__HAL_RCC_USART4_CLK_ENABLE();
	}
#endif

}
/*
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
*/
void UART_InitDev(UART_InitDevTypeDev *uart)
{
	GPIO_InitPin(uart->pins,uart->pins_size);
	if (HAL_UART_Init(&uart->huart) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
}

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

/*static UART_InitTypeDef UART_Test = {
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
}*/



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
