#ifndef __HD44780_I2C_H
#define __HD44780_I2C_H
//------------------------------------------------------------------------------
#include "PCF8574.h"
//------------------------------------------------------------------------------
typedef enum {
	/* Очищает индикатор и помещает курсор в самую левую позицию */
	HD44780_Comm_DisplayClear		= (1u<<0),

	/* Перемещает курсор в левую позицию */
	HD44780_Comm_ReturnHome			= (1u<<1),

	/* Установка направления сдвига курсора (ID=0/1—влево/вправо)
	 * и разрешение сдвига дисплея(SH=1) при записи в DDRAM */
	HD44780_Comm_EntryModeSet		= (1u<<2),

	/*Включает индикатор (D=1) и выбирает тип курсора (C, B) */
	HD44780_Comm_DisplayControl		= (1u<<3),

	/* Выполняет сдвиг дисплея или курсора
	 * (SC=0/1—курсор/дисплей, RL=0/1—влево/вправо) */
	HD44780_Comm_CursOrDispShift	= (1u<<4),

	/* Установка разрядности интерфейса (DL=0/1—4/8 бита) и
	 * страницы знакогенератора P */
	HD44780_Comm_FunctionSet		= (1u<<5) | (1u<<3),

	/* Установка адреса для последующих операций (и
	 * установка туда курсора) и выбор области CGRAM */
	HD44780_Comm_CGRAM		= (1u<<6),

	/*Установка адреса для последующих операций
	 * и выбор области DDRAM */
	HD44780_Comm_DDRAM		= (1u<<7)
} HD44780_Command;
//------------------------------------------------------------------------------
typedef enum {
	/* Разрешение сдвига дисплея (SH=1) при записи в DDRAM */
	HD44780_EntryModeSet_SH			= (1u<<0),

	/* Cдвига курсора ID=0/1—влево/вправо */
	HD44780_EntryModeSet_ID			= (1u<<1)
} HD44780_EntryModeSet;
//------------------------------------------------------------------------------
typedef enum {
	/* Включить индикатор (D=1) */
	HD44780_DisplayControl_DOn		= (1u<<2),

	/* курсора нет, ничего не мигает */
	HD44780_DisplayControl_C0		= 0x00,

	/* курсора нет, мигает весь символ в позиции курсора */
	HD44780_DisplayControl_C1		= 0x01,
	HD44780_DisplayControl_B		= HD44780_DisplayControl_C1,

	/* курсор есть (подчёркивание), ничего не мигает; */
	HD44780_DisplayControl_C2		= 0x02,
	HD44780_DisplayControl_C		= HD44780_DisplayControl_C2,

	/* курсор есть (подчёркивание) и только он и мигает.*/
	HD44780_DisplayControl_C3		= 0x03,
	HD44780_DisplayControl_CB		= HD44780_DisplayControl_C3

} HD44780_DisplayControl;
//------------------------------------------------------------------------------
typedef enum {
	/* Выбор типа сдвига SC=0/1—курсор/дисплей */
	HD44780_CursOrDispShift_Disp		= (1u<<3),

	/* Выбор направления сдвига RL=0/1—влево/вправо*/
	HD44780_CursOrDispShift_Right		= (1u<<2)

} HD44780_CursOrDispShift;
//------------------------------------------------------------------------------
typedef enum {
	/* Установка разрядности интерфейса (DL=0/1—4/8 бита) */
	HD44780_FunctionSet_8bit			= (1u<<4),

	/* Установка страницы знакогенератора P */
	HD44780_FunctionSet_Page			= (1u<<1),

} HD44780_FunctionSet;
//------------------------------------------------------------------------------
typedef enum {
	HD44780_DDRAM_Line0					= 0x00,
	HD44780_DDRAM_Line1					= 0x40,
	HD44780_DDRAM_Line2					= 0x10,
	HD44780_DDRAM_Line3					= 0x50,
} HD44780_DDRAM;
//------------------------------------------------------------------------------
typedef union {
	struct {
		uint8_t a0:1;
		uint8_t rw:1;
		uint8_t e:1;
		uint8_t led:1;
		uint8_t d:4;
	} bit;
	uint8_t data;
} HD44780_I2C_StrData;
//------------------------------------------------------------------------------
typedef struct {
	PCF8574_Device	pcf8574;
	HD44780_I2C_StrData	data;

} HD44780_I2C_Device;
//------------------------------------------------------------------------------
void HD44780_I2C_InitDev(HD44780_I2C_Device *dev);
void HD44780_I2C_DeInitDev(HD44780_I2C_Device *dev);

void HD44780_I2C_LedOn(HD44780_I2C_Device *dev);
void HD44780_I2C_LedOff(HD44780_I2C_Device *dev);

void HD44780_I2C_Command(HD44780_I2C_Device *dev,uint8_t data);
void HD44780_I2C_Data(HD44780_I2C_Device *dev,uint8_t data);

void HD44780_I2C_ClearDisplay(HD44780_I2C_Device *dev);
void HD44780_I2C_ReturnHome(HD44780_I2C_Device *dev);
void HD44780_I2C_EntryModeSet(HD44780_I2C_Device *dev,HD44780_EntryModeSet ems);
void HD44780_I2C_DisplayControl(HD44780_I2C_Device *dev,HD44780_DisplayControl dc);
void HD44780_I2C_CursOrDispShift(HD44780_I2C_Device *dev,HD44780_CursOrDispShift cods);
void HD44780_I2C_FunctionSet(HD44780_I2C_Device *dev,HD44780_FunctionSet fs);
void HD44780_I2C_SetDDRAM(HD44780_I2C_Device *dev,HD44780_DDRAM addr);

int HD44780_I2C_vsnprintf(	HD44780_I2C_Device *dev,
							HD44780_DDRAM addr,
							const char *text,
							...);

//------------------------------------------------------------------------------
void HD44780_I2C_TestInit();
void HD44780_I2C_TestLoop();
//------------------------------------------------------------------------------
#endif
