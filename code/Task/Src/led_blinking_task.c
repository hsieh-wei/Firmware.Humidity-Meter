#include "led_blinking_task.h"

// Task
// --------------------------------------------------------------------------
void led_blinking_task(void *parameter) {
  // avoid null pointer crash
  if (parameter == NULL) {
    vTaskDelete(NULL); // kill itself
  }
  // change void pointer into argument pointer
  LED_BLINKING_TASK_ARGUMENT *task_parameter =
      (LED_BLINKING_TASK_PARAMETER *)parameter;

  // Get task parameter
  LED_HANDLE *led = task_parameter->target_led;
  uint32_t period = task_parameter->blinking_period;

  // infinite loop
  led_off(led);
  while (1) {
    led_toggle(led);
    vTaskDelay(pdMS_TO_TICKS(period));
  }
}

// --------------------------------------------------------------------------
// Redefine Callback
// --------------------------------------------------------------------------
