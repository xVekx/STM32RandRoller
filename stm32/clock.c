#include "clock.h"
#include "def.h"
//------------------------------------------------------------------------------
#if defined ( BOARD_STM32F103C8T6 )
//------------------------------------------------------------------------------
static  RCC_ClkTypeDef Clk_Config = {
	.osc = {
		.OscillatorType	=	RCC_OSCILLATORTYPE_LSI	|	RCC_OSCILLATORTYPE_HSE,
		.HSEState =			RCC_HSE_ON,
		.HSEPredivValue =	RCC_HSE_PREDIV_DIV1,
		.HSIState =			RCC_HSI_ON,
		.LSIState =			RCC_LSI_ON,
		.PLL.PLLState =		RCC_PLL_ON,
		.PLL.PLLSource =	RCC_PLLSOURCE_HSE,
		.PLL.PLLMUL =		RCC_PLL_MUL9,
	},
	.clk = {
		.ClockType =		RCC_CLOCKTYPE_HCLK	|	RCC_CLOCKTYPE_SYSCLK	|
							RCC_CLOCKTYPE_PCLK1	|	RCC_CLOCKTYPE_PCLK2,
		.SYSCLKSource =		RCC_SYSCLKSOURCE_PLLCLK,
		.AHBCLKDivider =	RCC_SYSCLK_DIV1,
		.APB1CLKDivider =	RCC_HCLK_DIV2,
		.APB2CLKDivider =	RCC_HCLK_DIV1,
	},
	.periphclk = {
		.PeriphClockSelection =		RCC_PERIPHCLK_RTC	|	RCC_PERIPHCLK_USB,
		.RTCClockSelection =		RCC_RTCCLKSOURCE_LSI,
		.UsbClockSelection =		RCC_USBCLKSOURCE_PLL_DIV1_5,

	},
};
//------------------------------------------------------------------------------
void SystemClock_Config(void)
{
	//__HAL_RCC_AFIO_CLK_ENABLE();

	if (HAL_RCC_OscConfig(&Clk_Config.osc) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_RCC_ClockConfig(&Clk_Config.clk, FLASH_LATENCY_2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_RCCEx_PeriphCLKConfig(&Clk_Config.periphclk) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  //HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
//------------------------------------------------------------------------------
#elif defined ( BOARD_STM32F429I_DISCO )


//------------------------------------------------------------------------------
#elif defined ( BOARD_STM32F746G_DISCO )
//------------------------------------------------------------------------------
static  RCC_ClkTypeDef Clk_Config = {
	.osc = {
		.OscillatorType = RCC_OSCILLATORTYPE_HSE,
		.HSEState = RCC_HSE_ON,
		.PLL = {
			.PLLState = RCC_PLL_ON,
			.PLLSource = RCC_PLLSOURCE_HSE,
			.PLLQ = 9,
			.PLLM = 4,
			.PLLN = 216,
			.PLLP = RCC_PLLP_DIV2,
		}
	},
	.clk = {
			.ClockType =		RCC_CLOCKTYPE_HCLK	| RCC_CLOCKTYPE_SYSCLK |
								RCC_CLOCKTYPE_PCLK1	| RCC_CLOCKTYPE_PCLK2,
			.SYSCLKSource =		RCC_SYSCLKSOURCE_PLLCLK,
			.AHBCLKDivider =	RCC_SYSCLK_DIV1,
			.APB1CLKDivider =	RCC_HCLK_DIV4,
			.APB2CLKDivider =	RCC_HCLK_DIV2,

	},
	.periphclk = {
		.PeriphClockSelection =		RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_USART6 |
									RCC_PERIPHCLK_CLK48,
		.PLLSAI = {
			.PLLSAIN = 50,
			.PLLSAIR = 2,
			.PLLSAIQ = 2,
			.PLLSAIP = RCC_PLLSAIP_DIV2,
		},
		.PLLSAIDivQ = 1,
		.PLLSAIDivR = RCC_PLLSAIDIVR_4,
		.Usart6ClockSelection = RCC_USART6CLKSOURCE_SYSCLK,
		.Clk48ClockSelection = RCC_CLK48SOURCE_PLL,
		.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48,
	},
};
//------------------------------------------------------------------------------
void SystemClock_Config(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	if (HAL_RCC_OscConfig(&Clk_Config.osc) != HAL_OK) {
		PRINT_FUN_NAME_AND_LINE();
	}

	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		PRINT_FUN_NAME_AND_LINE();
	}

	if (HAL_RCC_ClockConfig(&Clk_Config.clk, FLASH_LATENCY_7) != HAL_OK) {
		PRINT_FUN_NAME_AND_LINE();
	}

	if (HAL_RCCEx_PeriphCLKConfig(&Clk_Config.periphclk) != HAL_OK) {
		PRINT_FUN_NAME_AND_LINE();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_RCC_EnableCSS();
}
//------------------------------------------------------------------------------
#else
	#warning "no select board"
#endif
//------------------------------------------------------------------------------
