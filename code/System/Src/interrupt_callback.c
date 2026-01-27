#include "interrupt_callback.h"
#include "board_config.h"
#include "sht30.h"
#include "pc_link.h"

// --------------------------------------------------------------------------
// Callback Redefine
// --------------------------------------------------------------------------
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    sht30_i2c_master_tx_cplt(&g_sht30_handle, hi2c);
    // you can add another i2c callback below
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    sht30_i2c_master_rx_cplt(&g_sht30_handle, hi2c);
    // you can add another i2c callback below
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    pc_link_uart_tx_cplt(&g_pc_link_handle, huart);
    // you can add another uart callback below
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    pc_link_uartex_rx_event(&g_pc_link_handle, huart, Size);
    // you can add another uart callback below
}