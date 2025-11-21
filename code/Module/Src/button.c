#include "button.h"
#include "led.h"
// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int button_pressed(BUTTON_HANDLE *handle)
{
    handle->pressed  = 0 ;
    if(HAL_GPIO_ReadPin(handle->gpiox, handle->gpio_pin) == GPIO_PIN_RESET){ // active-low
        handle->pressed  = 1 ;// button pressed
    }; 
    return BUTTON_SUCCESS;
}

void button_gpio_exti(BUTTON_HANDLE *handle, uint16_t GPIO_Pin){
    if(handle && handle->gpio_pin == GPIO_Pin){
        static LED_HANDLE s_led_handle ={
            .gpiox = GPIOA,
            .gpio_pin = GPIO_PIN_6,
        };
        led_toggle(&s_led_handle);
    }
}
