#include "actuators.h"
#include "sensors.h"
#include "uart.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"
#include "globals.h"

volatile float temperatureInCelsius;
volatile float lightIntensityPercentege;
volatile float soundIntensityPercentege;

volatile float temperatureInCelsiusTreshold;
volatile float lightIntensityPercentegeTreshold;
volatile float soundIntensityPercentegeTreshold;

void handleRegulation(void)
{
    if (temperatureInCelsius > temperatureInCelsiusTreshold + 1.0)
    {
        coolingOn();
    }
    else if (temperatureInCelsius < temperatureInCelsiusTreshold - 1.0)
    {
        heatingOn();
    }
    else
    {
        temperatureRegulationOff();
    }
    
    if (lightIntensityPercentege < lightIntensityPercentegeTreshold)
    {
        lightsOn();
    }
    else
    {
        lightsOff();
    }
    
    if (soundIntensityPercentege < soundIntensityPercentegeTreshold)
    {
        alarmOff();
    }
    else
    {
        alarmOn();
    }
}

void handleUART(void)
{
    if (uartRxBuffer == TEMPERATURE_REQUEST)
    {
        transmitTemperatureRead();
    }
    else if (uartRxBuffer == LIGHT_INTENSITY_REQUEST) 
    {
        transmitLightIntensityRead();
    } 
    else if (uartRxBuffer == SOUND_INTENSITY_REQUEST) 
    {
        transmitSoundIntensityRead();
    }
}

void heatingOn(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}

void coolingOn(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
}

void temperatureRegulationOff(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
}

void lightsOn(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

void lightsOff(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}


void alarmOn(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

void alarmOff(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}

void setTresholdValue(void)
{ 
    if (tresholdDirection == 0)
    {
        setTemperatureTreshold();
    }
    else if (tresholdDirection == 1)
    {
        setLightIntensityTreshold();
    }
    else if (tresholdDirection == 2)
    {
        setSoundIntensityTreshold();
    }
}

void errorHandler(void)
{
    while(1)
    {
        BSP_LED_Toggle(LED2);
        HAL_Delay(500);
    }
}


