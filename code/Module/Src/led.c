#include "led.h"
// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int led_on(LED_HANDLE *handle) {
    HAL_GPIO_WritePin(handle->gpiox, handle->gpio_pin, GPIO_PIN_RESET);  // active-low
    return LED_SUCCESS;
}

int led_off(LED_HANDLE *handle) {
    HAL_GPIO_WritePin(handle->gpiox, handle->gpio_pin, GPIO_PIN_SET);  // active-low
    return LED_SUCCESS;
}

int led_toggle(LED_HANDLE *handle) {
    HAL_GPIO_TogglePin(handle->gpiox, handle->gpio_pin);
    return LED_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define
// --------------------------------------------------------------------------