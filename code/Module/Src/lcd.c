#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"

// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------
// color in RGB565
#define RED     0xf800
#define ORANGE  0xfb40
#define YELLOW  0xffe0
#define GREEN   0x07e0
#define BLUE    0x001f
#define PURPLE  0x901f
#define BLACK   0x0000
#define WHITE   0xffff
#define GRAY    0xf800

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

    if(HAL_SPI_Transmit(handle->hspi, handle->tx_buf, 2, 100) != HAL_OK) return LCD_ERROR;

    // TCSH, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);     // cs high, stop transmit
    // TCHW, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.
    return LCD_SUCCESS;
}

static int lcd_set_coor(x0, y0, x1, y1)
{
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
    if (lcd_send_data(handle, 0x00) != LCD_SUCCESS) return LCD_ERROR; // coordinate:no flip/mirror/rotate, reflash:top→bottom & left→right, color:RGB

    // COLMOD, Set Interface Pixel Format as 16bit (2 byte per pixel)
    if (lcd_send_cmd(handle, 0x3A) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, 0x05) != LCD_SUCCESS) return LCD_ERROR; // COLMOD Parameter, IFPF 16-bit/pixel
    HAL_Delay(10); // just a buffer

    // DISPON, Turn into display On mode
    if (lcd_send_cmd(handle, 0x29) != LCD_SUCCESS) return LCD_ERROR;
    HAL_Delay(10); // just a buffer

    return LCD_SUCCESS;
}

int lcd_fill_screen(LCD_HANDLE *handle, int color)
{
    if (!handle || !handle->hspi){
        return LCD_ERROR;
    }

    // MADCTL, Memory access contro
    if (lcd_send_cmd(handle, 0x36) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, 0x00) != LCD_SUCCESS) return LCD_ERROR; // MH、ML

    // MADCTL, Memory access contro
    if (lcd_send_cmd(handle, 0x36) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, 0x00) != LCD_SUCCESS) return LCD_ERROR; // MH、ML

}

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
