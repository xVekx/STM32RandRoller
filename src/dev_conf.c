#include "dev_conf.h"
////////////////////////////////////////////////////////////////////////////////
static const GPIO_Device MAX2719_GPIO_Init[] = {
	{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_OUTPUT_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_12,	//SPI2_NSS
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_13 |	//SPI2_SCK
							GPIO_PIN_15,	//SPI2_MOSI
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_INPUT,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_NOPULL,
			.Pin		=	GPIO_PIN_14,	//SPI2_MISO
		},
		.gpio_typedef	=	GPIOB,
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
static const GPIO_Device BUTTOM_GPIO_Init[] = {
	{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_INPUT,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_PULLDOWN,
			.Pin		=	GPIO_PIN_9,
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_INPUT,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pull		=	GPIO_PULLDOWN,
			.Pin		=	GPIO_PIN_8,
		},
		.gpio_typedef	=	GPIOB,
	}
};
//------------------------------------------------------------------------------
static BUTTOM_Device	BUTTOM_Init = {
	.pins = BUTTOM_GPIO_Init,
	.pins_size = ARRAY_SIZE(BUTTOM_GPIO_Init),
	.count_ready = 20,
};
////////////////////////////////////////////////////////////////////////////////
static GPIO_Device HD44780_I2C_GPIO_Init[] = {
	{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pin		=	GPIO_PIN_7,
		},
		.gpio_typedef	=	GPIOB,
	},{
		.gpio_inittypedef = {
			.Mode		=	GPIO_MODE_AF_OD,
			.Speed		=	GPIO_SPEED_FREQ_HIGH,
			.Pin		=	GPIO_PIN_6,
		},
		.gpio_typedef	=	GPIOB,
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
static void rtc_event_callback(RTC_HandleTypeDef *hrtc)
{
	RandRollDevice.flag_update_1hz = 1;
}
//------------------------------------------------------------------------------
static RTC_Device RTC_Init = {
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
	.rtc_event_callback = &rtc_event_callback,
};
////////////////////////////////////////////////////////////////////////////////
static int tim3_count = 0;
//------------------------------------------------------------------------------
static void tim3_period_elapsed_callback(TIM_HandleTypeDef *htim)
{
	RandRollDevice.flag_update_100hz = 1;

	if(tim3_count < 10) {
		tim3_count++;
	} else {
		tim3_count=0;
		RandRollDevice.flag_update_10hz = 1;
	}
}
//------------------------------------------------------------------------------
static TIM_Base_Device TIM3_Base_Init = {
	.htim = {
		.Instance = TIM3,
		.Init = {
			.Prescaler = (uint32_t) ((72000000) / 10000) - 1,
			.Period = 200 - 1,
			.ClockDivision = 0,
			.CounterMode = TIM_COUNTERMODE_UP,
			.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
		},
	},
	.period_elapsed_callback = &tim3_period_elapsed_callback,
};
////////////////////////////////////////////////////////////////////////////////
Struct_Device RandRollDevice = {
	.max2719			= &MAX2719_Init,
	.buttom				= &BUTTOM_Init,
	.hd44780_i2c		= &HD44780_I2C_Init,
	.rtc				= &RTC_Init,
	.tim3_base			= &TIM3_Base_Init,
	.flag_update_1hz	= 0,
	.flag_update_10hz	= 0,
	.flag_update_100hz	= 0
};
//------------------------------------------------------------------------------
