#include "irq.h"
#include "main.h"

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(adcHandle.DMA_Handle);
}

