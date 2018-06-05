#include "dev_conf.h"
////////////////////////////////////////////////////////////////////////////////
///***************************LED Display************************************///
////////////////////////////////////////////////////////////////////////////////
//Настойка выводов LED дисплея
static const GPIO_Device MAX2719_GPIO_Init[] = {
	{
		.gpio_init = {
			.Mode		=	GPIO_MODE_OUTPUT_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_12,	//SPI2_NSS
		},
		.gpio_type	=	GPIOB,
	},{
		.gpio_init = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_13 |	//SPI2_SCK
							GPIO_PIN_15,	//SPI2_MOSI
		},
		.gpio_type	=	GPIOB,
	}
};
//------------------------------------------------------------------------------
static MAX2719_DevDevice	MAX2719_Init = {
	.spi =  {
		.hspi = {
			.Instance = SPI2,
			.Init = {
				.Mode				= SPI_MODE_MASTER,
				.Direction			= SPI_DIRECTION_2LINES,
				.DataSize			= SPI_DATASIZE_16BIT,
				.CLKPolarity		= SPI_POLARITY_LOW,
				.CLKPhase			= SPI_PHASE_1EDGE,
				.NSS				= SPI_NSS_SOFT,
				.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_256,
				.FirstBit			= SPI_FIRSTBIT_MSB,
				.TIMode				= SPI_TIMODE_DISABLE,
				.CRCCalculation		= SPI_CRCCALCULATION_DISABLE,
				.CRCPolynomial		= 10,
			}
		},
		.pins = MAX2719_GPIO_Init,
		.pins_size = ARRAY_SIZE(MAX2719_GPIO_Init),
	},
	.NSS = 0,
};
////////////////////////////////////////////////////////////////////////////////
///***************************BUTTOM*****************************************///
////////////////////////////////////////////////////////////////////////////////
 //Настойка выводов клавиатуры
static const GPIO_Device BUTTOM_GPIO_Init[] = {
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
static BUTTOM_Device	BUTTOM_Init = {
	.pins = BUTTOM_GPIO_Init,
	.pins_size = ARRAY_SIZE(BUTTOM_GPIO_Init),
	.count_ready = 5,
};
////////////////////////////////////////////////////////////////////////////////
///***************************LCD Display************************************///
////////////////////////////////////////////////////////////////////////////////
//Настойка выводов LСD дисплея
static GPIO_Device HD44780_I2C_GPIO_Init[] = {
	{
		.gpio_init = {
			.Mode		= GPIO_MODE_AF_OD,
			.Speed		= GPIO_SPEED_FREQ_HIGH,
			.Pin		= GPIO_PIN_7,
		},
		.gpio_type	= GPIOB,
	},{
		.gpio_init = {
			.Mode		= GPIO_MODE_AF_OD,
			.Speed		= GPIO_SPEED_FREQ_HIGH,
			.Pin		= GPIO_PIN_6,
		},
		.gpio_type	= GPIOB,
	}
};
//------------------------------------------------------------------------------
static HD44780_I2C_Device HD44780_I2C_Init = {
	.pcf8574 = {
		.i2c = {
			.hi2c = {
					.Instance = I2C1,
					.Init = {
					.ClockSpeed       = 100000,
					.DutyCycle        = I2C_DUTYCYCLE_2,
					.OwnAddress1      = 0,
					.AddressingMode   = I2C_ADDRESSINGMODE_7BIT,
					.DualAddressMode  = I2C_DUALADDRESS_DISABLE,
					.OwnAddress2      = 0,
					.GeneralCallMode  = I2C_GENERALCALL_DISABLE,
					.NoStretchMode    = I2C_NOSTRETCH_DISABLE,
				}
			},
			.pins = HD44780_I2C_GPIO_Init,
			.pins_size = ARRAY_SIZE(HD44780_I2C_GPIO_Init),
		},
		.i2c_addr = 0x27,
	}
};
////////////////////////////////////////////////////////////////////////////////
///***************************RTC********************************************///
////////////////////////////////////////////////////////////////////////////////
//Обработчик прерывания с rtc
static void rtc_event_callback(RTC_HandleTypeDef *hrtc)
{
	UNUSED(hrtc);
	RandRollDevice.flag_update_1hz = 1;
}
//------------------------------------------------------------------------------
static RTC_Device RTC_Init = {
	.hrtc = {
		.Instance = RTC,
		.Init = {
			.AsynchPrediv	= RTC_AUTO_1_SECOND,
			.OutPut			= RTC_OUTPUTSOURCE_NONE,
		},
	},
	.time = {
		.Hours		= 0x0,
		.Minutes	= 0x0,
		.Seconds	= 0x0,
	},
	.date = {
		.WeekDay	= RTC_WEEKDAY_MONDAY,
		.Month		= RTC_MONTH_JANUARY,
		.Date		= 0x1,
		.Year		= 0x0,
	},
	.rtc_event_callback = &rtc_event_callback,
};
////////////////////////////////////////////////////////////////////////////////
///***************************TIMER3*****************************************///
////////////////////////////////////////////////////////////////////////////////
//Обработчик прерывания с таймера 3
static void tim3_period_elapsed_callback(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);

	RandRollDevice.flag_update_100hz = 1;

	static int tim3_count_10_hz = 0;
	static int tim3_count_2_hz = 0;

	if(tim3_count_10_hz < 10 - 1) {
		tim3_count_10_hz++;
	} else {
		tim3_count_10_hz=0;
		RandRollDevice.flag_update_10hz = 1;
	}

	if(tim3_count_2_hz < 50 - 1) {
		tim3_count_2_hz++;
	} else {
		tim3_count_2_hz = 0;
		RandRollDevice.flag_update_2hz = 1;
	}
}
//------------------------------------------------------------------------------
static TIM_Base_Device TIM3_Base_Init = {
	.htim = {
		.Instance = TIM3,
		.Init = {
			.Prescaler		= (uint32_t) ((72000000) / 10000) - 1,
			.Period			= 100 - 1,
			.ClockDivision	= 0,
			.CounterMode	= TIM_COUNTERMODE_UP,
			.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
		},
	},
	.period_elapsed_callback = &tim3_period_elapsed_callback,
};
////////////////////////////////////////////////////////////////////////////////
///***************************UART DEBUG*************************************///
////////////////////////////////////////////////////////////////////////////////
//Настойка выводов uart
static const GPIO_Device UART_Debug_GPIO_Init[] = {
	{
		.gpio_init = {
			.Mode			= GPIO_MODE_AF_PP,
			.Speed			= GPIO_SPEED_FREQ_HIGH,
			.Pull			= GPIO_NOPULL,
			.Pin			= GPIO_PIN_9,
		},
		.gpio_type		= GPIOA,
	}
};
//------------------------------------------------------------------------------
static UART_Device UART_Debug_Init = {

	.huart = {
		.Instance = USART1,
		.Init = {
			.BaudRate		= 115200,
			.WordLength		= UART_WORDLENGTH_8B,
			.StopBits		= UART_STOPBITS_1,
			.Parity			= UART_PARITY_NONE,
			.Mode			= UART_MODE_TX_RX,
			.HwFlowCtl		= UART_HWCONTROL_NONE,
			.OverSampling	= UART_OVERSAMPLING_16,
		}
	},
	.pins = UART_Debug_GPIO_Init,
	.pins_size = ARRAY_SIZE(UART_Debug_GPIO_Init),
};
////////////////////////////////////////////////////////////////////////////////
///***************************RCC********************************************///
////////////////////////////////////////////////////////////////////////////////
static  RCC_Device RCC_Init = {
	.osc = {
		.OscillatorType			=	RCC_OSCILLATORTYPE_LSE	|
									RCC_OSCILLATORTYPE_HSE,
		.HSEState				=	RCC_HSE_ON,
		.LSEState				=	RCC_LSE_ON,
		.HSEPredivValue			=	RCC_HSE_PREDIV_DIV1,
		.PLL = {
			.PLLState			=	RCC_PLL_ON,
			.PLLSource			=	RCC_PLLSOURCE_HSE,
			.PLLMUL				=	RCC_PLL_MUL9,
		}
	},
	.clk = {
		.ClockType				=	RCC_CLOCKTYPE_HCLK		|
									RCC_CLOCKTYPE_SYSCLK	|
									RCC_CLOCKTYPE_PCLK1		|
									RCC_CLOCKTYPE_PCLK2,

		.SYSCLKSource			=	RCC_SYSCLKSOURCE_PLLCLK,
		.AHBCLKDivider			=	RCC_SYSCLK_DIV1,
		.APB1CLKDivider			=	RCC_HCLK_DIV2,
		.APB2CLKDivider			=	RCC_HCLK_DIV1,
	},
	.periphclk = {
		.PeriphClockSelection	=	RCC_PERIPHCLK_RTC	|
									RCC_PERIPHCLK_USB,
		.RTCClockSelection		=	RCC_RTCCLKSOURCE_HSE_DIV128,
		.UsbClockSelection		=	RCC_USBCLKSOURCE_PLL_DIV1_5,

	},
};
////////////////////////////////////////////////////////////////////////////////
///***************************ALG RANDOM*************************************///
////////////////////////////////////////////////////////////////////////////////
static RandAlgNode RandAlgNode_Init[] = {
	{	.count		= 2,
		.rand_min	= 5,
		.delta		= 95,
	},{
		.count		= 17,
		.rand_min	= 7,
		.delta		= 30,
	},{
		.count		= 15,
		.rand_min	= 4,
		.delta		= 15,
	},{
		.count		= 7,
		.rand_min	= 1,
		.delta		= 0,
	}
};
//------------------------------------------------------------------------------
static RandAlg RandAlg_Init = {
	.node = RandAlgNode_Init,
	.node_sz = ARRAY_SIZE(RandAlgNode_Init),
	.min = 5,
	.max = 100,
};
////////////////////////////////////////////////////////////////////////////////
///***************************LED DIODE**************************************///
////////////////////////////////////////////////////////////////////////////////
static const GPIO_Device LED_GPIO_Init = {
		.gpio_init = {
			.Mode			= GPIO_MODE_OUTPUT_PP,
			.Speed			= GPIO_SPEED_FREQ_HIGH,
			.Pull			= GPIO_NOPULL,
			.Pin			= GPIO_PIN_8,
		},
		.gpio_type		= GPIOA,
};
////////////////////////////////////////////////////////////////////////////////
Struct_Device RandRollDevice = {
	.max2719				= &MAX2719_Init,
	.buttom					= &BUTTOM_Init,
	.hd44780_i2c			= &HD44780_I2C_Init,
	.rtc					= &RTC_Init,
	.tim3_base				= &TIM3_Base_Init,
	.uart_debug				= &UART_Debug_Init,
	.rcc					= &RCC_Init,
	.ra						= &RandAlg_Init,
	.led					= &LED_GPIO_Init,

	.flag_update_1hz		= 0,
	.flag_update_10hz		= 0,
	.flag_update_100hz		= 0
};
//------------------------------------------------------------------------------
