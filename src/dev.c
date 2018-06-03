#include "dev.h"
//------------------------------------------------------------------------------
void DEVICE_Init(Struct_Device *dev)
{
	dev->max2719->intens		= MAX2719_Intensity_11_32;
	dev->max2719->scanlimit		= MAX2719_ScanLimit_01234567;
	dev->max2719->digitbegin	= MAX2719_Addr_DIG_0;
	dev->max2719->digitend		= MAX2719_Addr_DIG_3;

	MAX2719_InitDev(dev->max2719);

	BUTTOM_InitDev(dev->buttom);

	HD44780_I2C_InitDev(dev->hd44780_i2c);

	RTC_InitDev(dev->rtc);
	RTC_InitDevSecondIT(dev->rtc);

	TIM_Base_InitDev(dev->tim3_base);
	TIM_Base_Start_IT(dev->tim3_base);
}
//------------------------------------------------------------------------------
void DEVICE_Loop(Struct_Device *dev)
{
	if(dev->flag_update_1hz) {
		PrintfDataTime(&dev->rtc->hrtc);
	}
}
//------------------------------------------------------------------------------
