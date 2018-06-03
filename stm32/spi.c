//------------------------------------------------------------------------------
#include "spi.h"
#include "def.h"
//------------------------------------------------------------------------------
static SPI_Device* spi_ptr[] = { NULL, NULL, NULL, NULL };
//------------------------------------------------------------------------------
#if defined(SPI1)
void SPI1_IRQHandler()
{
	for(int i=0;i<ARRAY_SIZE(spi_ptr);i++) {
		if(spi_ptr[i] && spi_ptr[i]->hspi.Instance == SPI1 ) {
			HAL_SPI_IRQHandler(&spi_ptr[i]->hspi);
		}
	}
}
#endif
//------------------------------------------------------------------------------
#if defined(SPI2)
void SPI2_IRQHandler()
{
	for(int i=0;i<ARRAY_SIZE(spi_ptr);i++) {
		if(spi_ptr[i] && spi_ptr[i]->hspi.Instance == SPI2 ) {
			HAL_SPI_IRQHandler(&spi_ptr[i]->hspi);
		}
	}
}
#endif
//------------------------------------------------------------------------------
#if defined(SPI3)
void SPI3_IRQHandler()
{

}
#endif
//------------------------------------------------------------------------------
#if defined(SPI4)
void SPI4_IRQHandler()
{

}
#endif
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
	SPI_Clk(dev->hspi.Instance,DISABLE);
	if (HAL_SPI_DeInit(&dev->hspi) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	GPIO_DeInitPin(dev->pins,dev->pins_size);
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
