#ifndef __INTERRUPT_CALLBACK_H__
#define __INTERRUPT_CALLBACK_H__

#include "stm32f4xx_hal.h"
// --------------------------------------------------------------------------
// Callback Redefine
// --------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif  //__INTERRUPT_CALLBACK_H__
