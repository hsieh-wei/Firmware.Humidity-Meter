#ifndef __REPORT_LOG_H__
#define __REPORT_LOG_H__

#include "FreeRTOS.h"
#include "sht30.h"
#include "stm32f4xx_hal.h"
#include "semphr.h"

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    SHT30_HANDLE *target_sht30;
} SENSOR_MEASURE_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void sensor_measure_task(void *parameter);

#endif  //__REPORT_LOG_H__
