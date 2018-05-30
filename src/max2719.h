#ifndef __MAX2719_H
#define __MAX2719_H
//------------------------------------------------------------------------------
#include "spi.h"
//------------------------------------------------------------------------------
typedef enum {
	MAX2719_Addr_NOOP			= 0x00,
	MAX2719_Addr_DIG_0			= 0x01,
	MAX2719_Addr_DIG_1			= 0x02,
	MAX2719_Addr_DIG_2			= 0x03,
	MAX2719_Addr_DIG_3			= 0x04,
	MAX2719_Addr_DIG_4			= 0x05,
	MAX2719_Addr_DIG_5			= 0x06,
	MAX2719_Addr_DIG_6			= 0x07,
	MAX2719_Addr_DIG_7			= 0x08,
	MAX2719_Addr_DecodeMode		= 0x09,
	MAX2719_Addr_Intensity		= 0x0A,
	MAX2719_Addr_ScanLimit		= 0x0B,
	MAX2719_Addr_Shutdown		= 0x0C,
	MAX2719_Addr_DisplayTest	= 0x0F,
} MAX2719_Addr;
//------------------------------------------------------------------------------
typedef enum {
	MAX2719_ShutdownOff			= 0x00,
	MAX2719_ShutdownOn			= 0x01,

} MAX2719_Shutdown;
//------------------------------------------------------------------------------
typedef enum {
	MAX2719_DecodeMode_0		= 0x00,
	MAX2719_DecodeMode_1		= 0x01,
	MAX2719_DecodeMode_2		= 0x0F,
	MAX2719_DecodeModeFULL		= 0xFF
} MAX2719_DecodeMode;
//------------------------------------------------------------------------------
typedef enum {
	MAX2719_ScanLimit_0			= 0x00,
	MAX2719_ScanLimit_01,
	MAX2719_ScanLimit_012,
	MAX2719_ScanLimit_0123,
	MAX2719_ScanLimit_01234,
	MAX2719_ScanLimit_012345,
	MAX2719_ScanLimit_0123456,
	MAX2719_ScanLimit_01234567,
} MAX2719_ScanLimit;
//------------------------------------------------------------------------------
typedef enum {
	MAX2719_Intensity_01_32		= 0x00,
	MAX2719_Intensity_03_32		= 0x01,
	MAX2719_Intensity_05_32		= 0x02,
	MAX2719_Intensity_07_32		= 0x03,
	MAX2719_Intensity_09_32		= 0x04,
	MAX2719_Intensity_11_32		= 0x05,
	MAX2719_Intensity_13_32		= 0x06,
	MAX2719_Intensity_15_32		= 0x07,
	MAX2719_Intensity_17_32		= 0x08,
	MAX2719_Intensity_19_32		= 0x09,
	MAX2719_Intensity_21_32		= 0x0A,
	MAX2719_Intensity_23_32		= 0x0B,
	MAX2719_Intensity_25_32		= 0x0C,
	MAX2719_Intensity_27_32		= 0x0D,
	MAX2719_Intensity_29_32		= 0x0E,
	MAX2719_Intensity_31_32		= 0x0F,
} MAX2719_Intensity;
//------------------------------------------------------------------------------
typedef enum {
	MAX2719_FontCode_0			= 0x00,
	MAX2719_FontCode_1,
	MAX2719_FontCode_2,
	MAX2719_FontCode_3,
	MAX2719_FontCode_4,
	MAX2719_FontCode_5,
	MAX2719_FontCode_6,
	MAX2719_FontCode_7,
	MAX2719_FontCode_8,
	MAX2719_FontCode_9,
	MAX2719_FontCode_Minus,
	MAX2719_FontCode_E,
	MAX2719_FontCode_H,
	MAX2719_FontCode_L,
	MAX2719_FontCode_P,
	MAX2719_FontCode_Blank,
	MAX2719_FontCode_Dot		= (1<<7)
} MAX2719_FontCode;
//------------------------------------------------------------------------------
typedef enum {
	MAX2719_DisplayTestOff		= 0x00,
	MAX2719_DisplayTestOn		= 0x01,
} MAX2719_DisplayTest;
//------------------------------------------------------------------------------
typedef struct {
	SPI_Device			spi;
	MAX2719_Intensity	intens;
	MAX2719_ScanLimit	scanlimit;
	MAX2719_Addr		digitbegin;
	MAX2719_Addr		digitend;
//TODO добавить пин cs
	//void (*init_command)(void);
} MAX2719_DevDef;
//------------------------------------------------------------------------------
void MAX2719_InitDev(MAX2719_DevDef *dev);

void MAX2719_TestInit();
void MAX2719_TestLoop();
//void MAX2719_TestInit();
//------------------------------------------------------------------------------
#endif
