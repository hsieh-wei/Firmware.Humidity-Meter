#ifndef __SYSTEM_STATE_H__
#define __SYSTEM_STATE_H__

#include "FreeRTOS.h"
#include "semphr.h"

// --------------------------------------------------------------------------
// System State Define
// --------------------------------------------------------------------------
typedef struct {
    // update by sensor task
    float sht30_temperature;
    float sht30_humidity;

    // update by rx from pc task
    int sht30_temperature_upper_threshold;
    int sht30_temperature_lower_threshold;
    int sht30_humidity_upper_threshold;
    int sht30_humidity_lower_threshold;
    int sht30_timeout_count;
    uint32_t sht30_measure_period;  // period in ms

    // update by rx from pc task
    uint8_t lcd_brightness;       // 0~100, larger number means brighter
    int lcd_display_mode;         // 0 display temperature, 1 display humidity
    uint32_t lcd_refresh_period;  // period in ms
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

#endif  // __SYSTEM_STATE_H__