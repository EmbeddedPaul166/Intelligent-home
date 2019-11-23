#include "main.h"

int main()
{
    setupHardware();
    HAL_Delay(1000);
    while (1)
    {
        if (uartRxBuffer == 0x74)
        {
            transmitTemperatureRead();
        }
        else if (uartRxBuffer == 0x6C) 
        {
            transmitLightIntensityRead();
        } 
        else if (uartRxBuffer == 0x73) 
        {
            transmitSoundIntensityRead();
        }
        
        convertSensorMeasurements();
        
        if (tresholdDirection == 0)
        {
            setTemperatureTreshold();
        }
        else if (tresholdDirection == 1)
        {
            setLightIntensityTreshold();
        }
        else if (tresholdDirection == 2)
        {
            setSoundIntensityTreshold();
        }
        
        if (temperatureInCelsius > temperatureInCelsiusTreshold + 1.0)
        {
            coolingOn();
        }
        else if (temperatureInCelsius < temperatureInCelsiusTreshold - 1.0)
        {
            heatingOn();
        }
        else
        {
            temperatureRegulationOff();
        }
        
        if (lightIntensityPercentege < lightIntensityPercentegeTreshold)
        {
            lightsOn();
        }
        else
        {
            lightsOff();
        }
        
        if (soundIntensityPercentege < soundIntensityPercentegeTreshold)
        {
            alarmOff();
        }
        else
        {
            alarmOn();
        }
    }
    return 0;
}


void convertSensorMeasurements(void)
{
    float resistance = (float)(MAXIMUM_ADC_VALUE-(float)temperatureRead)*10000.0/(float)temperatureRead;
    temperatureInCelsius = 1.0/(logf(resistance/10000.0)/3975.0+1.0/298.15)-273.15;
    lightIntensityPercentege = ((float)lightIntensityRead/MAXIMUM_ADC_VALUE)*100.0;
    soundIntensityPercentege = ((float)soundIntensityRead/MAXIMUM_ADC_VALUE)*100.0;
}

void setTemperatureTreshold(void)
{
    float resistance = (float)(MAXIMUM_ADC_VALUE-(float)tresholdRead)*10000.0/(float)tresholdRead;
    temperatureInCelsiusTreshold = 1.0/(logf(resistance/10000.0)/3975.0+1.0/298.15)-273.15;
}

void setLightIntensityTreshold(void)
{
    lightIntensityPercentegeTreshold = ((float)tresholdRead/MAXIMUM_ADC_VALUE)*100.0;
}

void setSoundIntensityTreshold(void)
{
    soundIntensityPercentegeTreshold = ((float)tresholdRead/MAXIMUM_ADC_VALUE)*100.0;
}

void heatingOn(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}

void coolingOn(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
}

void temperatureRegulationOff(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
    HAL_Delay(300);
}

void lightsOn(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

void lightsOff(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}


void alarmOn(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}


void alarmOff(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}

void transmitTemperatureRead()
{
    uartRxBuffer = 0;
    uartTxBuffer[0] =  (uint8_t)(temperatureRead & 0xFF); //*((unsigned char*) & temperatureRead);
    uartTxBuffer[1] =  (uint8_t)((temperatureRead >> 8) & 0xFF); //*((unsigned char*)((&temperatureRead)+1));
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

void transmitLightIntensityRead()
{
    uartRxBuffer = 0;
    uartTxBuffer[0] =   (uint8_t)(lightIntensityRead & 0xFF);
    uartTxBuffer[1] =   (uint8_t)((lightIntensityRead >> 8) & 0xFF);
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

void transmitSoundIntensityRead()
{
    uartRxBuffer = 0;
    uartTxBuffer[0] =   (uint8_t)(soundIntensityRead & 0xFF);
    uartTxBuffer[1] =   (uint8_t)((soundIntensityRead >> 8) & 0xFF);
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

void setupHardware(void)
{
    HAL_Init();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
    errorHandlerSetup();
    systemClockConfig();
    adcConfig();
    usartSetup();
    gpioSetup();
    timerSetup();
}

void timerSetup(void)
{
    timer3Handle.Instance = TIM3;
    //25 MHz clock source, 100 ms count
    timer3Handle.Init.Prescaler = 2500 - 1;
    timer3Handle.Init.Period = 1000 - 1;
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


void gpioSetup(void)
{ 
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
    channelConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
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

void errorHandler(void)
{
    while(1)
    {
        BSP_LED_Toggle(LED2);
        HAL_Delay(500);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    temperatureRead = adcRead[0]; 
    lightIntensityRead = adcRead[1]; 
    soundIntensityRead = adcRead[2]; 
    tresholdRead = adcRead[3];
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    errorHandler();
}


