#include "FreeRTOS.h"
#include "semphr.h"
#include "lcd_monitor_task.h"
#include "system_state.h"
#include "lcd.h"
#include "button.h"

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------
static int display_temperature(LCD_HANDLE *handle, int temperature, int is_mode_changed) {
    // avoid artifacts
    if (temperature < 0) temperature = 0;
    if (temperature > 99) temperature = 99;

    // initial
    char tempature_tens_digit = (char)(temperature / 10 + 48);
    char tempature_unit_digit = (char)(temperature % 10 + 48);

    // different_mode_as_last, 0 is same, 1 is different
    if (is_mode_changed != 0) {
        if (lcd_fill_screen_dma(handle, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_icon_dma(handle, &LCD_Thermometer_30X30, 10, 45, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'T', &LCD_Font_11x18, 45, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'E', &LCD_Font_11x18, 61, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'M', &LCD_Font_11x18, 77, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'P', &LCD_Font_11x18, 93, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, ':', &LCD_Font_11x18, 109, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
    }

    if (lcd_print_font_dma(handle, tempature_tens_digit, &LCD_Font_11x18, 125, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_print_font_dma(handle, tempature_unit_digit, &LCD_Font_11x18, 141, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
}

static int display_humidity(LCD_HANDLE *handle, int humidity, int is_mode_changed) {
    // avoid artifacts
    if (humidity < 0) humidity = 0;
    if (humidity > 99) humidity = 99;

    // initial
    char humidity_tens_digit = (char)(humidity / 10 + 48);
    char humidity_unit_digit = (char)(humidity % 10 + 48);

    // different_mode_as_last, 0 is same, 1 is different
    if (is_mode_changed != 0) {
        if (lcd_fill_screen_dma(handle, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_icon_dma(handle, &LCD_Thermometer_30X30, 10, 45, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'H', &LCD_Font_11x18, 45, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'U', &LCD_Font_11x18, 61, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'M', &LCD_Font_11x18, 77, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, 'I', &LCD_Font_11x18, 93, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
        if (lcd_print_font_dma(handle, ':', &LCD_Font_11x18, 109, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
    }

    if (lcd_print_font_dma(handle, humidity_tens_digit, &LCD_Font_11x18, 125, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_print_font_dma(handle, humidity_unit_digit, &LCD_Font_11x18, 141, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE) != LCD_SUCCESS) return LCD_ERROR;
}
// --------------------------------------------------------------------------
// Task
// --------------------------------------------------------------------------
void lcd_monitor_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);  // kill itself
    }

    // change void pointer into argument pointer
    LCD_MONITOR_TASK_PARAMETER *task_parameter = (LCD_MONITOR_TASK_PARAMETER *)parameter;

    // Get task parameter
    LCD_HANDLE *lcd = task_parameter->target_lcd;

    // initial
    lcd_init(lcd);
    uint32_t period = g_system_state_handle.lcd_refresh_period;
    int last_mode = -1;
    int current_mode = g_system_state_handle.lcd_display_mode;
    int current_brightness = g_system_state_handle.lcd_brightness;
    float current_temperature = g_system_state_handle.sht30_temperature;
    float current_humidity = g_system_state_handle.sht30_humidity;

    // infinite loop
    while (1) {
        // Snapshot System State
        if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
            period = g_system_state_handle.lcd_refresh_period;
            current_mode = g_system_state_handle.lcd_display_mode;
            current_brightness = g_system_state_handle.lcd_brightness;
            current_temperature = g_system_state_handle.sht30_temperature;
            current_humidity = g_system_state_handle.sht30_humidity;
            xSemaphoreGive(g_system_state_mutex);
        }

        // Draw Logic
        if (current_mode == 0) {
            if (display_temperature(lcd, (int)current_temperature, last_mode ^ current_mode)) goto error_handler;
        } else if (current_mode == 1) {
            if (display_humidity(lcd, (int)current_humidity, last_mode ^ current_mode)) goto error_handler;
        }
        if (lcd_adjust_backlight(lcd, current_brightness)) goto error_handler;

        last_mode = current_mode;
        vTaskDelay(pdMS_TO_TICKS(period));

    error_handler:
        if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
            g_system_state_handle.lcd_monitor_timeout_count++;

            // force refresh next time
            last_mode = -1;

            xSemaphoreGive(g_system_state_mutex);
        }
        // Wait normally even if error occurred to avoid spinning
        vTaskDelay(pdMS_TO_TICKS(period));
    }
}
