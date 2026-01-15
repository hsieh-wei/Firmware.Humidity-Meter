#ifndef __SYSTEM_STATE_H__
#define __SYSTEM_STATE_H__

#include "FreeRTOS.h"
#include "semphr.h"

// --------------------------------------------------------------------------
// System State Define
// --------------------------------------------------------------------------
typedef struct {
  // update by sensor task
  float temperature;
  float humidity;

  // update by sensor task
  int temperature_alarm; // 0 is not trigger, 1 is trigger
  int humidity_alarm;    // 0 is not trigger, 1 is trigger

  // update by rx from pc task
  uint32_t sht30_measure_period; // period in ms
  uint8_t lcd_brightness;        // 0~100, larger number means brighter
  int lcd_display_mode;          // 0 display temperature, 1 display humidity
} SYSTEM_STATE_HANDLE;

// --------------------------------------------------------------------------
// System State Instance and RTOS Handle Define
// --------------------------------------------------------------------------
extern SYSTEM_STATE_HANDLE g_system_state_handle;
extern SemaphoreHandle_t g_system_state_mutex;

// --------------------------------------------------------------------------
// Initial API
// --------------------------------------------------------------------------
void system_state_init(void);

#endif // __SYSTEM_STATE_H__