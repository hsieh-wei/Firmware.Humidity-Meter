#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "button.h"
#include "lcd.h"
#include "led.h"
#include "pc_link.h"
#include "sht30.h"
#include "stm32f4xx_hal.h"
#include "sys_timestamp.h"

// --------------------------------------------------------------------------
// Handle Define
// --------------------------------------------------------------------------
// led handle
extern LED_HANDLE g_led_handle_d2;
extern LED_HANDLE g_led_handle_d3;

// button handle
extern BUTTON_HANDLE g_button_handle_k1;
extern BUTTON_HANDLE g_button_handle_k0;

// pc_link handle
extern PC_LINK_HANDLE g_pc_link_handle;

// sht30 handle
extern SHT30_HANDLE g_sht30_handle;

// lcd handle
extern LCD_HANDLE g_lcd_handle;

// system timestamp handle
extern SYS_TIMESTAMP_HANDLE g_sys_timestamp_handle;

#endif