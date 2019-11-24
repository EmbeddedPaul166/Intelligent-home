#include "irq.h"
#include "sensors.h"
#include "actuators.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"
#include "globals.h"

volatile int8_t tresholdDirection;
volatile uint8_t readingsDone;

ADC_HandleTypeDef adcHandle;
UART_HandleTypeDef uartHandle;
DMA_HandleTypeDef dmaUart2HandleRx;
DMA_HandleTypeDef dmaUart2HandleTx;
TIM_HandleTypeDef timer3Handle;

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(adcHandle.DMA_Handle);
}

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&uartHandle);
}

void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&dmaUart2HandleRx);
}

void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&dmaUart2HandleTx);
}

void EXTI4_IRQHandler(void)
{
    if (tresholdDirection < 2 && tresholdDirection >= 0)
    {
        tresholdDirection++;
    }
    else
    {
        tresholdDirection = 0;
    }
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void TIM3_IRQHandler(void)
{   
    if (readingsDone)
    {
        convertSensorMeasurements();
        setTresholdValue();    
        handleRegulation();
        readingsDone = 0;
    }
    HAL_TIM_IRQHandler(&timer3Handle);
}

