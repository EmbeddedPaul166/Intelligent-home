#include "hardware.h"
#include "actuators.h"
#include "globals.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

ADC_HandleTypeDef adcHandle;
UART_HandleTypeDef uartHandle;
DMA_HandleTypeDef dmaUart2HandleRx;
DMA_HandleTypeDef dmaUart2HandleTx;
TIM_HandleTypeDef timer3Handle;
volatile uint32_t adcRead[ADC_BUFFER_SIZE];

void setupHardware(void)
{
    HAL_Init();
    systemClockConfig();
    gpioSetup();
    errorHandlerSetup();
    adcConfig();
    timerSetup();
    usartSetup();
    HAL_Delay(100);
}

void systemClockConfig(void)
{
    RCC_ClkInitTypeDef clkInitStruct;
    RCC_OscInitTypeDef oscillatorInitStruct;

    oscillatorInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    oscillatorInitStruct.HSEState        = RCC_HSE_ON;
    oscillatorInitStruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
    oscillatorInitStruct.PLL.PLLState    = RCC_PLL_ON;
    oscillatorInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
    oscillatorInitStruct.PLL.PLLMUL      = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&oscillatorInitStruct)!= HAL_OK)
    {
        errorHandler(); 
    }

    clkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    clkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    clkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
    if (HAL_RCC_ClockConfig(&clkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
    {
        errorHandler();
    }
}

void gpioSetup(void)
{ 
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitTypeDef gpioInitStruct;
    gpioInitStruct.Pin = GPIO_PIN_9;
    gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pull = GPIO_NOPULL;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpioInitStruct);
    
    gpioInitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_5;
    gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pull = GPIO_NOPULL;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpioInitStruct);
    
    gpioInitStruct.Pin = GPIO_PIN_4;
    gpioInitStruct.Mode = GPIO_MODE_IT_RISING;
    gpioInitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &gpioInitStruct);
    
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0); 
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_5, GPIO_PIN_RESET);
}

void errorHandlerSetup(void)
{
     GPIO_InitTypeDef  gpioInitStruct;
    
    LED2_GPIO_CLK_ENABLE();
    gpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pull  = GPIO_PULLUP;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInitStruct.Pin = LED2_PIN;
    HAL_GPIO_Init(LED2_GPIO_PORT, &gpioInitStruct); 
}

void adcConfig(void)
{
    ADC_ChannelConfTypeDef channelConfig;

    adcHandle.Instance = ADC1; 
    adcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adcHandle.Init.ScanConvMode = ADC_SCAN_ENABLE;              
    adcHandle.Init.ContinuousConvMode = ENABLE;                       
    adcHandle.Init.NbrOfConversion = 4;                             
    adcHandle.Init.DiscontinuousConvMode = DISABLE;                                                    
    adcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    if (HAL_ADC_Init(&adcHandle) != HAL_OK)
    {
        errorHandler();
    }

    channelConfig.Channel      = ADC_CHANNEL_0; //A0
    channelConfig.Rank         = ADC_REGULAR_RANK_1;
    channelConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channelConfig) != HAL_OK)
    {
        errorHandler();
    }
    
    channelConfig.Channel      = ADC_CHANNEL_1; //A1
    channelConfig.Rank         = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channelConfig) != HAL_OK)
    {
        errorHandler();
    }

    channelConfig.Channel      = ADC_CHANNEL_4; //A2
    channelConfig.Rank         = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channelConfig) != HAL_OK)
    {
        errorHandler();
    }
    
    channelConfig.Channel      = ADC_CHANNEL_8; //A3
    channelConfig.Rank         = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channelConfig) != HAL_OK)
    {
        errorHandler();
    }   
    
    if (HAL_ADC_Start_DMA(&adcHandle, (uint32_t*)adcRead, ADC_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

void timerSetup(void)
{
    timer3Handle.Instance = TIM3;
    timer3Handle.Init.Prescaler = 64000 - 1;
    timer3Handle.Init.Period = 50 - 1;
    timer3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer3Handle.Init.RepetitionCounter = 0;
    timer3Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&timer3Handle) != HAL_OK)
    {
        errorHandler();
    }
    if (HAL_TIM_Base_Start_IT(&timer3Handle) != HAL_OK)
    {
        errorHandler();
    }
}

void usartSetup(void)
{
    uartHandle.Instance = USART2;
    uartHandle.Init.BaudRate = 115200;
    uartHandle.Init.WordLength = UART_WORDLENGTH_9B;
    uartHandle.Init.StopBits = UART_STOPBITS_1;
    uartHandle.Init.Parity = UART_PARITY_NONE;
    uartHandle.Init.Mode = UART_MODE_TX_RX;
    uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&uartHandle) != HAL_OK)
    {
        errorHandler();
    }
    if (HAL_UART_Receive_DMA(&uartHandle, &uartRxBuffer, 1) != HAL_OK)
    {
        errorHandler();
    }
}

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
        
        HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 1, 1); 
        HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
        HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 1, 1); 
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

