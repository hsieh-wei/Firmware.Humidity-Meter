#include "led_blinking_task.h"

// Task
// --------------------------------------------------------------------------
void led_blinking_task(void *argument) {
  // avoid null pointer crash
  if (argument == NULL) {
    vTaskDelete(NULL); // kill itself
  }
  // change void pointer into argument pointer
  LED_BLINKING_TASK_ARGUMENT *task_argument =
      (LED_BLINKING_TASK_ARGUMENT *)argument;

  // Get task argument
  LED_HANDLE *led = task_argument->target_led;
  uint32_t period = task_argument->blinking_period;

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
