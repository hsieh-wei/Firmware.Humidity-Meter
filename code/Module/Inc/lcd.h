#ifndef LCD_H
#define LCD_H

#include "lcd_font.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

// --------------------------------------------------------------------------
// Error Codes 
// --------------------------------------------------------------------------
typedef enum {
    LCD_SUCCESS  = 0,
    LCD_ERROR    = -1,
} LCD_ERR_t;

// --------------------------------------------------------------------------
// Handle
// --------------------------------------------------------------------------
typedef struct {
    GPIO_TypeDef *gpiox;  // gpio HAL handle (EX: &GPIOA) 
    uint16_t gpio_pin;    // gpio HAL pin (EX: GPIO_PIN_6) 
} LCD_Control_Pin_HANDLE;

typedef struct {
    SPI_HandleTypeDef *hspi;    // spi HAL handle (EX: &hspi1)
    LCD_Control_Pin_HANDLE rst; // reset
    LCD_Control_Pin_HANDLE dc;  // 0 command, 1 parameter and data
    LCD_Control_Pin_HANDLE cs;  // chip select
    LCD_Control_Pin_HANDLE blk; // LCD backlit modify 
    uint8_t tx_buf[32];         // LCD cmd buffer
} LCD_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
// Color in RGB565
#define LCD_COLOR_RED     0xF800
#define LCD_COLOR_ORANGE  0xFB40
#define LCD_COLOR_YELLOW  0xFFE0
#define LCD_COLOR_GREEN   0x07E0
#define LCD_COLOR_BLUE    0x001F
#define LCD_COLOR_PURPLE  0x901F
#define LCD_COLOR_BLACK   0x0000
#define LCD_COLOR_WHITE   0xFFFF
#define LCD_COLOR_GRAY    0x8410

int lcd_init(LCD_HANDLE *handle);
int lcd_fill_screen(LCD_HANDLE *handle, uint16_t color); // color can using below, Ex: LCD_COLOR_RED
// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
#endif // LCD_H 
