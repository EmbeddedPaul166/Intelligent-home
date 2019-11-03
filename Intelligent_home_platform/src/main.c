#include "main.h"

static void setupHardware(void);
static void systemClockConfig(void);
static void adcConfig(void);
static void usartSetup(void);
static void errorHandlerSetup(void);
static void errorHandler(void);
static void transmitTemperatureRead(void);
static void transmitLightIntensityRead(void);
static void transmitSoundIntensityRead(void);

static uint32_t adcRead[ADC_BUFFER_SIZE];
static uint32_t uartTxBuffer[UART_TX_BUFFER_SIZE];
static uint8_t uartRxBuffer;

int main()
{
    setupHardware();    
    while (1)
    {
        if (uartRxBuffer == 0x74)
        {
            uartRxBuffer = 0;
            transmitTemperatureRead();
        }
        else if (uartRxBuffer == 0x6C) 
        {
            uartRxBuffer = 0;
            transmitLightIntensityRead();
        } 
        else if (uartRxBuffer == 0x73) 
        {
            uartRxBuffer = 0;
            transmitSoundIntensityRead();
        }
        //temperatureInCelsius = 1.0/(log(1023.0/(float)temperatureRead-1.0)/4275.0+1.0/298.15)-273.15;
        //lightIntensityInLux = exp((float)lightIntensityRead/80.0);
        //soundIntensityInDB = 20.0*log(lightIntensityRead);
    }
    return 0;
}


static void transmitTemperatureRead()
{
    uartTxBuffer[0] =  (uint8_t)(temperatureRead & 0xFF); //*((unsigned char*) & temperatureRead);
    uartTxBuffer[1] =  (uint8_t)((temperatureRead >> 8) & 0xFF); //*((unsigned char*)((&temperatureRead)+1));
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

static void transmitLightIntensityRead()
{
    uartTxBuffer[0] =   (uint8_t)(lightIntensityRead & 0xFF);
    uartTxBuffer[1] =   (uint8_t)((lightIntensityRead >> 8) & 0xFF);
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

static void transmitSoundIntensityRead()
{
    uartTxBuffer[0] =   (uint8_t)(soundIntensityRead & 0xFF);
    uartTxBuffer[1] =   (uint8_t)((soundIntensityRead >> 8) & 0xFF);
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

static void setupHardware(void)
{
    HAL_Init();
    errorHandlerSetup();
    systemClockConfig();
    adcConfig();
    usartSetup();
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
    
    if (HAL_ADC_Start_DMA(&adcHandle, (uint32_t*)adcRead, ADC_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

static void usartSetup(void)
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


