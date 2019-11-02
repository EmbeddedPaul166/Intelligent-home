#include "main.h"

static void setupHardware(void);
static void systemClockConfig(void);
static void adcConfig(void);
static void errorHandlerSetup(void);
static void errorHandler(void);

static volatile int adcRead[ADC_BUFFER_SIZE];
static volatile int temperatureRead;
static volatile int lightIntensityRead;
static volatile int soundIntensityRead;

ADC_HandleTypeDef adcHandle;

int main()
{
    setupHardware();    
    while (1)
    {
        
    }
    return 0;
}

static void setupHardware(void)
{
    HAL_Init();
    errorHandlerSetup();
    systemClockConfig();    
    adcConfig();
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
    ADC_ChannelConfTypeDef channelConfig;

    adcHandle.Instance = ADC1; 
    adcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adcHandle.Init.ScanConvMode = ADC_SCAN_ENABLE;              
    adcHandle.Init.ContinuousConvMode = ENABLE;                       
    adcHandle.Init.NbrOfConversion = 3;                             
    adcHandle.Init.DiscontinuousConvMode = DISABLE;                                                    
    adcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    if (HAL_ADC_Init(&adcHandle) != HAL_OK)
    {
        errorHandler();
    }

    channelConfig.Channel      = ADC_CHANNEL_0; //A0
    channelConfig.Rank         = ADC_REGULAR_RANK_1;
    channelConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channelConfig) != HAL_OK)
    {
        errorHandler();
    }
    
    channelConfig.Channel      = ADC_CHANNEL_1; //A1
    channelConfig.Rank         = ADC_REGULAR_RANK_2;
    channelConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channelConfig) != HAL_OK)
    {
        errorHandler();
    }

    channelConfig.Channel      = ADC_CHANNEL_4; //A2
    channelConfig.Rank         = ADC_REGULAR_RANK_3;
    channelConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
    if (HAL_ADC_ConfigChannel(&adcHandle, &channelConfig) != HAL_OK)
    {
        errorHandler();
    }
    
    if (HAL_ADC_Start_DMA(&adcHandle, (uint32_t *)adcRead, ADC_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
    
    if (HAL_ADC_Start(&adcHandle) != HAL_OK)
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

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    temperatureRead = adcRead[0]; 
    lightIntensityRead = adcRead[1]; 
    soundIntensityRead = adcRead[2];
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    errorHandler();
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    
}


