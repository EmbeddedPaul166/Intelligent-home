#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

#define MAXIMUM_ADC_VALUE 4095.0
#define ADC_BUFFER_SIZE 4
#define UART_TX_BUFFER_SIZE 2
#define TEMPERATURE_REQUEST 0x74
#define LIGHT_INTENSITY_REQUEST 0x6C
#define SOUND_INTENSITY_REQUEST 0x73

extern volatile uint32_t temperatureRead;
extern volatile uint32_t lightIntensityRead;
extern volatile uint32_t soundIntensityRead;
extern volatile uint32_t tresholdRead;
extern volatile int8_t tresholdDirection;

extern volatile float temperatureInCelsius;
extern volatile float lightIntensityPercentege;
extern volatile float soundIntensityPercentege;

extern volatile float temperatureInCelsiusTreshold;
extern volatile float lightIntensityPercentegeTreshold;
extern volatile float soundIntensityPercentegeTreshold;

extern uint32_t adcRead[ADC_BUFFER_SIZE];
extern uint32_t uartTxBuffer[UART_TX_BUFFER_SIZE];
extern uint8_t uartRxBuffer;

extern ADC_HandleTypeDef adcHandle;
extern UART_HandleTypeDef uartHandle;
extern DMA_HandleTypeDef dmaUart2HandleRx;
extern DMA_HandleTypeDef dmaUart2HandleTx;
extern TIM_HandleTypeDef timer3Handle;

#endif /*__GLOBALS_H*/
