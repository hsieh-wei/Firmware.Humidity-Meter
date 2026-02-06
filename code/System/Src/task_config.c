#include "task_config.h"
#include "board_config.h"

// --------------------------------------------------------
// task handles define(using in suspend and resume)
// --------------------------------------------------------
TaskHandle_t g_sensor_measure_task_handle = NULL;
TaskHandle_t g_log_report_task_handle = NULL;
TaskHandle_t g_lcd_monitor_task_handle = NULL;
TaskHandle_t g_button_process_task_handle = NULL;
TaskHandle_t g_command_issue_task_handle = NULL;

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
};

BUTTON_PROCESS_TASK_PARAMETER g_button_process_task_param = {
    .target_button_k0 = &g_button_handle_k0,
    .target_button_k1 = &g_button_handle_k1,
};

COMMAND_ISSUE_TASK_PARAMETER g_command_issue_task_param = {
    .target_pc_link = &g_pc_link_handle,
};

// --------------------------------------------------------
// API
// Create Task include
// 1.1 Allocate memory from the Heap for Task's stack.
// 1.2 Create TCB
// 1.3 Add this Task to the Ready List
// --------------------------------------------------------
typedef enum {
    PRIORITY_IDLE = 0,  // FreeRTOS reserve
    PRIORITY_LOW = 1,
    PRIORITY_NORMAL = 2,
    PRIORITY_HIGH = 3,
    PRIORITY_REALTIME = 4
} TASK_PRIORITY_E;

void tasks_create(void) {
    BaseType_t status;

    // create button process task
    status = xTaskCreate(button_process_task,             // function pointer
                         "BUTTON_PROCESS_TASK",           // task name using in debug
                         128,                             // stack size (words)
                         &g_button_process_task_param,    // parameter into task function
                         PRIORITY_REALTIME,               // task priority, if delay, the user will feel that the machine is lag.
                         &g_button_process_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);

    // create command issue task
    status = xTaskCreate(command_issue_task,             // function pointer
                         "COMMAND_ISSUE_TASK",           // task name using in debug
                         128,                            // stack size (words)
                         &g_command_issue_task_param,    // parameter into task function
                         PRIORITY_HIGH,                  // task priority, if data is not collected quickly, the buffer may overflow
                         &g_command_issue_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);

    // create sensor measure task
    status = xTaskCreate(sensor_measure_task,             // function pointer
                         "SENSOR_MEASURE_TASK",           // task name using in debug
                         128,                             // stack size (words)
                         &g_sensor_measure_task_param,    // parameter into task function
                         PRIORITY_NORMAL,                 // task priority, It's important, but a 1ms to 5ms delay isn't a big deal.
                         &g_sensor_measure_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);

    // create log report task
    status = xTaskCreate(log_report_task,             // function pointer
                         "LOG_REPORT_TASK",           // task name using in debug
                         256,                         // stack size (words)
                         &g_log_report_task_param,    // parameter into task function
                         PRIORITY_LOW,                // task priority, only for debug
                         &g_log_report_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);

    // create lcd monitor task
    status = xTaskCreate(lcd_monitor_task,             // function pointer
                         "LCD_MONITOR_TASK",           // task name using in debug
                         256,                          // stack size (words)
                         &g_lcd_monitor_task_param,    // parameter into task function
                         PRIORITY_LOW,                 // task priority, If set its priority too high, it will block other tasks when it's drawing.
                         &g_lcd_monitor_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);
}