#ifndef MAIN
#define MAIN

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

#define ADC_BUFFER_SIZE 3

volatile float temperatureInCelsius;
volatile float lightIntensityInLux;
volatile float soundIntensityInDB;

#endif /*MAIN*/
