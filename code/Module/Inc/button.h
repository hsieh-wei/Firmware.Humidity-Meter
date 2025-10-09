#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f4xx_hal.h"

int button_pressed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
