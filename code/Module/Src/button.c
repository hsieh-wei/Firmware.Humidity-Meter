#include "button.h"

// active-low
int Button_Pressed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET; 
}
