#include "gpio.h"
//------------------------------------------------------------------------------
#if !defined  (ARRAY_SIZE)
	#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif
//------------------------------------------------------------------------------
#define PORT_CLK(_port,_port_sel,_stat_sel)									\
					{if(_port == _port_sel)									\
						if(_stat_sel) 										\
						 __HAL_RCC_##_port##_CLK_ENABLE();					\
						 else												\
						 __HAL_RCC_##_port##_CLK_DISABLE();}
//------------------------------------------------------------------------------
static void GPIO_Clk(GPIO_TypeDef *gpio_port,FunctionalState stat)
{

#if defined(GPIOA)
	PORT_CLK(GPIOA,gpio_port,stat);
#endif

#if defined(GPIOB)
	PORT_CLK(GPIOB,gpio_port,stat);
#endif

#if defined(GPIOC)
	PORT_CLK(GPIOC,gpio_port,stat);
#endif

#if defined(GPIOD)
	PORT_CLK(GPIOD,gpio_port,stat);
#endif

#if defined(GPIOE)
	PORT_CLK(GPIOE,gpio_port,stat);
#endif

#if defined(GPIOF)
	PORT_CLK(GPIOF,gpio_port,stat);
#endif

#if defined(GPIOG)
	PORT_CLK(GPIOG,gpio_port,stat);
#endif

#if defined(GPIOH)
	PORT_CLK(GPIOH,gpio_port,stat);
#endif

#if defined(GPIOI)
	PORT_CLK(GPIOI,gpio_port,stat);
#endif

#if defined(GPIOJ)
	PORT_CLK(GPIOJ,gpio_port,stat);
#endif

#if defined(GPIOK)
	PORT_CLK(GPIOK,gpio_port,stat);
#endif
}
//------------------------------------------------------------------------------
void GPIO_InitPin(const GPIO_InitDevTypeDef *pins,int size)
{
	if(!pins) return;
	for(int i=0;i<size;i++) {
		GPIO_TypeDef *gpio_port = pins[i].gpio_typedef;
		GPIO_InitTypeDef gpio_pin = pins[i].gpio_inittypedef;
		GPIO_Clk(gpio_port,ENABLE);
		HAL_GPIO_Init(gpio_port,&gpio_pin);
	}
}
//------------------------------------------------------------------------------
void GPIO_DeInitPin(const GPIO_InitDevTypeDef *pins,int size)
{
	for(int i=0;i<size;i++) {
		HAL_GPIO_DeInit(pins[i].gpio_typedef,
						pins[i].gpio_inittypedef.Pin);
		GPIO_Clk(pins[i].gpio_typedef,DISABLE);
	}
}
//------------------------------------------------------------------------------
void GPIO_WritePin(GPIO_InitDevTypeDef pin ,GPIO_PinState state)
{
	HAL_GPIO_WritePin(	pin.gpio_typedef,
						(uint16_t)pin.gpio_inittypedef.Pin,
						state);
}
//------------------------------------------------------------------------------
GPIO_PinState GPIO_ReadPin(GPIO_InitDevTypeDef pin)
{
	return  HAL_GPIO_ReadPin(	pin.gpio_typedef,
								(uint16_t)pin.gpio_inittypedef.Pin);
}
//------------------------------------------------------------------------------
#if defined ( BOARD_STM32F103C8T6 )
//------------------------------------------------------------------------------
static const GPIO_InitDevTypeDef GPIO_LED_InitPins[] = {
	{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_OUTPUT_PP,
			.Speed		= GPIO_SPEED_FREQ_MEDIUM,
			.Pull		= GPIO_PULLUP,
			.Pin		= GPIO_PIN_13,
		},
		.gpio_typedef	= GPIOC,
	}
};
//------------------------------------------------------------------------------
#elif defined ( BOARD_STM32F429I_DISCO )
//------------------------------------------------------------------------------
static const GPIO_InitDevTypeDef GPIO_LED_InitPins[] = {
	{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_OUTPUT_PP,
			.Speed		= GPIO_SPEED_FAST,
			.Pull		= GPIO_PULLUP,
			.Pin		= GPIO_PIN_13,
		},
		.gpio_typedef	= GPIOG,
	},{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_OUTPUT_PP,
			.Speed		= GPIO_SPEED_FAST,
			.Pull		= GPIO_PULLUP,
			.Pin		= GPIO_PIN_14,
		},
		.gpio_typedef	= GPIOG,
	},{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_OUTPUT_PP,
			.Speed		= GPIO_SPEED_FAST,
			.Pull		= GPIO_PULLUP,
			.Pin		= GPIO_PIN_8,
		},
		.gpio_typedef	= GPIOC,
	}
};
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
void GPIO_Test_Init(Led_TypeDef Led)
{
	if(Led == TALL) {
		uint32_t size = ARRAY_SIZE(GPIO_LED_InitPins);
		GPIO_InitPin(GPIO_LED_InitPins,(int)size);
		for(uint32_t i = 0; i<size;i++) {
				HAL_GPIO_WritePin(	GPIO_LED_InitPins[i].gpio_typedef,
									(uint16_t)GPIO_LED_InitPins[i].gpio_inittypedef.Pin,
									GPIO_PIN_RESET);
		}
	} else {
		GPIO_InitPin(&GPIO_LED_InitPins[Led],1);
		HAL_GPIO_WritePin(	GPIO_LED_InitPins[Led].gpio_typedef,
							(uint16_t)GPIO_LED_InitPins[Led].gpio_inittypedef.Pin,
							GPIO_PIN_RESET);
	}
}
//------------------------------------------------------------------------------
void GPIO_Test_On(Led_TypeDef Led)
{
	if(Led == TALL) {
		for(uint32_t i = 0; i< ARRAY_SIZE(GPIO_LED_InitPins);i++) {
			HAL_GPIO_WritePin(	GPIO_LED_InitPins[i].gpio_typedef,
								(uint16_t)GPIO_LED_InitPins[i].gpio_inittypedef.Pin,
								GPIO_PIN_SET);
		}
	} else {
		HAL_GPIO_WritePin(	GPIO_LED_InitPins[Led].gpio_typedef,
							(uint16_t)GPIO_LED_InitPins[Led].gpio_inittypedef.Pin,
							GPIO_PIN_SET);
	}
}
//------------------------------------------------------------------------------
void GPIO_Test_Off(Led_TypeDef Led)
{
	if(Led == TALL) {
		for(uint32_t i = 0; i< ARRAY_SIZE(GPIO_LED_InitPins);i++) {
			HAL_GPIO_WritePin(	GPIO_LED_InitPins[i].gpio_typedef,
								(uint16_t)GPIO_LED_InitPins[i].gpio_inittypedef.Pin,
								GPIO_PIN_RESET);
		}
	} else {
		HAL_GPIO_WritePin(	GPIO_LED_InitPins[Led].gpio_typedef,
							(uint16_t)GPIO_LED_InitPins[Led].gpio_inittypedef.Pin,
							GPIO_PIN_RESET);
	}
}
//------------------------------------------------------------------------------
