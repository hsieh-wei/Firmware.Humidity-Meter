#ifndef SHT30_H
#define SHT30_H

#include "FreeRTOS.h"
#include "stm32f4xx_hal.h"
#include "task.h"
#include <stdint.h>

// --------------------------------------------------------------------------
// Error Codes
// --------------------------------------------------------------------------
typedef enum {
  SHT30_SUCCESS = 0,
  SHT30_ERROR = -1,
  SHT30_TIMEOUT = -2,
} SHT30_ERR;

// --------------------------------------------------------------------------
// Handle
// --------------------------------------------------------------------------

// **** using in bare metal ****
// typedef enum {
//   SHT30_IDLE = 0,
//   SHT30_TX_TRANSMITTED = 1,
//   SHT30_TX_DONE = 2,
//   SHT30_RX_REQUESTED = 3,
//   SHT30_RX_DONE = 4,
//   SHT30_COMPUTE_DONE = 5,
// } SHT30_Measurement_State;
// ****************************

typedef struct {
  I2C_HandleTypeDef *hi2c; // I2C HAL handle (EX: &hi2c1)
  uint8_t tx_buf[2];       // sht30 cmd buffer
  uint8_t rx_buf[6];       // sht30 return measurement buffer
  uint8_t i2c_address;
  float humidity;
  float temperature;
  SemaphoreHandle_t tx_rx_complete_semaphore; // notice when tx or rx complete
} SHT30_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int sht30_init(SHT30_HANDLE *handle);
int sht30_measure_data_dma(SHT30_HANDLE *handle);
int sht30_get_data_dma(SHT30_HANDLE *handle);
int sht30_compute_data(SHT30_HANDLE *handle);
// --------------------------------------------------------------------------
// HAL Weak Callback re define
// --------------------------------------------------------------------------
void sht30_i2c_master_tx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *hi2c);
void sht30_i2c_master_rx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *hi2c);
#endif // SHT30_H
