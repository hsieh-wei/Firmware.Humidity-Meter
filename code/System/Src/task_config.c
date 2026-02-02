#include "task_config.h"
#include "board_config.h"

// --------------------------------------------------------
// task handles define(using in suspend and resume)
// --------------------------------------------------------
TaskHandle_t g_sensor_measure_task_handle = NULL;
TaskHandle_t g_log_report_task_handle = NULL;
TaskHandle_t g_lcd_monitor_task_handle;

// --------------------------------------------------------
// task parameters
// --------------------------------------------------------
SENSOR_MEASURE_TASK_PARAMETER g_sensor_measure_task_param = {
    .target_sht30 = &g_sht30_handle,
    .target_led_d2 = &g_led_handle_d2,
    .target_led_d3 = &g_led_handle_d3,
};

LOG_REPORT_TASK_PARAMETER g_log_report_task_param = {
    .target_pc_link = &g_pc_link_handle,
    .target_sys_timestamp = &g_sys_timestamp_handle,
};

LCD_MONITOR_TASK_PARAMETER g_lcd_monitor_task_param = {
    .target_lcd = &g_lcd_handle,
    .target_button_k0 = &g_button_handle_k0,
    .target_button_k1 = &g_button_handle_k1,
};

// --------------------------------------------------------
// API
// Create Task include
// 1.1 Allocate memory from the Heap for Task's stack.
// 1.2 Create TCB
// 1.3 Add this Task to the Ready List
// --------------------------------------------------------
void tasks_create(void) {
    // create sensor measure task
    BaseType_t status;
    status = xTaskCreate(sensor_measure_task,             // function pointer
                         "SENSOR_MEASURE_TASK",           // task name using in debug
                         128,                             // stack size (words)
                         &g_sensor_measure_task_param,    // parameter into task function
                         1,                               // task priority
                         &g_sensor_measure_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);

    // create log report task
    status = xTaskCreate(log_report_task,             // function pointer
                         "LOG_REPORT_TASK",           // task name using in debug
                         256,                         // stack size (words)
                         &g_log_report_task_param,    // parameter into task function
                         1,                           // task priority
                         &g_log_report_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);

    // create lcd monitor task
    status = xTaskCreate(lcd_monitor_task,             // function pointer
                         "LCD_MONITOR_TASK",           // task name using in debug
                         128,                          // stack size (words)
                         &g_lcd_monitor_task_param,    // parameter into task function
                         1,                            // task priority
                         &g_lcd_monitor_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);
}