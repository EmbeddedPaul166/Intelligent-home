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
volatile uint8_t measurementsDone;

volatile float temperatureInCelsius;
volatile float lightIntensityPercentege;
volatile float soundIntensityPercentege;

volatile float temperatureInCelsiusTreshold;
volatile float lightIntensityPercentegeTreshold;
volatile float soundIntensityPercentegeTreshold;

volatile uint32_t adcRead[ADC_BUFFER_SIZE];
volatile uint32_t readCount;

TIM_HandleTypeDef timer3Handle;

void convertSensorMeasurements(void)
{
    float resistance = (float)(MAXIMUM_ADC_VALUE-(float)adcReadAverage[0])*10000.0/(float)adcReadAverage[0];
    temperatureInCelsius = 1.0/(logf(resistance/10000.0)/3975.0+1.0/298.15)-273.15;
    lightIntensityPercentege = ((float)adcReadAverage[1]/MAXIMUM_ADC_VALUE)*100.0;
    soundIntensityPercentege = ((float)adcReadAverage[2]/MAXIMUM_ADC_VALUE)*100.0;
}

void setTemperatureTreshold(void)
{
    temperatureInCelsiusTreshold =((float)adcReadAverage[3]/MAXIMUM_ADC_VALUE)*30.0+10; 
}

void setLightIntensityTreshold(void)
{
    lightIntensityPercentegeTreshold = ((float)adcReadAverage[3]/MAXIMUM_ADC_VALUE)*100.0;
}

void setSoundIntensityTreshold(void)
{
    soundIntensityPercentegeTreshold = ((float)adcReadAverage[3]/MAXIMUM_ADC_VALUE)*100.0;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (readCount != NUMBER_OF_READS)
    {
        temperatureRead += adcRead[0]; 
        lightIntensityRead += adcRead[1]; 
        soundIntensityRead += adcRead[2]; 
        tresholdRead += adcRead[3];
        readCount++;
    }
    else
    { 
        adcReadAverage[0] = temperatureRead/(float)NUMBER_OF_READS;
        adcReadAverage[1] = lightIntensityRead/(float)NUMBER_OF_READS;
        adcReadAverage[2] = soundIntensityRead/(float)NUMBER_OF_READS;
        adcReadAverage[3] = tresholdRead/(float)NUMBER_OF_READS;
        
        temperatureRead = 0;
        lightIntensityRead = 0;
        soundIntensityRead = 0;
        tresholdRead = 0;
        
        measurementsDone = 1;
        readCount = 0;
    }
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    errorHandler();
}

