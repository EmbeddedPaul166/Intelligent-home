#include "main.h"

void HAL_MspInit(void)
{
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_NOJTAG();
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    static DMA_HandleTypeDef dmaHandle;
    GPIO_InitTypeDef gpioInitStruct = {0};
    RCC_PeriphCLKInitTypeDef peripheralClkInit;
    
    __HAL_RCC_ADC1_CLK_ENABLE();
    peripheralClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    peripheralClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    HAL_RCCEx_PeriphCLKConfig(&peripheralClkInit); 
    
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    gpioInitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4; //A0, A1, A2
    gpioInitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &gpioInitStruct);
    
    gpioInitStruct.Pin = GPIO_PIN_0; //A3
    gpioInitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOB, &gpioInitStruct);
    
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

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        __HAL_RCC_ADC1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
        HAL_DMA_DeInit(hadc->DMA_Handle);
    }
}



void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef gpioInitStruct = {0};
    if (huart->Instance == USART2)
    {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        
        gpioInitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        gpioInitStruct.Mode = GPIO_MODE_AF_PP;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &gpioInitStruct);
        
        dmaUart2HandleRx.Instance = DMA1_Channel6;
        dmaUart2HandleRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        dmaUart2HandleRx.Init.PeriphInc = DMA_PINC_DISABLE;
        dmaUart2HandleRx.Init.MemInc = DMA_MINC_ENABLE;
        dmaUart2HandleRx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        dmaUart2HandleRx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        dmaUart2HandleRx.Init.Mode = DMA_CIRCULAR;
        dmaUart2HandleRx.Init.Priority = DMA_PRIORITY_HIGH;
        HAL_DMA_Init(&dmaUart2HandleRx);
        __HAL_LINKDMA(huart,hdmarx,dmaUart2HandleRx);
        
        dmaUart2HandleTx.Instance = DMA1_Channel7;
        dmaUart2HandleTx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        dmaUart2HandleTx.Init.PeriphInc = DMA_PINC_DISABLE;
        dmaUart2HandleTx.Init.MemInc = DMA_MINC_ENABLE;
        dmaUart2HandleTx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        dmaUart2HandleTx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        dmaUart2HandleTx.Init.Mode = DMA_NORMAL;
        dmaUart2HandleTx.Init.Priority = DMA_PRIORITY_HIGH;
        HAL_DMA_Init(&dmaUart2HandleTx);
        __HAL_LINKDMA(huart,hdmatx,dmaUart2HandleTx);
        
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0); 
        HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
        HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0); 
        HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance == USART2)
    {
        __HAL_RCC_USART2_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
        HAL_DMA_DeInit(huart->hdmarx);
        HAL_DMA_DeInit(huart->hdmatx);
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* timer3Handle_base)
{
    if(timer3Handle_base->Instance==TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* timer3Handle_base)
{
    if(timer3Handle_base->Instance==TIM3)
    {
        __HAL_RCC_TIM3_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM3_IRQn);
    }
}
