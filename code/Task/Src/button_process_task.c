#include "FreeRTOS.h"
#include "task.h"
#include "system_state.h"
#include "button_process_task.h"
#include "button.h"

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Task
// --------------------------------------------------------------------------
void button_process_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);  // kill itself
    }

    // change void pointer into argument pointer
    // BUTTON_PROCESS_TASK_PARAMETER *task_parameter = (BUTTON_PROCESS_TASK_PARAMETER *)parameter;

    // Get task parameter
    // BUTTON_HANDLE *button_k0 = task_parameter->target_button_k0;
    // BUTTON_HANDLE *button_k1 = task_parameter->target_button_k1;

    // initial
    uint32_t received_bits;
    uint8_t new_lcd_brightness;
    // infinite loop
    while (1) {
        // Task will wake up when receiving any bit
        // ulBitsToClearOnEntry => 0xFFFFFFFF is no clear, the value will copy into received_bits
        // ulBitsToClearOnExit => 0xFFFFFFFF is all clear to avoid obstruction next time notify
        // pulNotificationValue => an pointer that the function will write the notification value before entry clear
        if (xTaskNotifyWait(0, 0xFFFFFFFF, &received_bits, portMAX_DELAY) == pdTRUE) {
            if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
                if ((received_bits & BUTTON_K0_NOTIFY) != 0) {
                    g_system_state_handle.lcd_display_mode ^= 1;  // reverse mode
                } else if ((received_bits & BUTTON_K1_NOTIFY) != 0) {
                    new_lcd_brightness = g_system_state_handle.lcd_brightness;
                    new_lcd_brightness += 10;
                    if (new_lcd_brightness > 100) new_lcd_brightness = 0;
                    g_system_state_handle.lcd_brightness = new_lcd_brightness;
                }
                xSemaphoreGive(g_system_state_mutex);
            }
        }
    }
}
