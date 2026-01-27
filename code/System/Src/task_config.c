#include "task_config.h"
#include "board_config.h"

// --------------------------------------------------------
// task handles define(using in suspend and resume)
// --------------------------------------------------------
TaskHandle_t g_sensor_task_handle = NULL;

// --------------------------------------------------------
// task parameters
// --------------------------------------------------------
SENSOR_MEASURE_TASK_PARAMETER g_sensor_task_param = {
    .target_sht30 = &g_sht30_handle,
};

// --------------------------------------------------------
// API
// Create Task include
// 1.1 Allocate memory from the Heap for Task's stack.
// 1.2 Create TCB
// 1.3 Add this Task to the Ready List
// --------------------------------------------------------
void tasks_create(void) {
    // Create Sensor Task
    BaseType_t status;
    status = xTaskCreate(sensor_measure_task,     // function pointer
                         "SHT30_SENSOR_TASK",     // task name using in debug
                         128,                     // stack size (words)
                         &g_sensor_task_param,    // parameter into task function
                         1,                       // task priority
                         &g_sensor_task_handle);  // handle using in suspend, delete, notify
    configASSERT(status == pdPASS);
}