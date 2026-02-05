#include "command_issue_task.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "pc_link.h"
#include "system_state.h"

// --------------------------------------------------------------------------
// Notify Element
// --------------------------------------------------------------------------
StreamBufferHandle_t command_stream_buffer = NULL;

// --------------------------------------------------------------------------
// define command rule
// --------------------------------------------------------------------------
// cmd [0] = item, cmd [1] = sub item, cmd [2] = value
// item, sub item
// 00,00 = temperature,sht30_temperature_upper_threshold
// 00,01 = temperature,sht30_temperature_lower_threshold
// 01,00 = humidity,sht30_humidity_upper_threshold;
// 01,01 = humidity,sht30_humidity_lower_threshold
// 02,00 = lcd,lcd_brightness
// 02,01 = lcd,lcd_display_mode

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
    command_stream_buffer = xStreamBufferCreate(30 * sizeof(uint8_t), 3 * sizeof(uint8_t));
    configASSERT(command_stream_buffer != NULL);

    // init pc link
    uint8_t cmd_process_buffer[3];
    pc_link_rx_init(pc_link);
    size_t receive_size;
    // infinite loop
    while (1) {
        // get stream buffer data, get cmd per time(3 byte)
        receive_size = xStreamBufferReceive(command_stream_buffer, cmd_process_buffer, sizeof(cmd_process_buffer), portMAX_DELAY);

        // check cmd
        if (receive_size == 3) {
            if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
                switch (cmd_process_buffer[0]) {
                    case CMD_ITEM_TEMPERATURE:
                        switch (cmd_process_buffer[1]) {
                            case CMD_SUB_TEMPERATURE_UPPER_THERSHOLD:
                                g_system_state_handle.sht30_temperature_upper_threshold = cmd_process_buffer[2];
                                break;
                            case CMD_SUB_TEMPERATURE_LOWER_THERSHOLD:
                                g_system_state_handle.sht30_temperature_lower_threshold = cmd_process_buffer[2];
                                break;
                        }
                        break;
                    case CMD_ITEM_HUMIDITY:
                        switch (cmd_process_buffer[1]) {
                            case CMD_SUB_HUMIDITY_UPPER_THERSHOLD:
                                g_system_state_handle.sht30_humidity_upper_threshold = cmd_process_buffer[2];
                                break;
                            case CMD_SUB_HUMIDITY_LOWER_THERSHOLD:
                                g_system_state_handle.sht30_humidity_lower_threshold = cmd_process_buffer[2];
                                break;
                        }
                        break;
                    case CMD_ITEM_LCD:
                        switch (cmd_process_buffer[1]) {
                            case CMD_SUB_LCD_BRIGHTNESS:
                                g_system_state_handle.lcd_brightness = cmd_process_buffer[2];
                                break;
                            case CMD_SUB_LCD_MODE:
                                g_system_state_handle.lcd_display_mode = cmd_process_buffer[2];
                                break;
                        }
                        break;
                }
                xSemaphoreGive(g_system_state_mutex);
            }
        }
    }
}
