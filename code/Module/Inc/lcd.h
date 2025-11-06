#ifndef LCD_H
#define LCD_H

#include "stm32f4xx_hal.h"

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
#define RED     0xf800
#define ORANGE  0xfb40
#define YELLOW  0xffe0
#define GREEN   0x07e0
#define BLUE    0x001f
#define PURPLE  0x901f
#define BLACK   0x0000
#define WHITE   0xffff
#define GRAY    0xf800

int lcd_init(LCD_HANDLE *handle);

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
#endif // LCD_H 
