#ifndef __LED_TOGGLE_TASK_H__
#define __LED_TOGGLE_TASK_H__

#include "FreeRTOS.h"
#include "led.h"
#include "stm32f4xx_hal.h"
#include "task.h"

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
  LED_HANDLE *target_led;
  uint32_t blinking_period;
} LED_BLINKING_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void led_blinking_task(void *parameter);
// --------------------------------------------------------------------------
// Redefine Callback
// --------------------------------------------------------------------------

#endif //__LED_TOGGLE_TASK_H__
