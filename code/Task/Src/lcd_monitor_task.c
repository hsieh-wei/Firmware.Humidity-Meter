#include "FreeRTOS.h"
#include "semphr.h"
#include "lcd_monitor_task.h"
#include "system_state.h"
#include "lcd.h"

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------
static char tempature_tens_digit;
static char tempature_unit_digit;
static void display_temperature(LCD_HANDLE *handle) {
    if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
        tempature_tens_digit = (char)(((int)g_system_state_handle.sht30_temperature) / 10 + 48);
        tempature_unit_digit = (char)(((int)g_system_state_handle.sht30_temperature) % 10 + 48);
    }
    (void)lcd_fill_screen(handle, LCD_COLOR_WHITE);
    (void)lcd_print_icon(handle, &LCD_Thermometer_30X30, 10, 45, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
    (void)lcd_print_font(handle, 'T', &LCD_Font_11x18, 45, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
    (void)lcd_print_font(handle, 'e', &LCD_Font_11x18, 61, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
    (void)lcd_print_font(handle, 'm', &LCD_Font_11x18, 77, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
    (void)lcd_print_font(handle, 'p', &LCD_Font_11x18, 93, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
    (void)lcd_print_font(handle, ':', &LCD_Font_11x18, 109, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
    (void)lcd_print_font(handle, tempature_tens_digit, &LCD_Font_11x18, 125, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
    (void)lcd_print_font(handle, tempature_unit_digit, &LCD_Font_11x18, 141, 51, LCD_COLOR_BLUE, LCD_COLOR_WHITE);
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
    LCD_MONITOR_TASK_HANDLE *task_parameter = (LCD_MONITOR_TASK_HANDLE *)parameter;

    // Get task parameter
    LCD_HANDLE *lcd = task_parameter->target_lcd;

    // initial
    lcd_init(lcd);
    uint32_t period;

    // infinite loop
    while (1) {
        display_temperature(lcd);
        if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
            period = g_system_state_handle.sht30_measure_period;
            xSemaphoreGive(g_system_state_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(period));
    }
}
