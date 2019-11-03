#ifndef MAIN
#define MAIN

#include <stdint.h>
#include <math.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

#define ADC_BUFFER_SIZE 3
#define UART_TX_BUFFER_SIZE 2

volatile uint32_t temperatureRead;
volatile uint32_t lightIntensityRead;
volatile uint32_t soundIntensityRead;

volatile float temperatureInCelsius;
volatile float lightIntensityInLux;
volatile float soundIntensityInDB;

ADC_HandleTypeDef adcHandle;
UART_HandleTypeDef uartHandle;
DMA_HandleTypeDef dmaUart2HandleRx;
DMA_HandleTypeDef dmaUart2HandleTx;


#endif /*MAIN*/
