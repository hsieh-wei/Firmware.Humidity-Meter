#ifndef SHT30_H
#define SHT30_H

#include "stm32f4xx_hal.h"

// --------------------------------------------------------------------------
// Error Codes 
// --------------------------------------------------------------------------
typedef enum {
    SHT30_SUCCESS  = 0,
    SHT30_ERROR    = -1,
} SHT30_ERR_t;

// --------------------------------------------------------------------------
// Handle
// --------------------------------------------------------------------------
typedef enum {
    SHT30_IDLE                  = 0,
    SHT30_TX_TRANSMITTED        = 1,
    SHT30_WORKING_TX            = 2,
    SHT30_RX_REQUESTED          = 3,
    SHT30_WORKING_RX            = 4,
} SHT30_Measurement_State;

typedef struct {
    I2C_HandleTypeDef *hi2c;  // I2C HAL handle (EX: &hi2c1)
    uint8_t tx_buf[2];        // sht30 cmd buffer
    uint8_t rx_buf[4];        // sht30 return measurement buffer
    uint8_t i2c_address;
    float humidity;
    float temperature;
    SHT30_Measurement_State status
} SHT30_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int sht30_init(SHT30_HANDLE *handle);
int sht30_read(SHT30_HANDLE *handle);

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
void sht30_irq_tx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *huart);
void sht30_irq_rx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *huart);
#endif // SHT30_H 
