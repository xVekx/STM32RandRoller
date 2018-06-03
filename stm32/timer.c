#include "timer.h"
//------------------------------------------------------------------------------
#if defined(TIM2)
static TIM_Base_Device *tim2_ptr = NULL;
#endif
//------------------------------------------------------------------------------
#if defined(TIM3)
static TIM_Base_Device *tim3_ptr = NULL;
#endif
//------------------------------------------------------------------------------
#if defined(TIM4)
static TIM_Base_Device *tim4_ptr = NULL;
#endif
//------------------------------------------------------------------------------
#define TIM_CLK(_dev,_dev_sel,_stat_sel)									\
					{if(_dev == _dev_sel)									\
						if(_stat_sel) 										\
							__HAL_RCC_##_dev##_CLK_ENABLE();				\
						else												\
							__HAL_RCC_##_dev##_CLK_DISABLE();}
//------------------------------------------------------------------------------
#define TIM_SET_PTR(_dev,_dev_sel,_ptr,_set)								\
					{if((_dev) == (_dev_sel)) _ptr = _set;}
//------------------------------------------------------------------------------
static void TIM_Clk(TIM_TypeDef *tim,FunctionalState stat)
{
#if defined(TIM1)
	TIM_CLK(TIM1,tim,stat);
#endif

#if defined(TIM2)
	TIM_CLK(TIM2,tim,stat);
#endif

#if defined(TIM3)
	TIM_CLK(TIM3,tim,stat);
#endif

#if defined(TIM4)
	TIM_CLK(TIM4,tim,stat);
#endif
}
//------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//printf("%s - 1\n",__func__);
	if(	tim3_ptr &&
		htim->Instance == tim3_ptr->htim.Instance &&
		tim3_ptr->period_elapsed_callback ) {
		tim3_ptr->period_elapsed_callback(htim);
		//printf("%s - 1\n",__func__);
	}
}
//------------------------------------------------------------------------------
void TIM3_IRQHandler(void)
{
	//printf("%s\n",__func__);
	if(tim3_ptr) {
		//printf("%s\n",__func__);
		HAL_TIM_IRQHandler(&tim3_ptr->htim);
	}
}
//------------------------------------------------------------------------------
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

}
//------------------------------------------------------------------------------
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{

}
//------------------------------------------------------------------------------
void TIM_Base_InitDev(TIM_Base_Device *dev)
{
	TIM_Clk(dev->htim.Instance,ENABLE);
	if(HAL_TIM_Base_Init(&dev->htim) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

#if defined(TIM2)
	TIM_SET_PTR(dev->htim.Instance,TIM2,tim2_ptr,dev);
#endif

#if defined(TIM3)
	TIM_SET_PTR(dev->htim.Instance,TIM3,tim3_ptr,dev);
#endif

#if defined(TIM4)
	TIM_SET_PTR(dev->htim.Instance,TIM4,tim4_ptr,dev);
#endif

	printf("%s\n",__func__);
}
//------------------------------------------------------------------------------
void TIM_Base_DeInitDev(TIM_Base_Device *dev)
{
	if(HAL_TIM_Base_DeInit(&dev->htim) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	TIM_Clk(dev->htim.Instance,DISABLE);

#if defined(TIM2)
	TIM_SET_PTR(dev->htim.Instance,TIM2,tim2_ptr,NULL);
#endif

#if defined(TIM3)
	TIM_SET_PTR(dev->htim.Instance,TIM3,tim3_ptr,NULL);
#endif

#if defined(TIM4)
	TIM_SET_PTR(dev->htim.Instance,TIM4,tim4_ptr,NULL);
#endif

	printf("%s\n",__func__);
}
//------------------------------------------------------------------------------
void TIM_Base_Start_IT(TIM_Base_Device *dev)
{
	if(HAL_TIM_Base_Start_IT(&dev->htim) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
#if defined(TIM3)
	if(dev->htim.Instance == TIM3) {
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}
#endif
	printf("%s\n",__func__);
}
//------------------------------------------------------------------------------
void TIM_Base_Stop_IT(TIM_Base_Device *dev)
{
#if defined(TIM3)
	if(dev->htim.Instance == TIM3) {
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
	}
#endif
	if(HAL_TIM_Base_Stop_IT(&dev->htim) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	printf("%s\n",__func__);
}
//------------------------------------------------------------------------------
#if 1

#include "gpio.h"
//------------------------------------------------------------------------------
static void tim_test(TIM_HandleTypeDef *htim)
{
	static int count = 0;
	if(count < 100) {
		count++;
	} else {
		count=0;
		printf("%s\n",__func__);
	}
}
//------------------------------------------------------------------------------
static TIM_Base_Device tim_base_init = {
	.htim = {
		.Instance = TIM3,
		.Init = {
			.Period = 0,
			.Prescaler = 0,
			.ClockDivision = 0,
			.CounterMode = TIM_COUNTERMODE_UP,
			.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
		},
	},
	.period_elapsed_callback = &tim_test,
};
//------------------------------------------------------------------------------
void TIM_TestInit()
{
	tim_base_init.htim.Init.Prescaler = (uint32_t) ((SystemCoreClock) / 10000) - 1;
	tim_base_init.htim.Init.Period = 200 - 1;
	//tim_base_init.htim.Init.Prescaler = 1000;
	TIM_Base_InitDev(&tim_base_init);
	TIM_Base_Start_IT(&tim_base_init);

	printf("SystemCoreClock:%i\n",SystemCoreClock);
}
//------------------------------------------------------------------------------
#endif
