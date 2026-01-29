#include "FreeRTOS.h"
#include "semphr.h"
#include "log_report_task.h"
#include "pc_link.h"
#include "system_state.h"
#include <stdio.h>

// Task
// --------------------------------------------------------------------------
void log_report_task(void *parameter) {
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
    char log_buffer[256];
    int log_buffer_len;

    // infinite loop
    while (1) {
        // get the current system state
        if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
            current_system_state = g_system_state_handle;
            xSemaphoreGive(g_system_state_mutex);
        }

        // log formatting
        log_buffer_len = snprintf(log_buffer, sizeof(log_buffer),
                                  "[temperature] %d, threshold:%d,%d\n"
                                  "[humidity] %d, threshold:%d,%d\n"
                                  "[lcd] brightness:%d, mode:%d\n"
                                  "[error] sht30: %d, pc_link: %d\n"
                                  "[period]sht30 measure:%d, log report:%d, lcd refresh:%d\n",
                                  // temperature
                                  (int)(current_system_state.sht30_temperature), current_system_state.sht30_temperature_lower_threshold,
                                  current_system_state.sht30_temperature_upper_threshold,
                                  // humidity
                                  (int)(current_system_state.sht30_humidity), current_system_state.sht30_humidity_lower_threshold,
                                  current_system_state.sht30_humidity_upper_threshold,
                                  // lcd
                                  (int)current_system_state.lcd_brightness, current_system_state.lcd_display_mode,
                                  // error
                                  current_system_state.sht30_error_timeout_count, current_system_state.pc_link_error_timeout_count,
                                  // period
                                  (int)current_system_state.sht30_measure_period, (int)current_system_state.pc_link_log_report_period,
                                  (int)current_system_state.lcd_refresh_period);

        // send log by uart
        int tx_success = 0;

        // judge format is success
        if (log_buffer_len > 0 && log_buffer_len < sizeof(log_buffer)) {
            // judge tx is success
            if (pc_link_tx_dma(handle, (uint8_t *)log_buffer, (uint16_t)log_buffer_len) == PC_LINK_SUCCESS) {
                tx_success = 1;
            }
        }

        // error handle
        if (tx_success != 1) {
            if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
                g_system_state_handle.pc_link_error_timeout_count++;
                xSemaphoreGive(g_system_state_mutex);
            }
        }

        // period
        vTaskDelay(pdMS_TO_TICKS(current_system_state.pc_link_log_report_period));
    }
}
