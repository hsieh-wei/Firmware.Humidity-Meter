#include "FreeRTOS.h"
#include "semphr.h"
#include "report_log_task.h"
#include "pc_link.h"
#include "system_state.h"

// Task
// --------------------------------------------------------------------------
void report_log_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);  // kill itself
    }

    // change void pointer into argument pointer
    LOG_TASK_PARAMETER *task_parameter = (LOG_TASK_PARAMETER *)parameter;

    // Get task parameter
    PC_LINK_HANDLE *handle = task_parameter->target_pc_link;

    // init
    pc_link_init(handle);
    SYSTEM_STATE_HANDLE current_system_state;
    char log_buffer[128];

    while (1) {
        // get the current system state
        if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
            current_system_state = g_system_state_handle;
            xSemaphoreGive(g_system_state_mutex);
        }

        // log formatting
        snprintf(log_buffer, sizeof(log_buffer),
                 "[temperature] %d, threshold:%d,%d\n"
                 "[humidity] %d, threshold:%d,%d\n"
                 "[lcd] brightness:%d, mode:%d\n"
                 "[error] sht30: %d"
                 "[period]sht30 measure:%d, lcd refresh:%d",
                 // temperature
                 (int)(current_system_state.sht30_temperature), current_system_state.sht30_temperature_lower_threshold,
                 current_system_state.sht30_temperature_upper_threshold,
                 // humidity
                 (int)(current_system_state.sht30_humidity), current_system_state.sht30_humidity_lower_threshold,
                 current_system_state.sht30_humidity_upper_threshold,
                 // lcd
                 (int)current_system_state.lcd_brightness, current_system_state.lcd_display_mode,
                 // error
                 current_system_state.sht30_error_timeout_count,
                 // period
                 current_system_state.sht30_measure_period, current_system_state.lcd_refresh_period);
    }
}
