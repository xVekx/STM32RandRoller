#ifndef __BUTTOM_H
#define __BUTTOM_H
//------------------------------------------------------------------------------
#include "gpio.h"
//------------------------------------------------------------------------------
typedef union {
	struct {
		uint8_t StateNew:1;		//Кнопка 0 - Down(Нажата), 1 - Up(Отжата)
		uint8_t StateOld:1;		//
		uint8_t TrigUpToDown:1;	//Тригер по Нажатию
		uint8_t TrigDownToUp:1;	//Тригер по Отжатию
		uint8_t Reserved:4;
		uint8_t Count;
	} b;
	uint16_t w;
} BUTTOM_State;
//------------------------------------------------------------------------------
typedef struct {
	BUTTOM_State*			state;
	GPIO_Device*	pins;
	const uint8_t			pins_size;
	uint8_t					ready;
	uint8_t					count_ready;
} BUTTOM_Device;
//------------------------------------------------------------------------------
#define GET_BUTTOM_TRIG_UP_TO_DOWN(_dev,_i) ( (_dev)->state[(_i)].b.TrigUpToDown )
#define GET_BUTTOM_TRIG_DOWN_TO_UP(_dev,_i) ( (_dev)->state[(_i)].b.TrigDownToUp )
#define GET_BUTTOM_IS_READY(_dev)			( (_dev)->ready )
#define GET_BUTTOM_SIZE(_dev)				( (_dev)->pins_size )
//------------------------------------------------------------------------------
void BUTTOM_InitDev(BUTTOM_Device *dev);
void BUTTOM_DeInitDev(BUTTOM_Device *dev);
void BUTTOM_TestInit();
void BUTTOM_TestLoop();
//------------------------------------------------------------------------------
#endif
