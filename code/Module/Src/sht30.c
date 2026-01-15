#include "sht30.h"
#include "pc_link.h"
#include "stm32f4xx_hal_i2c.h"
#include <stdint.h>
#include <stdio.h>

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------
#define SHT30_ADDRESS (0x44 << 1) // ADDR connected to logic low

static uint8_t sht30_crc8(const uint8_t *data) {
  uint8_t crc = 0xFF; // Init
  for (uint32_t i = 0; i < 2; ++i) {
    // crc two byte
    crc ^= data[i];
    for (int j = 0; j < 8; ++j) {
      // MSB first => And 0x80(get the highest bit is 1 or 0)
      // if highest bit is 1 ,left shift, XOR Polynomial(0x31)
      // if highest bit is 0 ,left shift
      crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
    }
  }
  return crc; // XorOut=0x00
}

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int sht30_init(SHT30_HANDLE *handle) {
  if (!handle || !handle->hi2c) {
    return SHT30_ERROR;
  }

  // minimal waiting time after power up
  HAL_Delay(1);

  // initial variable
  handle->i2c_address = SHT30_ADDRESS;

  // Soft Reset
  handle->tx_buf[0] = 0x30;
  handle->tx_buf[1] = 0xA2;
  if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, handle->tx_buf,
                              2, 200) != HAL_OK)
    return SHT30_ERROR;
  handle->status = SHT30_TX_TRANSMITTED;
  HAL_Delay(2); // minimal waiting time after soft reset

  // Stop Periodic Measurement Mode
  handle->tx_buf[0] = 0x30;
  handle->tx_buf[1] = 0x93;
  if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, handle->tx_buf,
                              2, 200) != HAL_OK)
    return SHT30_ERROR;
  handle->status = SHT30_TX_TRANSMITTED;
  HAL_Delay(1); // minimal waiting time before another command

  // Disable Heater
  handle->tx_buf[0] = 0x30;
  handle->tx_buf[1] = 0x66;
  if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, handle->tx_buf,
                              2, 200) != HAL_OK)
    return SHT30_ERROR;
  handle->status = SHT30_TX_TRANSMITTED;
  HAL_Delay(1); // minimal waiting time before another command

  // Clear SHT30 Status Register
  handle->tx_buf[0] = 0x30;
  handle->tx_buf[1] = 0x41;
  if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, handle->tx_buf,
                              2, 200) != HAL_OK)
    return SHT30_ERROR;
  handle->status = SHT30_TX_TRANSMITTED;
  HAL_Delay(1); // minimal waiting time before another command

  handle->status = SHT30_IDLE;

  return SHT30_SUCCESS;
}

int sht30_measure_data_dma(SHT30_HANDLE *handle) {
  if (!handle || !handle->hi2c) {
    return SHT30_ERROR;
  }

  // Repeatability High, Disabled Clock Stretching Measurement
  handle->tx_buf[0] = 0x24;
  handle->tx_buf[1] = 0x00;
  if (HAL_I2C_Master_Transmit_DMA(handle->hi2c, handle->i2c_address,
                                  handle->tx_buf, 2) != HAL_OK)
    return SHT30_ERROR;
  handle->status = SHT30_TX_TRANSMITTED;

  return SHT30_SUCCESS;
}

int sht30_get_data_dma(SHT30_HANDLE *handle) {
  if (!handle || !handle->hi2c) {
    return SHT30_ERROR;
  }

  // minimal waiting time after high repeatability measurement
  HAL_Delay(15);

  // get measurement data
  if (HAL_I2C_Master_Receive_DMA(handle->hi2c, handle->i2c_address,
                                 handle->rx_buf, 6) != HAL_OK)
    return SHT30_ERROR;
  handle->status = SHT30_RX_REQUESTED;

  return SHT30_SUCCESS;
}

int sht30_compute_data(SHT30_HANDLE *handle) {
  if (!handle || !handle->hi2c) {
    return SHT30_ERROR;
  }

  // CRC Validate
  if (sht30_crc8(&handle->rx_buf[0]) != handle->rx_buf[2])
    return SHT30_ERROR;
  if (sht30_crc8(&handle->rx_buf[3]) != handle->rx_buf[5])
    return SHT30_ERROR;

  // Calculate Temperature
  uint16_t raw_temperature =
      (uint16_t)((handle->rx_buf[0] << 8) | handle->rx_buf[1]);
  handle->temperature =
      -45.0f + 175.0f * ((float)raw_temperature / (65535.0f)); // 65535 = 2^16-1

  // Calculate Humidity
  uint16_t raw_humidity =
      (uint16_t)((handle->rx_buf[3] << 8) | handle->rx_buf[4]);
  handle->humidity = 100 * (float)raw_humidity / 65535.0f; // 65535 = 2^16-1

  handle->status = SHT30_COMPUTE_DONE;

  return SHT30_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define
// --------------------------------------------------------------------------
void sht30_i2c_master_tx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *hi2c) {
  if (handle && handle->hi2c == hi2c) {
    handle->status = SHT30_TX_DONE;
  }
}

void sht30_i2c_master_rx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *hi2c) {
  if (handle && handle->hi2c == hi2c) {
    handle->status = SHT30_RX_DONE;
  }
}
