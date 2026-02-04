#include "command_issue_task.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "pc_link.h"

// --------------------------------------------------------------------------
// Notify Element
// --------------------------------------------------------------------------
QueueHandle_t command_queue = NULL;

// --------------------------------------------------------------------------
// define command rule
// --------------------------------------------------------------------------
// cmd [0] = item, cmd [1] = sub item, cmd [2] = value
// item, sub item
// 0,0 = temperature,sht30_temperature_upper_threshold
// 0,1 = temperature,sht30_temperature_lower_threshold
// 1,0 = humidity,sht30_humidity_upper_threshold;
// 1,1 = humidity,sht30_humidity_lower_threshold
// 2,0 = lcd,lcd_brightness
// 2,1 = lcd,lcd_display_mode
// 3,0 = period,sht30_measure_period
// 3,1 = period,pc_link_log_report_period
// 3,2 = period,lcd_refresh_period

typedef enum {
    CMD_ITEM_TEMPERATURE = 0,
    CMD_ITEM_HUMIDITY = 1,
    CMD_ITEM_LCD = 2,
    CMD_ITEM_PERIOD = 3,
} CMD_ITEM;

typedef enum {
    CMD_SUB_TEMPERATURE_UPPER_THERSHOLD = 0,
    CMD_SUB_TEMPERATURE_LOWER_THERSHOLD = 1,
} CMD_SUB_TEMPERATURE;

typedef enum {
    CMD_SUB_HUMIDITY_UPPER_THERSHOLD = 0,
    CMD_SUB_HUMIDITY_LOWER_THERSHOLD = 1,
} CMD_SUB_HUMIDITY;

typedef enum {
    CMD_SUB_LCD_BRIGHTNESS = 0,
    CMD_SUB_LCD_MODE = 1,
} CMD_SUB_LCD;

typedef enum {
    CMD_SUB_PERIOD_SHT30 = 0,
    CMD_SUB_PERIOD_LOG = 1,
    CMD_SUB_PERIOD_LCD = 2,
} CMD_SUB_PERIOD;

// --------------------------------------------------------------------------
// Task
// --------------------------------------------------------------------------
void command_issue_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);  // kill itself
    }

    // change void pointer into argument pointer
    COMMAND_ISSUE_TASK_PARAMETER *task_parameter = (COMMAND_ISSUE_TASK_PARAMETER *)parameter;

    // Get task parameter
    PC_LINK_HANDLE *pc_link = task_parameter->target_pc_link;

    // initial queue
    command_queue = xQueueCreate(128, sizeof(uint8_t));
    configASSERT(command_queue != NULL);

    // init pc link
    pc_link_rx_init(pc_link);

    // infinite loop
    while (1) {
    }
}
