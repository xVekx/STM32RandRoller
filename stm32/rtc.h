#ifndef __RTC_H
#define __RTC_H
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
	RTC_HandleTypeDef hrtc;
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	void (*rtc_event_callback)(RTC_HandleTypeDef *hrtc);
} RTC_Device;
//------------------------------------------------------------------------------
void RTC_InitDev(RTC_Device *dev);
void RTC_DeInitDev(RTC_Device *dev);
//------------------------------------------------------------------------------
void RTC_InitDevSecondIT(RTC_Device *dev);
void RTC_DeInitDevSecondIT(RTC_Device *dev);
//------------------------------------------------------------------------------
void PrintfDataTime(RTC_HandleTypeDef *hrtc);
time_t RTC_Read(RTC_HandleTypeDef *hrtc);
void RTC_Write(RTC_HandleTypeDef *hrtc,time_t t);
//------------------------------------------------------------------------------
void RTC_TestInit();
void RTC_TestLoop();
//------------------------------------------------------------------------------
#endif
