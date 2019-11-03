#include "main.h"

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    static DMA_HandleTypeDef dmaHandle;
    GPIO_InitTypeDef gpioInitStruct;
    RCC_PeriphCLKInitTypeDef peripheralClkInit;
    
    __HAL_RCC_ADC1_CLK_ENABLE();
    peripheralClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    peripheralClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    HAL_RCCEx_PeriphCLKConfig(&peripheralClkInit); 
    
    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    gpioInitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4;
    gpioInitStruct.Mode = GPIO_MODE_ANALOG;
    //gpioInitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpioInitStruct);
    
    dmaHandle.Instance = DMA1_Channel1;
    dmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    dmaHandle.Init.MemInc = DMA_MINC_ENABLE;
    dmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;   
    dmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;   
    dmaHandle.Init.Mode = DMA_CIRCULAR;              
    dmaHandle.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&dmaHandle);
    __HAL_LINKDMA(hadc, DMA_Handle, dmaHandle);

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}
