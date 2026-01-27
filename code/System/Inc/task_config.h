#ifndef __TASK_CONFIG_H__
#define __TASK_CONFIG_H__

#include "FreeRTOS.h"
#include "task.h"
#include "sensor_measure_task.h"

// --------------------------------------------------------
// task handles define(using in suspend and resume)
// --------------------------------------------------------
extern TaskHandle_t g_sensor_task_handle;

// --------------------------------------------------------
// task parameters
// --------------------------------------------------------
extern SENSOR_MEASURE_TASK_PARAMETER g_sensor_task_param;

// --------------------------------------------------------
// API
// --------------------------------------------------------
void tasks_create(void);  // create all task

#endif