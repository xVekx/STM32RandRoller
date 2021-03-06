#ifndef __DEV_CONF_H
#define __DEV_CONF_H
//------------------------------------------------------------------------------
#include "max2719.h"
#include "buttom.h"
#include "hd44780_i2c.h"
#include "rtc.h"
#include "timer.h"
#include "uart.h"
#include "clock.h"
#include "rand_alg.h"
#include "dev_print.h"
//------------------------------------------------------------------------------
typedef struct {

	MAX2719_DevDevice		*max2719;
	BUTTOM_Device			*buttom;
	HD44780_I2C_Device		*hd44780_i2c;
	RTC_Device				*rtc;
	TIM_Base_Device			*tim3_base;
	UART_Device				*uart_debug;
	RCC_Device				*rcc;
	RandAlg					*ra;
	const GPIO_Device		*led;

	uint8_t					flag_update_1hz;
	uint8_t					flag_update_2hz;
	uint8_t					flag_update_10hz;
	uint8_t					flag_update_100hz;

} Struct_Device;
//------------------------------------------------------------------------------
Struct_Device RandRollDevice;
//------------------------------------------------------------------------------
#endif
