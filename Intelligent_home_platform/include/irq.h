#ifndef IRQ
#define IRQ

#include "main.h"

extern ADC_HandleTypeDef adcHandle;

void SysTick_Handler(void);
void DMA1_Channel1_IRQHandler(void);
void ADC1_2_IRQHandler(void);


#endif /*IRQ*/
