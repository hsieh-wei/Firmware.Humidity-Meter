#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>

// --------------------------------------------------------------------------
// Error Codes
// --------------------------------------------------------------------------
typedef enum {
  BUTTON_SUCCESS = 0,
  BUTTON_ERROR = -1,
} BUTTON_ERR_t;

// --------------------------------------------------------------------------
// Handle
// --------------------------------------------------------------------------
typedef struct {
  GPIO_TypeDef *gpiox; // gpio HAL handle (EX: &GPIOA)
  uint16_t gpio_pin;   // gpio HAL pin (EX: GPIO_PIN_6)
  int pressed;         // 1 is pressed, 0 is not pressed
} BUTTON_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int button_pressed(BUTTON_HANDLE *handle);

// --------------------------------------------------------------------------
// HAL Weak Callback re define
// --------------------------------------------------------------------------
#endif
