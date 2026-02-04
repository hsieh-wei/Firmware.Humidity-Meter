// --------------------------------------------------------------------------
// This file define global infrastructure which is related with hardware driver
// --------------------------------------------------------------------------
#include "board_config.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

// --------------------------------------------------------------------------
// Config Board Usimg Peripheral
// --------------------------------------------------------------------------
// led handle
LED_HANDLE g_led_handle_d2 = {
    .gpiox = GPIOA,
    .gpio_pin = GPIO_PIN_6,
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
static uint8_t s_pc_rx_buffer[15];
static uint8_t s_pc_tx_buffer[256];
PC_LINK_HANDLE g_pc_link_handle = {
    .huart = &huart1,
    .rx_buf = s_pc_rx_buffer,
    .rx_buf_len = sizeof(s_pc_rx_buffer),
    .tx_buf = s_pc_tx_buffer,
    .tx_buf_len = sizeof(s_pc_tx_buffer),
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
    .timestamp = 0,
};