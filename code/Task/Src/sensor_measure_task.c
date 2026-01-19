#include "sensor_measure_task.h"
#include "board_config.h"
#include "sht30.h"

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

  sht30_init(sht30);
  // infinite loop
  while (1) {
    if (sht30->status == SHT30_IDLE || sht30->status == SHT30_COMPUTE_DONE) {
      sht30_measure_data_dma(sht30);
    } else if (sht30->status == SHT30_TX_DONE) {
      vTaskDelay(15); // waiting time after high repeatability measurement
      sht30_get_data_dma(sht30);
    } else if (sht30->status == SHT30_RX_DONE) {
      sht30_compute_data(sht30);
      vTaskDelay(pdMS_TO_TICKS(period));
    }
  }
}
