#ifndef LCD_H
#define LCD_H

#include "lcd_glyphs_table.h"
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
    uint8_t tx_buf[32];         // LCD cmd para data dma buffer, 16 pixel per time
    volatile int tx_busy;       // avoid send two tx message in one time 
} LCD_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
//LCD screen size(left upper is (0,0), x increase when go right, y increase when go down)
#define LCD_WIDTH_X   160
#define LCD_HEIGHT_Y  128

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

// color can using below, Ex: LCD_COLOR_RED
int lcd_fill_screen(LCD_HANDLE *handle, uint16_t color); 

//color can using below, font has three size(7X10,11x18,16x26), font can print from "space" to "~"(can reference ascii)
int lcd_print_font(LCD_HANDLE *handle, char font, const LCD_FONT_HANDLE *font_lookup_table, uint16_t x_start, uint16_t y_start, uint16_t font_color, uint16_t background_color);
int lcd_print_icon(LCD_HANDLE *handle, const LCD_ICON_HANDLE *lookup_table, uint16_t x_start, uint16_t y_start, uint16_t icon_color, uint16_t background_color);
// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
void lcd_irq_tx_cplt(LCD_HANDLE *handle, SPI_HandleTypeDef *hspi);
#endif // LCD_H 
