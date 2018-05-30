#ifndef __PCF8574_H
#define __PCF8574_H
//------------------------------------------------------------------------------
#include "i2c.h"
#include "def.h"
//------------------------------------------------------------------------------
typedef struct {
	I2C_Device	i2c;
	uint8_t		i2c_addr;
} PCF8574_Device;
//------------------------------------------------------------------------------
void PCF8574_InitDev(PCF8574_Device *dev);
void PCF8574_DeInitDev(PCF8574_Device *dev);
//------------------------------------------------------------------------------
HAL_StatusTypeDef PCF8574_Write(PCF8574_Device *dev, uint8_t *data,int size);
HAL_StatusTypeDef PCF8574_Read(PCF8574_Device *dev, uint8_t *data,int size);
//------------------------------------------------------------------------------
void PCF8574_TestInit();
void PCF8574_TestLoop();
//------------------------------------------------------------------------------
#endif
