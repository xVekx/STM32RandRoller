#include "dev_print.h"
//------------------------------------------------------------------------------
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
//------------------------------------------------------------------------------
static UART_Device	*uart_debug = NULL;
//------------------------------------------------------------------------------
void DEV_PRINT_SetPtrDebugUart(UART_Device *dev)
{
	uart_debug = dev;
}
//------------------------------------------------------------------------------
int _write (int fd, char *ptr, int len)
{
	UNUSED(fd);
	if(fd == STDERR_FILENO) {
		//HAL_UART_Transmit(	&UART_DebugInit.huart,
		//					(uint8_t*)"[ERR]",5,0xFFFF);
		for(int i=0;i<len;i++) {
			if(uart_debug) {
				HAL_UART_Transmit(	&uart_debug->huart,
									(uint8_t*)&ptr[i],1,0xFFFF);
				if(ptr[i] == '\n') {
					HAL_UART_Transmit(	&uart_debug->huart,
										(uint8_t*)"\r",1,0xFFFF);
				}
			}
		}
	} else if (fd == STDOUT_FILENO) {
		if(uart_debug) {
			HAL_UART_Transmit(	&uart_debug->huart,
								(uint8_t*)"[OUT]",5,0xFFFF);
			for(int i=0;i<len;i++) {
				HAL_UART_Transmit(	&uart_debug->huart,
									(uint8_t*)&ptr[i],1,0xFFFF);
				if(ptr[i] == '\n') {
					HAL_UART_Transmit(	&uart_debug->huart,
										(uint8_t*)"\r",1,0xFFFF);
				}
			}
		}
	}
	return len;
}

