#include "buttom.h"
#include <stdio.h>
#include <string.h>
#include "def.h"
//------------------------------------------------------------------------------
#define KEY_DOWN			0
#define KEY_UP				1
//------------------------------------------------------------------------------
void BUTTOM_InitDev(BUTTOM_Device *dev)
{
	GPIO_InitPin(dev->pins,dev->pins_size);
	dev->state = (BUTTOM_State*)malloc(sizeof(BUTTOM_State) * dev->pins_size);
	for(uint8_t i=0; i<dev->pins_size;i++) {
		dev->state[i].w = 0;
		dev->state[i].b.StateOld = KEY_UP;
		dev->state[i].b.StateNew = KEY_UP;
	}
	dev->ready = 0;
}
//------------------------------------------------------------------------------
void BUTTOM_DeInitDev(BUTTOM_Device *dev)
{
	free(dev->state);
	GPIO_DeInitPin(dev->pins,dev->pins_size);
}
//------------------------------------------------------------------------------
void BUTTOM_Scan(BUTTOM_Device *dev)
{
	uint8_t i;
	for( i = 0;i<dev->pins_size;i++) {
		GPIO_PinState State = GPIO_ReadPin(dev->pins[i]);
		if(State == GPIO_PIN_SET) {
			dev->state[i].b.StateNew = KEY_UP;
		} else {
			dev->state[i].b.StateNew = KEY_DOWN;
		}
	}

	for(i=0;i<dev->pins_size;i++) {
		if(dev->state[i].b.StateNew != dev->state[i].b.StateOld) {
			dev->state[i].b.Count++;
		} else {
			dev->state[i].b.Count = 0;
		}
	}

	for(i=0;i<dev->pins_size;i++) {
		if(dev->state[i].b.Count >= dev->count_ready) {
			if(	( dev->state[i].b.StateOld == KEY_UP ) &&
				( dev->state[i].b.StateNew == KEY_DOWN ) ) {
				dev->state[i].b.TrigUpToDown = 1;
				dev->ready = 1;
			}
			if(	( dev->state[i].b.StateOld == KEY_DOWN ) &&
				( dev->state[i].b.StateNew == KEY_UP ) ) {
				dev->state[i].b.TrigDownToUp = 1;
				dev->ready = 1;
			}
			dev->state[i].b.Count = 0;
			dev->state[i].b.StateOld = dev->state[i].b.StateNew;
		}
	}
}
//------------------------------------------------------------------------------
void BUTTOM_Clean(BUTTOM_Device *dev)
{
	for(uint8_t i=0; i<dev->pins_size; i++) {
		if(dev->state[i].b.TrigDownToUp) {
			dev->state[i].b.TrigDownToUp = 0;
		}
		if(dev->state[i].b.TrigUpToDown) {
			dev->state[i].b.TrigUpToDown = 0;
		}
	}
	dev->ready = 0;
}
//------------------------------------------------------------------------------
#if 0
static const GPIO_Device GPIO_ButtomPinInit[] = {
	{
		.gpio_init = {
			.Mode		=	GPIO_MODE_INPUT,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_PULLDOWN,
			.Pin		=	GPIO_PIN_9,
		},
		.gpio_type	=	GPIOB,
	},{
		.gpio_init = {
			.Mode		=	GPIO_MODE_INPUT,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_PULLDOWN,
			.Pin		=	GPIO_PIN_8,
		},
		.gpio_type	=	GPIOB,
	}
};
//------------------------------------------------------------------------------
static BUTTOM_Device	buttom = {
	.pins = GPIO_ButtomPinInit,
	.pins_size = ARRAY_SIZE(GPIO_ButtomPinInit),
	.count_ready = 20,
};
//------------------------------------------------------------------------------
void BUTTOM_TestInit()
{
	BUTTOM_InitDev(&buttom);
}
//------------------------------------------------------------------------------
void BUTTOM_TestLoop()
{
	BUTTOM_Device *dev = &buttom;
	BUTTOM_Scan(dev);
	if(GET_BUTTOM_IS_TRIG(dev)) {
		for(int i = 0; i < GET_BUTTOM_SIZE(dev); i++) {
			if(GET_BUTTOM_TRIG_UP_TO_DOWN(dev,i)) {
				printf("BUTTOM TrigUpToDown buttom %d\n",i);
			}
			if(GET_BUTTOM_TRIG_DOWN_TO_UP(dev,i)) {
				printf("BUTTOM TrigDownToUp buttom %d\n",i);
			}
		}
		BUTTOM_Clean(dev);
	}
}
//------------------------------------------------------------------------------
#endif
