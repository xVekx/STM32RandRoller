//------------------------------------------------------------------------------
#include "spi.h"
#include "def.h"
//------------------------------------------------------------------------------
#define SPI_CLK(_port,_port_sel,_stat_sel)									\
					{if(_port == _port_sel)									\
						if(_stat_sel) 										\
						 __HAL_RCC_##_port##_CLK_ENABLE();					\
						 else												\
						 __HAL_RCC_##_port##_CLK_DISABLE();}

//------------------------------------------------------------------------------
static void SPI_Clk(SPI_TypeDef *spi,FunctionalState stat)
{
#if defined(SPI1)
	SPI_CLK(SPI1,spi,stat);
#endif

#if defined(SPI2)
	SPI_CLK(SPI2,spi,stat);
#endif

#if defined(SPI3)
	SPI_CLK(SPI3,spi,stat);
#endif

#if defined(SPI4)
	SPI_CLK(SPI4,spi,stat);
#endif
}
//------------------------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) { }
//------------------------------------------------------------------------------
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) { }
//------------------------------------------------------------------------------
void SPI_InitDev(SPI_Device *dev)
{
	SPI_Clk(dev->hspi.Instance,ENABLE);
	GPIO_InitPin(dev->pins,dev->pins_size);
	if (HAL_SPI_Init(&dev->hspi) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
}
//------------------------------------------------------------------------------
void SPI_DeInit(SPI_Device *dev)
{
	if (HAL_SPI_DeInit(&dev->hspi) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	GPIO_DeInitPin(dev->pins,dev->pins_size);
	SPI_Clk(dev->hspi.Instance,DISABLE);
}
#if 0
//------------------------------------------------------------------------------
void SPI_TestInit()
{

}
//------------------------------------------------------------------------------
void SPI_TestLoop()
{

}
//------------------------------------------------------------------------------
#endif
