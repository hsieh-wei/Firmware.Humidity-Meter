#include "sensor_measure_task.h"
#include "board_config.h"
#include "led.h"
#include "sht30.h"
#include "system_state.h"
#include "semphr.h"
#include "FreeRTOS.h"

// --------------------------------------------------------------------------
// Task
// --------------------------------------------------------------------------
void sensor_measure_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);
    }

    // change void pointer into argument pointer
    SENSOR_MEASURE_TASK_PARAMETER *task_parameter = (SENSOR_MEASURE_TASK_PARAMETER *)parameter;

    // Get task parameter
    SHT30_HANDLE *sht30 = task_parameter->target_sht30;
    LED_HANDLE *led_d2 = task_parameter->target_led_d2;
    LED_HANDLE *led_d3 = task_parameter->target_led_d3;

    // initial
    sht30_init(sht30);

    // [修正1] 給予預設值，防止第一次失敗導致 Delay 異常
    uint32_t period = 1000;

    // 用於暫存 LED 狀態，避免在 Mutex 內操作 IO (雖然 GPIO很快，但這是好習慣)
    int led_d2_state = 0;
    int led_d3_state = 0;

    while (1) {
        // step 1 trigger measurement
        if (sht30_measure_data_dma(sht30) == SHT30_SUCCESS) {
            // step 2 read data
            if (sht30_get_data_dma(sht30) == SHT30_SUCCESS) {
                // step 3 compute temperature and humidity
                sht30_compute_data(sht30);

                // step 4 critical section
                if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
                    // update global state
                    g_system_state_handle.sht30_temperature = sht30->temperature;
                    g_system_state_handle.sht30_humidity = sht30->humidity;

                    // calculate alarm (logic only)
                    if ((int)sht30->temperature > g_system_state_handle.sht30_temperature_upper_threshold ||
                        (int)sht30->temperature < g_system_state_handle.sht30_temperature_lower_threshold) {
                        led_d2_state = 1;
                    } else {
                        led_d2_state = 0;
                    }
                    if ((int)sht30->humidity > g_system_state_handle.sht30_humidity_upper_threshold ||
                        (int)sht30->humidity < g_system_state_handle.sht30_humidity_lower_threshold) {
                        led_d3_state = 1;
                    } else {
                        led_d3_state = 0;
                    }

                    // update period
                    period = g_system_state_handle.sht30_measure_period;

                    xSemaphoreGive(g_system_state_mutex);
                }

                // step 5 change led status
                if (led_d2_state)
                    led_on(led_d2);
                else
                    led_off(led_d2);
                if (led_d3_state)
                    led_on(led_d3);
                else
                    led_off(led_d3);

            } else {
                // Get Data Failed
                goto error_handler;
            }
        } else {
            // Measure Trigger Failed
            goto error_handler;
        }

        vTaskDelay(pdMS_TO_TICKS(period));
        continue;

    // ---------------------------------------------------------
    // Error Handler (Unified)
    // ---------------------------------------------------------
    error_handler:
        if (xSemaphoreTake(g_system_state_mutex, portMAX_DELAY) == pdTRUE) {
            g_system_state_handle.sht30_error_timeout_count++;
            period = g_system_state_handle.sht30_measure_period;
            xSemaphoreGive(g_system_state_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(period));
    }
}
