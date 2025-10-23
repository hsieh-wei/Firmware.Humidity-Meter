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
typedef struct {
    I2C_HandleTypeDef *hi2c;  // I2C HAL handle (EX: &hi2c1) 
    uint8_t i2c_address;
    float humidity;
    float temperature;
} SHT30_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int sht30_init(SHT30_HANDLE *handle);
int sht30_read(SHT30_HANDLE *handle);

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
void sht30_irq_rx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *huart);
void sht30_irq_tx_cplt(SHT30_HANDLE *handle, I2C_HandleTypeDef *huart);
#endif // SHT30_H 
