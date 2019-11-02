#ifndef MAIN
#define MAIN

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

#define ADC_BUFFER_SIZE 3

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

#endif /*MAIN*/
