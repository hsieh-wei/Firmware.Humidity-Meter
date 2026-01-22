#include "interrupt_callback.h"
#include "board_config.h"
#include "sht30.h"

// --------------------------------------------------------------------------
// Callback Redefine
// --------------------------------------------------------------------------
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
  sht30_i2c_master_tx_cplt(&g_sht30_handle, hi2c);
  // you can add another i2c callback below
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  sht30_i2c_master_rx_cplt(&g_sht30_handle, hi2c);
}