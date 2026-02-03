#include "interrupt_callback.h"
#include "board_config.h"
#include "sht30.h"
#include "pc_link.h"
#include "sys_timestamp.h"
#include "lcd.h"

// --------------------------------------------------------------------------
// Callback Redefine
// --------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    pc_link_uart_tx_cplt(&g_pc_link_handle, huart);
    // you can add another uart callback below
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    pc_link_uartex_rx_event(&g_pc_link_handle, huart, Size);
    // you can add another uart callback below
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    sht30_i2c_master_tx_cplt(&g_sht30_handle, hi2c);
    // you can add another i2c callback below
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    sht30_i2c_master_rx_cplt(&g_sht30_handle, hi2c);
    // you can add another i2c callback below
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    lcd_spi_tx_cplt(&g_lcd_handle, hspi);
    // you can add another spi callback below
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // system clock for rtos
    if (htim->Instance == TIM7) {
        HAL_IncTick();
    }

    sys_timestamp_tim_period_elapsed(&g_sys_timestamp_handle, htim);
    // you can add another tim callback below
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    BaseType_t yield = pdFALSE;
    if (g_button_handle_k0.gpio_pin == GPIO_Pin) {
        //
    } else if (g_button_handle_k1.gpio_pin == GPIO_Pin) {
        //
    }
    // you can add another gpio callback below
    portYIELD_FROM_ISR(yield);
}