// --------------------------------------------------------------------------
// This file define global infrastructure which is related with software
// driver(system state handle and rtos handle)
// --------------------------------------------------------------------------
#include "system_state.h"

// --------------------------------------------------------------------------
// Handle Define
// --------------------------------------------------------------------------
SYSTEM_STATE_HANDLE g_system_state_handle = {0};
SemaphoreHandle_t g_system_state_mutex = NULL;

// --------------------------------------------------------------------------
// Handle Define
// --------------------------------------------------------------------------
void system_state_init(void) {
    // initial rtos handle
    if (g_system_state_mutex == NULL) {
        g_system_state_mutex = xSemaphoreCreateMutex();

        // make sure mutex is build
        configASSERT(g_system_state_mutex != NULL);
    }

    // initial system state handle
    g_system_state_handle.sht30_temperature = 0;
    g_system_state_handle.sht30_humidity = 0;
    g_system_state_handle.sht30_temperature_upper_threshold = 0;
    g_system_state_handle.sht30_temperature_lower_threshold = 0;
    g_system_state_handle.sht30_humidity_upper_threshold = 0;
    g_system_state_handle.sht30_humidity_lower_threshold = 0;
    g_system_state_handle.sht30_error_timeout_count = 0;
    g_system_state_handle.sht30_measure_period = 100;
    g_system_state_handle.lcd_brightness = 0;
    g_system_state_handle.lcd_display_mode = 0;
}