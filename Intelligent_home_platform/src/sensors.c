#include <math.h>
#include "sensors.h"
#include "actuators.h"
#include "globals.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

volatile uint32_t temperatureRead;
volatile uint32_t lightIntensityRead;
volatile uint32_t soundIntensityRead;
volatile uint32_t tresholdRead;
volatile uint8_t readingsDone;

volatile float temperatureInCelsius;
volatile float lightIntensityPercentege;
volatile float soundIntensityPercentege;

volatile float temperatureInCelsiusTreshold;
volatile float lightIntensityPercentegeTreshold;
volatile float soundIntensityPercentegeTreshold;

uint32_t adcRead[ADC_BUFFER_SIZE];

void convertSensorMeasurements(void)
{
    float resistance = (float)(MAXIMUM_ADC_VALUE-(float)temperatureRead)*10000.0/(float)temperatureRead;
    temperatureInCelsius = 1.0/(logf(resistance/10000.0)/3975.0+1.0/298.15)-273.15;
    lightIntensityPercentege = ((float)lightIntensityRead/MAXIMUM_ADC_VALUE)*100.0;
    soundIntensityPercentege = ((float)soundIntensityRead/MAXIMUM_ADC_VALUE)*100.0;
}

void setTemperatureTreshold(void)
{
    temperatureInCelsiusTreshold =((float)tresholdRead/MAXIMUM_ADC_VALUE)*30.0+10; 
}

void setLightIntensityTreshold(void)
{
    lightIntensityPercentegeTreshold = ((float)tresholdRead/MAXIMUM_ADC_VALUE)*100.0;
}

void setSoundIntensityTreshold(void)
{
    soundIntensityPercentegeTreshold = ((float)tresholdRead/MAXIMUM_ADC_VALUE)*100.0;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (!readingsDone)
    {
        temperatureRead = adcRead[0]; 
        lightIntensityRead = adcRead[1]; 
        soundIntensityRead = adcRead[2]; 
        tresholdRead = adcRead[3];
        readingsDone = 1;
    }
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    errorHandler();
}

