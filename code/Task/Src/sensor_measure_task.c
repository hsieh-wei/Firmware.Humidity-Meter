#include "sensor_measure_task.h"
#include "board_config.h"
#include "sht30.h"
#include "system_state.h"

// Task
// --------------------------------------------------------------------------
void sensor_measure_task(void *parameter) {
  // avoid null pointer crash
  if (parameter == NULL) {
    vTaskDelete(NULL); // kill itself
  }
  // change void pointer into argument pointer
  SENSOR_MEASURE_TASK_PARAMETER *task_parameter =
      (SENSOR_MEASURE_TASK_PARAMETER *)parameter;

  // Get task parameter
  SHT30_HANDLE *sht30 = task_parameter->target_sht30;
  uint32_t period = task_parameter->measure_period;
  uint32_t temperature_threshold = task_parameter->temperature_alarm_threshold;
  uint32_t humidity_threshold = task_parameter->humidity_alarm_threshold;

  sht30_init(sht30);
  // infinite loop
  while (1) {
    if (sht30_measure_data_dma(sht30) == SHT30_SUCCESS) {
      if (sht30_get_data_dma(sht30) == SHT30_SUCCESS) {
        sht30_compute_data(sht30);
        g_system_state_handle.temperature = sht30->temperature;
        g_system_state_handle.humidity = sht30->humidity;
        if (sht30->temperature > temperature_threshold) {
          g_system_state_handle.temperature_alarm = 1;
        }
        if (sht30->temperature > humidity_threshold) {
          g_system_state_handle.humidity_alarm = 1;
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(period));
  }
}
