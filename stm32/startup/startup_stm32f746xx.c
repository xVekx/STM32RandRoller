extern unsigned long  _sidata;
/* start address for the .data section. defined in linker script */
extern unsigned long  _sdata;
/* end address for the .data section. defined in linker script */
extern unsigned long  _edata;
/* start address for the .bss section. defined in linker script */
extern unsigned long  _sbss;
/* end address for the .bss section. defined in linker script */
extern unsigned long _ebss;
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */
extern unsigned long _estack;
//------------------------------------------------------------------------------
extern int main(void);
extern void SystemInit(void);
extern void __libc_init_array(void);
extern int printf(const char*,...);
//------------------------------------------------------------------------------
void Reset_Handler(void)
{
	/* Initialize data and bss */
	unsigned long *pulSrc, *pulDest;

	/* Copy the data segment initializers from flash to SRAM */
	pulSrc = &_sidata;

	for(pulDest = &_sdata; pulDest < &_edata; ) {
		*(pulDest++) = *(pulSrc++);
	}
	/* Zero fill the bss segment. */
	for(pulDest = &_sbss; pulDest < &_ebss; ) {
	*(pulDest++) = 0;
	}

	SystemInit();
	//__libc_init_array(); C++
	main();
}
//------------------------------------------------------------------------------
void Default_Handler(void)
{
	printf("Default_Handler\n");
	while (1) {
	}
}
//------------------------------------------------------------------------------
#define WEAK __attribute__ ((weak, alias("Default_Handler")))
//------------------------------------------------------------------------------
void WEAK NMI_Handler(void);
void WEAK HardFault_Handler(void);
void WEAK MemManage_Handler(void);
void WEAK BusFault_Handler(void);
void WEAK UsageFault_Handler(void);

void WEAK SVC_Handler(void);
void WEAK DebugMon_Handler(void);

void WEAK PendSV_Handler(void);
void WEAK SysTick_Handler(void);

void WEAK WWDG_IRQHandler(void);
void WEAK PVD_IRQHandler(void);
void WEAK TAMP_STAMP_IRQHandler(void);
void WEAK RTC_WKUP_IRQHandler(void);
void WEAK FLASH_IRQHandler(void);
void WEAK RCC_IRQHandler(void);

void WEAK EXTI0_IRQHandler(void);
void WEAK EXTI1_IRQHandler(void);
void WEAK EXTI2_IRQHandler(void);
void WEAK EXTI3_IRQHandler(void);
void WEAK EXTI4_IRQHandler(void);
  
void WEAK DMA1_Stream0_IRQHandler(void);
void WEAK DMA1_Stream1_IRQHandler(void);
void WEAK DMA1_Stream2_IRQHandler(void);
void WEAK DMA1_Stream3_IRQHandler(void);
void WEAK DMA1_Stream4_IRQHandler(void);
void WEAK DMA1_Stream5_IRQHandler(void);
void WEAK DMA1_Stream6_IRQHandler(void);
          
void WEAK ADC_IRQHandler(void);
void WEAK CAN1_TX_IRQHandler(void);
void WEAK CAN1_RX0_IRQHandler(void);
void WEAK CAN1_RX1_IRQHandler(void);
void WEAK CAN1_SCE_IRQHandler(void);
void WEAK EXTI9_5_IRQHandler(void);
void WEAK TIM1_BRK_TIM9_IRQHandler(void);
void WEAK TIM1_UP_TIM10_IRQHandler(void);
void WEAK TIM1_TRG_COM_TIM11_IRQHandler(void);
void WEAK TIM1_CC_IRQHandler(void);
void WEAK TIM2_IRQHandler(void);
void WEAK TIM3_IRQHandler(void);
void WEAK TIM4_IRQHandler(void);
void WEAK I2C1_EV_IRQHandler(void);
void WEAK I2C1_ER_IRQHandler(void);
void WEAK I2C2_EV_IRQHandler(void);
void WEAK I2C2_ER_IRQHandler(void);
void WEAK SPI1_IRQHandler(void);
void WEAK SPI2_IRQHandler(void);
void WEAK USART1_IRQHandler(void);
void WEAK USART2_IRQHandler(void);
void WEAK USART3_IRQHandler(void);
void WEAK EXTI15_10_IRQHandler(void);
void WEAK RTC_Alarm_IRQHandler(void);
void WEAK OTG_FS_WKUP_IRQHandler(void);
void WEAK TIM8_BRK_TIM12_IRQHandler(void);
void WEAK TIM8_UP_TIM13_IRQHandler(void);
void WEAK TIM8_TRG_COM_TIM14_IRQHandler(void);
void WEAK TIM8_CC_IRQHandler(void);
void WEAK DMA1_Stream7_IRQHandler(void);
void WEAK FMC_IRQHandler(void);
void WEAK SDMMC1_IRQHandler(void);
void WEAK TIM5_IRQHandler(void);
void WEAK SPI3_IRQHandler(void);
void WEAK UART4_IRQHandler(void);
void WEAK UART5_IRQHandler(void);
void WEAK TIM6_DAC_IRQHandler(void);
void WEAK TIM7_IRQHandler(void);
        
void WEAK DMA2_Stream0_IRQHandler(void);
void WEAK DMA2_Stream1_IRQHandler(void);
void WEAK DMA2_Stream2_IRQHandler(void);
void WEAK DMA2_Stream3_IRQHandler(void);
void WEAK DMA2_Stream4_IRQHandler(void);

void WEAK ETH_IRQHandler(void);
void WEAK ETH_WKUP_IRQHandler(void);
void WEAK CAN2_TX_IRQHandler(void);
void WEAK CAN2_RX0_IRQHandler(void);
void WEAK CAN2_RX1_IRQHandler(void);
void WEAK CAN2_SCE_IRQHandler(void);
void WEAK OTG_FS_IRQHandler(void);
void WEAK DMA2_Stream5_IRQHandler(void);
void WEAK DMA2_Stream6_IRQHandler(void);
void WEAK DMA2_Stream7_IRQHandler(void);
void WEAK USART6_IRQHandler(void);
void WEAK I2C3_EV_IRQHandler(void);
void WEAK I2C3_ER_IRQHandler(void);
void WEAK OTG_HS_EP1_OUT_IRQHandler(void);
void WEAK OTG_HS_EP1_IN_IRQHandler(void);
void WEAK OTG_HS_WKUP_IRQHandler(void);
void WEAK OTG_HS_IRQHandler(void);
void WEAK DCMI_IRQHandler(void);
void WEAK RNG_IRQHandler(void);
void WEAK FPU_IRQHandler(void);
void WEAK UART7_IRQHandler(void);
void WEAK UART8_IRQHandler(void);
void WEAK SPI4_IRQHandler(void);
void WEAK SPI5_IRQHandler(void);
void WEAK SPI6_IRQHandler(void);
void WEAK SAI1_IRQHandler(void);
void WEAK LTDC_IRQHandler(void);
void WEAK LTDC_ER_IRQHandler(void);
void WEAK DMA2D_IRQHandler(void);
void WEAK SAI2_IRQHandler(void);
void WEAK QUADSPI_IRQHandler(void);
void WEAK LPTIM1_IRQHandler(void);
void WEAK CEC_IRQHandler(void);
void WEAK I2C4_EV_IRQHandler(void);
void WEAK I2C4_ER_IRQHandler(void);
void WEAK SPDIF_RX_IRQHandler(void);
//------------------------------------------------------------------------------
__attribute__ ((section(".isr_vector"))) void (* const g_pfnVectors[])(void) = {
	(void (*)(void))((unsigned long)&_estack),
	Reset_Handler,

	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
  
  /* External Interrupts */
	WWDG_IRQHandler,					//Window WatchDog              */
	PVD_IRQHandler,						//PVD through EXTI Line detection */
	TAMP_STAMP_IRQHandler,				//* Tamper and TimeStamps through the EXTI line */
	RTC_WKUP_IRQHandler,				//* RTC Wakeup through the EXTI line */
	FLASH_IRQHandler,					//* FLASH                        */
	RCC_IRQHandler,						//* RCC                          */
	EXTI0_IRQHandler,                  //* EXTI Line0                   */
	EXTI1_IRQHandler,                  //* EXTI Line1                   */
	EXTI2_IRQHandler,                  /* EXTI Line2                   */                          
	EXTI3_IRQHandler,                  /* EXTI Line3                   */                          
	EXTI4_IRQHandler,                  /* EXTI Line4                   */                          
	DMA1_Stream0_IRQHandler,           /* DMA1 Stream 0                */                  
	DMA1_Stream1_IRQHandler,           /* DMA1 Stream 1                */                   
	DMA1_Stream2_IRQHandler,           /* DMA1 Stream 2                */                   
	DMA1_Stream3_IRQHandler,           /* DMA1 Stream 3                */                   
	DMA1_Stream4_IRQHandler,           /* DMA1 Stream 4                */                   
	DMA1_Stream5_IRQHandler,           /* DMA1 Stream 5                */                   
	DMA1_Stream6_IRQHandler,           /* DMA1 Stream 6                */                   
	ADC_IRQHandler,                    /* ADC1, ADC2 and ADC3s         */                   
	CAN1_TX_IRQHandler,                /* CAN1 TX                      */                         
	CAN1_RX0_IRQHandler,               /* CAN1 RX0                     */                          
	CAN1_RX1_IRQHandler,               /* CAN1 RX1                     */                          
	CAN1_SCE_IRQHandler,               /* CAN1 SCE                     */                          
	EXTI9_5_IRQHandler,                /* External Line[9:5]s          */                          
	TIM1_BRK_TIM9_IRQHandler,          /* TIM1 Break and TIM9          */         
	TIM1_UP_TIM10_IRQHandler,          /* TIM1 Update and TIM10        */         
	TIM1_TRG_COM_TIM11_IRQHandler,     /* TIM1 Trigger and Commutation and TIM11 */
	TIM1_CC_IRQHandler,                /* TIM1 Capture Compare         */                          
	TIM2_IRQHandler,                   /* TIM2                         */                   
	TIM3_IRQHandler,                   /* TIM3                         */                   
	TIM4_IRQHandler,                   /* TIM4                         */                   
	I2C1_EV_IRQHandler,                /* I2C1 Event                   */                          
	I2C1_ER_IRQHandler,                /* I2C1 Error                   */                          
	I2C2_EV_IRQHandler,                /* I2C2 Event                   */                          
	I2C2_ER_IRQHandler,                /* I2C2 Error                   */                            
	SPI1_IRQHandler,                   /* SPI1                         */                   
	SPI2_IRQHandler,                   /* SPI2                         */                   
	USART1_IRQHandler,                 /* USART1                       */                   
	USART2_IRQHandler,                 /* USART2                       */                   
	USART3_IRQHandler,                 /* USART3                       */                   
	EXTI15_10_IRQHandler,              /* External Line[15:10]s        */                          
	RTC_Alarm_IRQHandler,              /* RTC Alarm (A and B) through EXTI Line */                 
	OTG_FS_WKUP_IRQHandler,            /* USB OTG FS Wakeup through EXTI line */                       
	TIM8_BRK_TIM12_IRQHandler,         /* TIM8 Break and TIM12         */         
	TIM8_UP_TIM13_IRQHandler,          /* TIM8 Update and TIM13        */         
	TIM8_TRG_COM_TIM14_IRQHandler,     /* TIM8 Trigger and Commutation and TIM14 */
	TIM8_CC_IRQHandler,                /* TIM8 Capture Compare         */                          
	DMA1_Stream7_IRQHandler,           /* DMA1 Stream7                 */                          
	FMC_IRQHandler,                    /* FMC                          */                   
	SDMMC1_IRQHandler,                 /* SDMMC1                       */                   
	TIM5_IRQHandler,                   /* TIM5                         */                   
	SPI3_IRQHandler,                   /* SPI3                         */                   
	UART4_IRQHandler,                  /* UART4                        */                   
	UART5_IRQHandler,                  /* UART5                        */                   
	TIM6_DAC_IRQHandler,               /* TIM6 and DAC1&2 underrun errors */                   
	TIM7_IRQHandler,                   /* TIM7                         */
	DMA2_Stream0_IRQHandler,           /* DMA2 Stream 0                */                   
	DMA2_Stream1_IRQHandler,           /* DMA2 Stream 1                */                   
	DMA2_Stream2_IRQHandler,           /* DMA2 Stream 2                */                   
	DMA2_Stream3_IRQHandler,           /* DMA2 Stream 3                */                   
	DMA2_Stream4_IRQHandler,           /* DMA2 Stream 4                */                   
	ETH_IRQHandler,                    /* Ethernet                     */                   
	ETH_WKUP_IRQHandler,               /* Ethernet Wakeup through EXTI line */                     
	CAN2_TX_IRQHandler,                /* CAN2 TX                      */                          
	CAN2_RX0_IRQHandler,               /* CAN2 RX0                     */                          
	CAN2_RX1_IRQHandler,               /* CAN2 RX1                     */                          
	CAN2_SCE_IRQHandler,               /* CAN2 SCE                     */                          
	OTG_FS_IRQHandler,                 /* USB OTG FS                   */                   
	DMA2_Stream5_IRQHandler,           /* DMA2 Stream 5                */                   
	DMA2_Stream6_IRQHandler,           /* DMA2 Stream 6                */                   
	DMA2_Stream7_IRQHandler,           /* DMA2 Stream 7                */                   
	USART6_IRQHandler,                 /* USART6                       */                    
	I2C3_EV_IRQHandler,                /* I2C3 event                   */                          
	I2C3_ER_IRQHandler,                /* I2C3 error                   */                          
	OTG_HS_EP1_OUT_IRQHandler,         /* USB OTG HS End Point 1 Out   */                   
	OTG_HS_EP1_IN_IRQHandler,          /* USB OTG HS End Point 1 In    */                   
	OTG_HS_WKUP_IRQHandler,            /* USB OTG HS Wakeup through EXTI */                         
	OTG_HS_IRQHandler,                 /* USB OTG HS                   */                   
	DCMI_IRQHandler,                   /* DCMI                         */                   
	0,                               /* Reserved                     */                   
	RNG_IRQHandler,                    /* Rng                          */
	FPU_IRQHandler,                    /* FPU                          */
	UART7_IRQHandler,                  /* UART7                        */      
	UART8_IRQHandler,                  /* UART8                        */
	SPI4_IRQHandler,                   /* SPI4                         */
	SPI5_IRQHandler,                   /* SPI5                           */
	SPI6_IRQHandler,                   /* SPI6                         */
	SAI1_IRQHandler,                   /* SAI1                          */
	LTDC_IRQHandler,                   /* LTDC                          */
	LTDC_ER_IRQHandler,                /* LTDC error                      */
	DMA2D_IRQHandler,                  /* DMA2D                          */
	SAI2_IRQHandler,                   /* SAI2                         */
	QUADSPI_IRQHandler,                /* QUADSPI                      */
	LPTIM1_IRQHandler,                 /* LPTIM1                       */
	CEC_IRQHandler,                    /* HDMI_CEC                     */
	I2C4_EV_IRQHandler,                /* I2C4 Event                   */
	I2C4_ER_IRQHandler,                /* I2C4 Error                   */
	SPDIF_RX_IRQHandler,               /* SPDIF_RX                     */  
};
//------------------------------------------------------------------------------
