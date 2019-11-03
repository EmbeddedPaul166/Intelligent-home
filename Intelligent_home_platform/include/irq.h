#ifndef IRQ
#define IRQ

#include "main.h"

extern ADC_HandleTypeDef adcHandle;

void SysTick_Handler(void);
void DMA1_Channel1_IRQHandler(void);
void USART2_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);

#endif /*IRQ*/
