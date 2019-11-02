#include "main.h"

int main()
{
    setupHardware();    
    while (1)
    {
        if (HAL_ADC_Start_DMA(&adcHandle, (uint32_t *)adcRead, ADC_BUFFER_SIZE) != HAL_OK)
        {
            errorHandler();
        }
        //Modify below to add calculations of real values
        temperatureRead = adcRead[0]; 
        lightIntensityRead = adcRead[1]; 
        soundIntensityRead = adcRead[2]; 
    }
    return 0;
}

static void setupHardware(void)
{
    HAL_Init();
    systemClockConfig();   
    errorHandlerSetup();
    adcConfig();
    if (HAL_ADC_Start(&adcHandle) != HAL_OK)
    {
        errorHandler();
    }
}

static void errorHandlerSetup(void)
{
    static GPIO_InitTypeDef  gpioInitStruct;
    
    LED2_GPIO_CLK_ENABLE();
    gpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pull  = GPIO_PULLUP;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInitStruct.Pin = LED2_PIN;
    HAL_GPIO_Init(LED2_GPIO_PORT, &gpioInitStruct); 
}

static void systemClockConfig(void)
{
    RCC_ClkInitTypeDef clkInitStruct = {0};
    RCC_OscInitTypeDef oscillatorInitStruct = {0};

    oscillatorInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSI;
    oscillatorInitStruct.HSEState        = RCC_HSE_OFF;
    oscillatorInitStruct.LSEState        = RCC_LSE_OFF;
    oscillatorInitStruct.HSIState        = RCC_HSI_ON;
    oscillatorInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    oscillatorInitStruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
    oscillatorInitStruct.PLL.PLLState    = RCC_PLL_ON;
    oscillatorInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSI_DIV2;
    oscillatorInitStruct.PLL.PLLMUL      = RCC_PLL_MUL16;
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

static void adcConfig(void)
{
    ADC_ChannelConfTypeDef   channel0Config; //A0
    ADC_ChannelConfTypeDef   channel1Config; //A1
    ADC_ChannelConfTypeDef   channel4Config; //A2

    adcHandle.Instance = ADC1;
    
    adcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adcHandle.Init.ScanConvMode = ADC_SCAN_ENABLE;              
    adcHandle.Init.ContinuousConvMode = ENABLE;                       
    adcHandle.Init.NbrOfConversion = 3;                             
    adcHandle.Init.DiscontinuousConvMode = DISABLE;                       
    adcHandle.Init.NbrOfDiscConversion = 0;                             
    adcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    if (HAL_ADC_Init(&adcHandle) != HAL_OK)
    {
        errorHandler();
    }

    channel0Config.Channel      = ADC_CHANNEL_0;
    channel0Config.Rank         = ADC_REGULAR_RANK_1;
    channel0Config.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channel0Config) != HAL_OK)
    {
        errorHandler();
    }
    
    channel1Config.Channel      = ADC_CHANNEL_1;
    channel1Config.Rank         = ADC_REGULAR_RANK_2;
    channel1Config.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channel1Config) != HAL_OK)
    {
        errorHandler();
    }

    channel4Config.Channel      = ADC_CHANNEL_4;
    channel4Config.Rank         = ADC_REGULAR_RANK_3;
    channel4Config.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channel4Config) != HAL_OK)
    {
        errorHandler();
    }
}

static void errorHandler(void)
{
    while(1)
    {
        BSP_LED_Toggle(LED2);
        HAL_Delay(500);
    }
}




