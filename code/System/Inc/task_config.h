#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "FreeRTOS.h"
#include "task.h"
#include "sensor_measure_task.h"

// --------------------------------------------------------
// Task Handles (using in suspend and resume)
// --------------------------------------------------------
extern TaskHandle_t xSensorTaskHandle;

// --------------------------------------------------------
// Task Parameters
// --------------------------------------------------------
extern SENSOR_MEASURE_TASK_PARAMETER g_sensor_task_param;

// --------------------------------------------------------
// API
// --------------------------------------------------------
void tasks_init_and_create(void);  // create all task

#endif