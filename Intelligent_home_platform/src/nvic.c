#include "nvic.h"
#include "main.h"

void SysTick_Handler(void)
{
    HAL_IncTick();
}



