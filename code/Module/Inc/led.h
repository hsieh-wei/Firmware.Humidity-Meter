#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx_hal_gpio.h"

void Led_On(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void Led_Off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void Led_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif