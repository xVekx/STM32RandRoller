#include "spi.h"
#include "def.h"
//------------------------------------------------------------------------------
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
#if defined( SPI1 )
	if( hspi->Instance == SPI1 ) {
		__HAL_RCC_SPI1_CLK_ENABLE();
	}
#endif

#if defined( SPI2 )
	if( hspi->Instance == SPI2 ) {
		__HAL_RCC_SPI2_CLK_ENABLE();
	}
#endif

#if defined( SPI3 )
	if( hspi->Instance == SPI3 ) {
		__HAL_RCC_SPI3_CLK_ENABLE();
	}
#endif

#if defined( SPI4 )
	if( hspi->Instance == SPI4) {
		__HAL_RCC_SPI4_CLK_ENABLE();
	}
#endif
}
//------------------------------------------------------------------------------
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
#if defined( SPI1 )
	if( hspi->Instance == SPI1) {
		__HAL_RCC_SPI1_CLK_DISABLE();
	}
#endif

#if defined( SPI2 )
	if(hspi->Instance == SPI2) {
		__HAL_RCC_SPI2_CLK_DISABLE();
	}
#endif

#if defined( SPI3 )
	if(hspi->Instance==SPI3) {
		__HAL_RCC_SPI3_CLK_DISABLE();
	}
#endif

#if defined( SPI4 )
	if(hspi->Instance==SPI4) {
		__HAL_RCC_SPI4_CLK_DISABLE();
	}
#endif
}
//------------------------------------------------------------------------------
void SPI_InitDev(SPI_Device *dev)
{
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
}
//------------------------------------------------------------------------------
void SPI_TestInit()
{

}
//------------------------------------------------------------------------------
void SPI_TestLoop()
{

}
//------------------------------------------------------------------------------
