#include "button.h"

int GPIO_Button_Pressed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
    if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET){
        return 1;
    }
    else{
        return 0;
    }
}