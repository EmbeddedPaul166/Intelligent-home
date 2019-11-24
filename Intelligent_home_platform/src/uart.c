#include "uart.h"
#include "actuators.h"
#include "globals.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

DMA_HandleTypeDef dmaUart2HandleRx;
DMA_HandleTypeDef dmaUart2HandleTx;
uint32_t uartTxBuffer[UART_TX_BUFFER_SIZE];
uint8_t uartRxBuffer;

void transmitTemperatureRead()
{
    uartRxBuffer = 0;
    uartTxBuffer[0] =  (uint8_t)(temperatureRead & 0xFF);
    uartTxBuffer[1] =  (uint8_t)((temperatureRead >> 8) & 0xFF); 
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

void transmitLightIntensityRead()
{
    uartRxBuffer = 0;
    uartTxBuffer[0] =   (uint8_t)(lightIntensityRead & 0xFF);
    uartTxBuffer[1] =   (uint8_t)((lightIntensityRead >> 8) & 0xFF);
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

void transmitSoundIntensityRead()
{
    uartRxBuffer = 0;
    uartTxBuffer[0] =   (uint8_t)(soundIntensityRead & 0xFF);
    uartTxBuffer[1] =   (uint8_t)((soundIntensityRead >> 8) & 0xFF);
    if (HAL_UART_Transmit_DMA(&uartHandle, (uint8_t*)uartTxBuffer, UART_TX_BUFFER_SIZE) != HAL_OK)
    {
        errorHandler();
    }
}

