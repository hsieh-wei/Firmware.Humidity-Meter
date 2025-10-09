#ifndef __led_H__
#define __led_H__

#include "stm32f4xx_hal_gpio.h"

void led_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void led_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void led_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif