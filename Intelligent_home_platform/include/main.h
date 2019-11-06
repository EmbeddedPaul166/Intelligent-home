#ifndef MAIN
#define MAIN

#include <stdint.h>
#include <math.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

#define MAXIMUM_ADC_VALUE 4095.0
#define ADC_BUFFER_SIZE 4
#define UART_TX_BUFFER_SIZE 2

volatile uint32_t temperatureRead;
volatile uint32_t lightIntensityRead;
volatile uint32_t soundIntensityRead;
volatile uint32_t tresholdRead;
volatile uint8_t tresholdDirection;

volatile float temperatureInCelsius;
volatile float lightIntensityPercentege;
volatile float soundIntensityPercentege;

volatile float temperatureInCelsiusTreshold;
volatile float lightIntensityPercentegeTreshold;
volatile float soundIntensityPercentegeTreshold;

ADC_HandleTypeDef adcHandle;
UART_HandleTypeDef uartHandle;
DMA_HandleTypeDef dmaUart2HandleRx;
DMA_HandleTypeDef dmaUart2HandleTx;


#endif /*MAIN*/
