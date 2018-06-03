#include "rtc.h"
#include "def.h"

#include <time.h>
//------------------------------------------------------------------------------
RTC_Device *rtc_ptr = NULL;
//------------------------------------------------------------------------------
void RTC_IRQHandler(void)
{
	if(rtc_ptr) {
		HAL_RTCEx_RTCIRQHandler(&rtc_ptr->hrtc);
	}
}
//------------------------------------------------------------------------------
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(rtc_ptr && rtc_ptr->rtc_event_callback) {
		rtc_ptr->rtc_event_callback(hrtc);
	}
}
//------------------------------------------------------------------------------
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_BKP_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	/* Enable BKP CLK enable for backup registers */
	//
	/* Peripheral clock enable */
	__HAL_RCC_RTC_ENABLE();
}
//------------------------------------------------------------------------------
void RTC_InitDev(RTC_Device *dev)
{
	//HAL_RTCEx_EnableBypassShadow(&dev->hrtc);
	//__HAL_RCC_RTC_ENABLE();
	RTC_DeInitDev(dev);

	HAL_RTC_WaitForSynchro(&dev->hrtc);

	if(HAL_RTC_Init(&dev->hrtc) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	if(HAL_RTCEx_BKUPRead(&dev->hrtc, RTC_BKP_DR1) != 0x32F2) {
		if( HAL_RTC_SetTime(&dev->hrtc, &dev->time, RTC_FORMAT_BCD) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		if(HAL_RTC_SetDate(&dev->hrtc, &dev->date, RTC_FORMAT_BCD) != HAL_OK) {
			_Error_Handler(__FILE__, __LINE__);
		}

		HAL_RTCEx_BKUPWrite(&dev->hrtc,RTC_BKP_DR1,0x32F2);
	}

	rtc_ptr = dev;
	printf("RTC_InitDev\n");
}
//------------------------------------------------------------------------------
void RTC_DeInitDev(RTC_Device *dev)
{
	if(HAL_RTC_DeInit(&dev->hrtc) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	__HAL_RCC_RTC_DISABLE();
	rtc_ptr = NULL;
}
//------------------------------------------------------------------------------
void RTC_InitDevSecondIT(RTC_Device *dev)
{
	//RTC_InitDev(dev);
	HAL_RTCEx_SetSecond_IT(&dev->hrtc);
	// HAL_RTC_WaitForSynchro(&hrtc);
	NVIC_EnableIRQ(RTC_IRQn);
}
//------------------------------------------------------------------------------
void RTC_DeInitDevSecondIT(RTC_Device *dev)
{
	NVIC_DisableIRQ(RTC_IRQn);
	HAL_RTCEx_DeactivateSecond(&dev->hrtc);
	//RTC_DeInitDev(dev);
}
//------------------------------------------------------------------------------
void PrintfDataTime(RTC_HandleTypeDef *hrtc)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);
	printf("Time %.2d:%.2d:%.2d\n",sTime.Hours,sTime.Minutes,sTime.Seconds);
}
//------------------------------------------------------------------------------
time_t RTC_Read(RTC_HandleTypeDef *hrtc)
{
	RTC_DateTypeDef data;
	RTC_TimeTypeDef time;
	struct tm timeinfo;

	// Read actual date and time
	// Warning: the time must be read first!
	HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc, &data, RTC_FORMAT_BIN);

	// Setup a tm structure based on the RTC
	timeinfo.tm_wday	= data.WeekDay;
	timeinfo.tm_mon		= data.Month - 1;
	timeinfo.tm_mday	= data.Date;
	timeinfo.tm_year	= data.Year + 100;
	timeinfo.tm_hour	= time.Hours;
	timeinfo.tm_min		= time.Minutes;
	timeinfo.tm_sec		= time.Seconds;

	// Convert to timestamp
	time_t t = mktime(&timeinfo);
	return t;
}
//------------------------------------------------------------------------------
void RTC_Write(RTC_HandleTypeDef *hrtc,time_t t)
{
	RTC_DateTypeDef data;
	RTC_TimeTypeDef time;

	// Convert the time into a tm
	struct tm timeinfo = *localtime(&t);

	// Fill RTC structures
	data.WeekDay	= timeinfo.tm_wday;
	data.Month		= timeinfo.tm_mon + 1;
	data.Date		= timeinfo.tm_mday;
	data.Year		= timeinfo.tm_year - 100;
	time.Hours		= timeinfo.tm_hour;
	time.Minutes	= timeinfo.tm_min;
	time.Seconds	= timeinfo.tm_sec;
	//timeStruct.TimeFormat		= RTC_HOURFORMAT12_PM;
	//timeStruct.DayLightSaving	= RTC_DAYLIGHTSAVING_NONE;
	//timeStruct.StoreOperation	= RTC_STOREOPERATION_RESET;

	// Change the RTC current date/time
	HAL_RTC_SetDate(&hrtc, &data, RTC_FORMAT_BIN);
	HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
}

#if 1

#include "gpio.h"
static volatile int zzzz = 0;
//------------------------------------------------------------------------------
static void rtc_event_callback_test(RTC_HandleTypeDef *hrtc)
{

	zzzz = 1;
	PrintfDataTime(hrtc);

}
//------------------------------------------------------------------------------
RTC_Device rtc_init = {
	.hrtc = {
		.Instance = RTC,
		.Init = {
			.AsynchPrediv = RTC_AUTO_1_SECOND,
			.OutPut = RTC_OUTPUTSOURCE_NONE,
		},
	},
	.time = {
		.Hours = 0x0,
		.Minutes = 0x0,
		.Seconds = 0x0,
	},
	.date = {
		.WeekDay = RTC_WEEKDAY_MONDAY,
		.Month = RTC_MONTH_JANUARY,
		.Date = 0x1,
		.Year = 0x0,
	},
	.rtc_event_callback = &rtc_event_callback_test,
};
//------------------------------------------------------------------------------
void RTC_TestInit()
{
	RTC_InitDev(&rtc_init);
	RTC_InitDevSecondIT(&rtc_init);
}
//------------------------------------------------------------------------------
void RTC_TestLoop()
{
	if(zzzz == 1) {
			GPIO_Test_Toggle(TLED_PC13);
			int a = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_RTC);
			printf("%s %i\n",__func__,a);
			zzzz = 0;
	}
}

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
