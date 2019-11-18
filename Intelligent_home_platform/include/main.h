#ifndef MAIN
#define MAIN

#include <stdint.h>
#include <math.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

#define MAXIMUM_ADC_VALUE 4095.0
#define ADC_BUFFER_SIZE 4
#define UART_TX_BUFFER_SIZE 2

/* Pinout:
 * A0 - temperature sensor
 * A1 - light sensor
 * A2 - sound sensor
 * A3 - potentiometer
 * D5 - button
 * D6 - red LED
 * D8 - blue LED
 * D4 - green LED
 */

void setupHardware(void);
void systemClockConfig(void);
void adcConfig(void);
void usartSetup(void);
void gpioSetup(void);
void errorHandlerSetup(void);
void errorHandler(void);
void transmitTemperatureRead(void);
void transmitLightIntensityRead(void);
void transmitSoundIntensityRead(void);
void convertSensorMeasurements(void);
void setTemperatureTreshold(void);
void setLightIntensityTreshold(void);
void setSoundIntensityTreshold(void);
void heatingOn(void);
void coolingOn(void);
void temperatureRegulationOff(void);
void lightsOn(void);
void lightsOff(void);
void alarmOn(void);
void alarmOff(void);

volatile uint32_t temperatureRead;
volatile uint32_t lightIntensityRead;
volatile uint32_t soundIntensityRead;
volatile uint32_t tresholdRead;
volatile int8_t tresholdDirection;

volatile float temperatureInCelsius;
volatile float lightIntensityPercentege;
volatile float soundIntensityPercentege;

volatile float temperatureInCelsiusTreshold;
volatile float lightIntensityPercentegeTreshold;
volatile float soundIntensityPercentegeTreshold;

uint32_t adcRead[ADC_BUFFER_SIZE];
uint32_t uartTxBuffer[UART_TX_BUFFER_SIZE];
uint8_t uartRxBuffer;

ADC_HandleTypeDef adcHandle;
UART_HandleTypeDef uartHandle;
DMA_HandleTypeDef dmaUart2HandleRx;
DMA_HandleTypeDef dmaUart2HandleTx;


#endif /*MAIN*/
