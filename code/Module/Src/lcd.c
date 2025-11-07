#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include <stdint.h>

// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------
//LCD screen size
#define WIDTH_X   160
#define HEIGHT_Y  128

static int lcd_send_cmd(LCD_HANDLE *handle, uint8_t cmd)
{
    handle->tx_buf[0] = cmd;
    HAL_GPIO_WritePin(handle->dc.gpiox, handle->dc.gpio_pin, GPIO_PIN_RESET);   // dc send command
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_RESET);   // cs low, ready to transmit

    //TCSS, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually. 

    if(HAL_SPI_Transmit(handle->hspi, handle->tx_buf, 1, 100) != HAL_OK) return LCD_ERROR;

    // TCSH, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);     // cs high, stop transmit

    // TCHW, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    return LCD_SUCCESS;
}

static int lcd_send_data(LCD_HANDLE *handle, uint8_t data) // also can using in send parameter
{
    handle->tx_buf[0] = data;
    HAL_GPIO_WritePin(handle->dc.gpiox, handle->dc.gpio_pin, GPIO_PIN_SET);     // dc send data
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_RESET);   // cs low, ready to transmit

    //TCSS, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually. 

    if(HAL_SPI_Transmit(handle->hspi, handle->tx_buf, 1, 100) != HAL_OK) return LCD_ERROR;

    // TCSH, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);     // cs high, stop transmit

    // TCHW, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    return LCD_SUCCESS;
}

static int lcd_set_coordinate(LCD_HANDLE *handle, uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end)
{
    // CASET, set write data begin point(x)
    if (lcd_send_cmd(handle, 0x2A) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, (uint8_t)(x_start >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR; // parameter 1
    if (lcd_send_data(handle, (uint8_t)x_start & 0xFF) != LCD_SUCCESS) return LCD_ERROR;        // parameter 2
    if (lcd_send_data(handle, (uint8_t)(x_end >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR;   // parameter 3
    if (lcd_send_data(handle, (uint8_t)x_end & 0xFF) != LCD_SUCCESS) return LCD_ERROR;          // parameter 4

    // RASET, set write data begin point(y)
    if (lcd_send_cmd(handle, 0x2B) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, (uint8_t)(y_start >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR; // parameter 1
    if (lcd_send_data(handle, (uint8_t)y_start & 0xFF) != LCD_SUCCESS) return LCD_ERROR;        // parameter 2
    if (lcd_send_data(handle, (uint8_t)(y_end >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR;   // parameter 3
    if (lcd_send_data(handle, (uint8_t)y_end & 0xFF) != LCD_SUCCESS) return LCD_ERROR;          // parameter 4

    return LCD_SUCCESS;
}
// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int lcd_init(LCD_HANDLE *handle)
{
    if (!handle || !handle->hspi){
        return LCD_ERROR;
    }

    // Initial Control Pin
    HAL_GPIO_WritePin(handle->rst.gpiox, handle->rst.gpio_pin, GPIO_PIN_SET);   // unreset status
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);     // cs high, stop transmit
    HAL_GPIO_WritePin(handle->blk.gpiox, handle->blk.gpio_pin, GPIO_PIN_SET);   // blk light status

    // Hardware Reset
    HAL_GPIO_WritePin(handle->rst.gpiox, handle->rst.gpio_pin, GPIO_PIN_RESET);
    HAL_Delay(1); // ensure reset pulse duration > 10us
    HAL_GPIO_WritePin(handle->rst.gpiox, handle->rst.gpio_pin, GPIO_PIN_SET);
    HAL_Delay(5); // It is necessary to wait 5m sec after releasing RESX before sending commands.

    // SWRESET, Software Reset
    if (lcd_send_cmd(handle, 0x01) != LCD_SUCCESS) return LCD_ERROR;
    HAL_Delay(120); // no matter what mode,it will be necessary to wait 120ms before sending next command

    // SLPOUT, Turn into sleep out mode
    if (lcd_send_cmd(handle, 0x11) != LCD_SUCCESS) return LCD_ERROR;
    HAL_Delay(120); // no matter what mode,it will be necessary to wait 120ms before sending next command

    // MADCTL, Memory access control
    if (lcd_send_cmd(handle, 0x36) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, 0xA0) != LCD_SUCCESS) return LCD_ERROR; // coordinate:xy exchange + y mirror, reflash:top→bottom & left→right, color:RGB

    // COLMOD, Set Interface Pixel Format as 16bit (2 byte per pixel)
    if (lcd_send_cmd(handle, 0x3A) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, 0x05) != LCD_SUCCESS) return LCD_ERROR; // COLMOD Parameter, IFPF 16-bit/pixel

    // DISPON, Turn into display On mode
    if (lcd_send_cmd(handle, 0x29) != LCD_SUCCESS) return LCD_ERROR;

    return LCD_SUCCESS;
}

int lcd_fill_screen(LCD_HANDLE *handle, uint16_t color)
{
    if (!handle || !handle->hspi){
        return LCD_ERROR;
    }

    // set full screen coordinate
    if (lcd_set_coordinate(handle, 0, WIDTH_X-1, 0, HEIGHT_Y-1) != LCD_SUCCESS) return LCD_ERROR;
    
    // RAMWR Memory Write 
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t color_high_bit = (color >> 8) & 0xFF;
    uint8_t color_low_bit = color & 0xFF;
    for (int i=0; i < HEIGHT_Y*WIDTH_X; i++) {
        if (lcd_send_data(handle, color_high_bit) != LCD_SUCCESS) return LCD_ERROR;   // high bit of color
        if (lcd_send_data(handle, color_low_bit) != LCD_SUCCESS) return LCD_ERROR;    // low bit of color
    }
    return LCD_SUCCESS;
}

int lcd_print_font(LCD_HANDLE *handle, char font, const LCD_FONT_HANDLE *font_lookup_table, uint16_t x_start, uint16_t y_start, uint16_t font_color, uint16_t background_color)
{
    if (!handle || !handle->hspi){
        return LCD_ERROR;
    }

    // set full screen coordinate
    if (lcd_set_coordinate(handle, x_start, x_start+(font_lookup_table->width)-1, y_start,y_start+(font_lookup_table->height)-1) != LCD_SUCCESS) return LCD_ERROR;

    // RAMWR Memory Write 
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t font_color_high_bit = (font_color >> 8) & 0xFF;
    uint8_t font_color_low_bit = font_color & 0xFF;
    uint8_t background_color_high_bit = (background_color >> 8) & 0xFF;
    uint8_t background_color_low_bit = background_color & 0xFF;
    int font_index = (font-0x20)*font_lookup_table->height; // ascii code "space" is 0x20, and lookup table is start at "space"
    // start to print font
    for (int y = 0; y < font_lookup_table->height; y++) {
        uint16_t row_bit = font_lookup_table->data[font_index]; // row bit which is 7~16bit x data
        for (int x = 0; x < font_lookup_table->width; x++) {
            int bit = 15 - x; // start bit is from msb
            if ((row_bit >> bit) & 1) {
                // print font color
                if (lcd_send_data(handle, font_color_high_bit) != LCD_SUCCESS) return LCD_ERROR;
                if (lcd_send_data(handle, font_color_low_bit)  != LCD_SUCCESS) return LCD_ERROR;
            } else {
                // print background color
                if (lcd_send_data(handle, background_color_high_bit) != LCD_SUCCESS) return LCD_ERROR;
                if (lcd_send_data(handle, background_color_low_bit)  != LCD_SUCCESS) return LCD_ERROR;
            }
        }
        font_index++; //next row
    }
    return LCD_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
