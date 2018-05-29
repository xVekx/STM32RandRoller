#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
//------------------------------------------------------------------------------
#include "uart.h"
#include "debug_print.h"
#include "def.h"
//------------------------------------------------------------------------------
static const GPIO_InitDevTypeDef GPIO_DebugUartPinInit[] = {
	{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_AF_PP,
			.Speed		= GPIO_SPEED_FREQ_HIGH,
			.Pull		= GPIO_NOPULL,
			.Pin		= GPIO_PIN_9,
		},
		.gpio_typedef	= GPIOA,
	},{
		.gpio_inittypedef = {
			.Mode		= GPIO_MODE_INPUT,
			.Speed		= GPIO_SPEED_FREQ_HIGH,
			.Pull		= GPIO_NOPULL,
			.Pin		= GPIO_PIN_10,
		},
		.gpio_typedef	= GPIOA,
	}
};
//------------------------------------------------------------------------------
static UART_InitDevTypeDev UART_DebugInit = {

	.huart = {
		.Instance = USART1,
		.Init = {
			.BaudRate = 115200,
			.WordLength = UART_WORDLENGTH_8B,
			.StopBits = UART_STOPBITS_1,
			.Parity = UART_PARITY_NONE,
			.Mode = UART_MODE_TX_RX,
			.HwFlowCtl = UART_HWCONTROL_NONE,
			.OverSampling = UART_OVERSAMPLING_16,
		}
	},
	.pins = GPIO_DebugUartPinInit,
	.pins_size = ARRAY_SIZE(GPIO_DebugUartPinInit),
};
//------------------------------------------------------------------------------
void DEBUG_Init()
{
	UART_InitDev(&UART_DebugInit);
}


//fmemopen
//------------------------------------------------------------------------------
int _write (int fd, char *ptr, int len)
{
	UNUSED(fd);
	if(fd == STDERR_FILENO) {
		HAL_UART_Transmit(	&UART_DebugInit.huart,
							(uint8_t*)"[ERR]",5,0xFFFF);
		for(int i=0;i<len;i++) {
			HAL_UART_Transmit(	&UART_DebugInit.huart,
								(uint8_t*)&ptr[i],1,0xFFFF);
			if(ptr[i] == '\n') {
				HAL_UART_Transmit(	&UART_DebugInit.huart,
									(uint8_t*)"\r",1,0xFFFF);
			}
		}
	} else if (fd == STDOUT_FILENO) {
		HAL_UART_Transmit(	&UART_DebugInit.huart,
							(uint8_t*)"[OUT]",5,0xFFFF);
		for(int i=0;i<len;i++) {
			HAL_UART_Transmit(	&UART_DebugInit.huart,
								(uint8_t*)&ptr[i],1,0xFFFF);
			if(ptr[i] == '\n') {
				HAL_UART_Transmit(	&UART_DebugInit.huart,
									(uint8_t*)"\r",1,0xFFFF);
			}
		}
	}
	return len;
}
//------------------------------------------------------------------------------
/* http://www.pixelbeat.org/programming/gcc/format_specs.html
%[flags][min field width][precision][length]conversion specifier
  -----  ---------------  ---------  ------ -------------------
   \             #,*        .#, .*     /             \
	\                                 /               \
   #,0,-,+, ,',I                 hh,h,l,ll,j,z,L    c,d,u,x,X,e,f,g,s,p,%
   -------------                 ---------------    -----------------------
   # | Alternate,                 hh | char,           c | unsigned char,
   0 | zero pad,                   h | short,          d | signed int,
   - | left align,                 l | long,           u | unsigned int,
   + | explicit + - sign,         ll | long long,      x | unsigned hex int,
	 | space for + sign,           j | [u]intmax_t,    X | unsigned HEX int,
   ' | locale thousands grouping,  z | size_t,         e | [-]d.ddde±dd double,
   I | Use locale's alt digits     t | ptrdiff_t,      E | [-]d.dddE±dd double,
								   L | long double,  ---------=====
   if no precision   => 6 decimal places            /  f | [-]d.ddd double,
   if precision = 0  => 0 decimal places      _____/   g | e|f as appropriate,
   if precision = #  => # decimal places               G | E|F as appropriate,
   if flag = #       => always show decimal point      s | string,
											 ..............------
											/          p | pointer,
   if precision      => max field width    /           % | %
*/
