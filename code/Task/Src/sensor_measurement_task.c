#include "sensor_measure_task.h"

// Task
// --------------------------------------------------------------------------
void led_blinking_task(void *parameter) {
  // avoid null pointer crash
  if (parameter == NULL) {
    vTaskDelete(NULL); // kill itself
  }
  // change void pointer into argument pointer
  SENSOR_MEASURE_TASK_PARAMETER *task_parameter =
      (SENSOR_MEASURE_TASK_PARAMETER *)parameter;

  // Get task parameter
  uint32_t period = task_parameter->measure_period;

  // infinite loop

  while (1) {

    vTaskDelay(pdMS_TO_TICKS(period));
  }
}

// --------------------------------------------------------------------------
// Redefine Callback
// --------------------------------------------------------------------------
