#ifndef __SENSOR_MEASURE_H__
#define __SENSOR_MEASURE_H__

#include "FreeRTOS.h"
#include "sht30.h"
#include "stm32f4xx_hal.h"
#include "task.h"

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
  SHT30_HANDLE *target_sht30;
  uint32_t measure_period;
} SENSOR_MEASURE_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void sensor_measure_task(void *parameter);
// --------------------------------------------------------------------------
// Redefine Callback
// --------------------------------------------------------------------------
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
#endif //__SENSOR_MEASURE_H__
