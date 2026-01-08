#include "button.h"
#include "lcd.h"
#include "led.h"
#include "pc_link.h"
#include "sht30.h"
#include "stm32f4xx_hal.h"
#include "sys_timestamp.h"

// led handle
LED_HANDLE g_led_handle_d2 = {
    .gpiox = GPIOA,
    .gpio_pin = GPIO_PIN_7,
};
LED_HANDLE g_led_handle_d3 = {
    .gpiox = GPIOA,
    .gpio_pin = GPIO_PIN_7,
};

// button handle
BUTTON_HANDLE g_button_handle_k1 = {
    .gpiox = GPIOE,
    .gpio_pin = GPIO_PIN_3,
};
BUTTON_HANDLE g_button_handle_k0 = {
    .gpiox = GPIOE,
    .gpio_pin = GPIO_PIN_4,
};

// pc_link handle
PC_LINK_HANDLE g_pc_link_handle = {
    .huart = &huart1,
};

// sht30 handle
SHT30_HANDLE g_sht30_handle = {
    .hi2c = &hi2c1,
};

// lcd handle
LCD_HANDLE g_lcd_handle = {
    .hspi = &hspi1,
    .rst = {GPIOC, GPIO_PIN_0},
    .dc = {GPIOC, GPIO_PIN_1},
    .cs = {GPIOC, GPIO_PIN_2},
    .blk =
        {
            &htim2,
            TIM_CHANNEL_1,
        },
};

// system timestamp handle
SYS_TIMESTAMP_HANDLE g_sys_timestamp_handle = {
    .htim = &htim6,
};