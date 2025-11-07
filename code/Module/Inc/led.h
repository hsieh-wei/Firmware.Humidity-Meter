#ifndef __led_H__
#define __led_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>

// --------------------------------------------------------------------------
// Error Codes 
// --------------------------------------------------------------------------
typedef enum {
    LED_SUCCESS  = 0,
    LED_ERROR    = -1,
} LED_ERR_t;

// --------------------------------------------------------------------------
// Handle
// --------------------------------------------------------------------------
typedef struct {
    GPIO_TypeDef *gpiox;  // gpio HAL handle (EX: &GPIOA) 
    uint16_t gpio_pin;    // gpio HAL pin (EX: GPIO_PIN_6) 
} LED_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int led_on(LED_HANDLE *handle);
int led_off(LED_HANDLE *handle);
int led_toggle(LED_HANDLE *handle);

#endif
