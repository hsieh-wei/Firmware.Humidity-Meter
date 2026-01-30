#ifndef __SENSOR_MEASURE_TASK_H__
#define __SENSOR_MEASURE_TASK_H__

#include "sht30.h"
#include "led.h"

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    SHT30_HANDLE *target_sht30;
    LED_HANDLE *target_led_d2;
    LED_HANDLE *target_led_d3;
} SENSOR_MEASURE_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void sensor_measure_task(void *parameter);

#endif  //__SENSOR_MEASURE_TASK_H__
