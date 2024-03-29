#ifndef __IRQ_H
#define __IRQ_H

#include "main.h"

void SysTick_Handler(void);
void DMA1_Channel1_IRQHandler(void);
void USART2_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif /*__IRQ_H*/
