#include "led_blinking_task.h"

// --------------------------------------------------------------------------
// static handle
// --------------------------------------------------------------------------
static LED_HANDLE s_led_handle_d2 = {.gpiox = GPIOA, .gpio_pin = GPIO_PIN_6};
static LED_HANDLE s_led_handle_d3 = {.gpiox = GPIOA, .gpio_pin = GPIO_PIN_7};
// --------------------------------------------------------------------------
// Task
// --------------------------------------------------------------------------
void led_toggle_task(void *argument) {
  led_off(&s_led_handle_d3);
  while (1) {
    led_toggle(&s_led_handle_d2);
    vTaskDelay(500);
  }
}

// --------------------------------------------------------------------------
// Redefine Callback
// --------------------------------------------------------------------------
