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
    uint8_t *tx_buf;            // LCD cmd buffer
} LCD_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int lcd_init(LCD_HANDLE *handle);

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
#endif // LCD_H 
