#ifndef __SYSTEM_STATE_H__
#define __SYSTEM_STATE_H__

#include "FreeRTOS.h"
#include "semphr.h"

// --------------------------------------------------------------------------
// System State Define
// --------------------------------------------------------------------------
typedef struct {
    // temperature
    float sht30_temperature;
    int sht30_temperature_upper_threshold;
    int sht30_temperature_lower_threshold;

    // humidity
    float sht30_humidity;
    int sht30_humidity_upper_threshold;
    int sht30_humidity_lower_threshold;

    // lcd
    uint8_t lcd_brightness;  // 0~100, larger number means brighter
    int lcd_display_mode;    // 0 display temperature, 1 display humidity

    // error
    int sht30_error_timeout_count;
    int pc_link_error_timeout_count;

    // period
    uint32_t sht30_measure_period;       // period in ms
    uint32_t pc_link_log_report_period;  // period in ms
    uint32_t lcd_refresh_period;         // period in ms
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