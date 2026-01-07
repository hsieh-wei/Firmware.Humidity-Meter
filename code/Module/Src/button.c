#include "button.h"
#include "led.h"
// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int button_pressed(BUTTON_HANDLE *handle) {
  handle->pressed = 0;
  if (HAL_GPIO_ReadPin(handle->gpiox, handle->gpio_pin) ==
      GPIO_PIN_RESET) {  // active-low
    handle->pressed = 1; // button pressed
  };
  return BUTTON_SUCCESS;
}
