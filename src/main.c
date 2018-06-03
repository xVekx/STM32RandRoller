//------------------------------------------------------------------------------
#define USE_FULL_ASSERT    1U
//------------------------------------------------------------------------------
#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>
//#include <inttypes.h>
//#include <errno.h>
//#include <sys/stat.h>
//#include <sys/times.h>
//#include <sys/unistd.h>
//------------------------------------------------------------------------------
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
//------------------------------------------------------------------------------
#include "clock.h"
#include "gpio.h"
#include "def.h"
#include "uart.h"
#include "debug_print.h"
/*#include "max2719.h"
#include "buttom.h"
#include "hd44780_i2c.h"
#include "rtc.h"
#include "timer.h"*/

#include "dev.h"

//------------------------------------------------------------------------------
void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
	HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
	HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

	HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);

	/**DISABLE: JTAG-DP Disabled and SW-DP Disabled */
	//__HAL_RCC_AFIO_CLK_ENABLE();
	//__HAL_AFIO_REMAP_SWJ_DISABLE();
}
//------------------------------------------------------------------------------
int main(void)
{
	HAL_Init();
	SystemClock_Config();
	GPIO_Test_Init(TLED_PC13);
	DEBUG_Init();

	DEVICE_Init(&RandRollDevice);

	while (1) {
		DEVICE_Loop(&RandRollDevice);
	}
}
////////////////////////////////////////////////////////////////////////////////
///*******************************Прерывания**********************************//
////////////////////////////////////////////////////////////////////////////////
void NMI_Handler(void)
{

}
//------------------------------------------------------------------------------
void HardFault_Handler(void)
{
	while (1) {}
}
//------------------------------------------------------------------------------
void MemManage_Handler(void)
{
	while (1) {}
}
//------------------------------------------------------------------------------
void BusFault_Handler(void)
{
	while (1) {}
}
//------------------------------------------------------------------------------
void UsageFault_Handler(void)
{
	while (1) {}
}
//------------------------------------------------------------------------------
void SVC_Handler(void)
{

}
//------------------------------------------------------------------------------
void DebugMon_Handler(void)
{

}
//------------------------------------------------------------------------------
void PendSV_Handler(void)
{

}
//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
//------------------------------------------------------------------------------
extern PCD_HandleTypeDef hpcd_USB_FS;
//------------------------------------------------------------------------------
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	//HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
//------------------------------------------------------------------------------
void _Error_Handler(char *file, int line)
{
	fprintf(stderr,"Error_Handler : file %s on line %d\n", file, line);
	while(1) { }
}
//------------------------------------------------------------------------------
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	fprintf(stderr,"Wrong parameters value: file %s on line %d\n", file, line);
}
#endif /* USE_FULL_ASSERT */
