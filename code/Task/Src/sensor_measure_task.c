#include "sensor_measure_task.h"
#include "board_config.h"
#include "led.h"
#include "sht30.h"
#include "system_state.h"

// Task
// --------------------------------------------------------------------------
void sensor_measure_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);  // kill itself
    }
    // change void pointer into argument pointer
    SENSOR_MEASURE_TASK_PARAMETER *task_parameter = (SENSOR_MEASURE_TASK_PARAMETER *)parameter;

    // Get task parameter
    SHT30_HANDLE *sht30 = task_parameter->target_sht30;

    // initial
    sht30_init(sht30);
    uint32_t period = g_system_state_handle.sht30_measure_period;

    // infinite loop
    while (1) {
        // dynamic change sensor peroid
        if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
            period = g_system_state_handle.sht30_measure_period;
            xSemaphoreGive(g_system_state_mutex);
        }

        // sensor working and alarm judge
        if (sht30_measure_data_dma(sht30) == SHT30_SUCCESS) {
            if (sht30_get_data_dma(sht30) == SHT30_SUCCESS) {
                sht30_compute_data(sht30);
                if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
                    // get new sensor data
                    g_system_state_handle.sht30_temperature = sht30->temperature;
                    g_system_state_handle.sht30_humidity = sht30->humidity;

                    // setting alarm led
                    if (sht30->temperature > g_system_state_handle.sht30_temperature_upper_threshold ||
                        sht30->temperature < g_system_state_handle.sht30_temperature_lower_threshold) {
                        led_on(&g_led_handle_d2);
                    } else {
                        led_off(&g_led_handle_d2);
                    }
                    if (sht30->humidity > g_system_state_handle.sht30_humidity_upper_threshold ||
                        sht30->humidity < g_system_state_handle.sht30_humidity_lower_threshold) {
                        led_on(&g_led_handle_d3);
                    } else {
                        led_off(&g_led_handle_d3);
                    }

                    xSemaphoreGive(g_system_state_mutex);
                }
            }
        } else {
            // Error Handling will be write
            if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
                g_system_state_handle.sht30_error_timeout_count++;
                xSemaphoreGive(g_system_state_mutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(period));
    }
}
