#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"

// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------
int lcd_send_cmd(LCD_HANDLE *handle, uint8_t cmd)
{
    *handle->tx_buf = cmd;
    HAL_GPIO_WritePin(handle->dc.gpiox, handle->dc.gpio_pin, GPIO_PIN_RESET);   // dc send command
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_RESET);   // cs low, ready to transmit
    HAL_Delay(1); // TCSS, Chip Select Setup Time (Write), 45ns
    if(HAL_SPI_Transmit(handle->hspi, handle->tx_buf, 1, 100) != HAL_OK) return LCD_ERROR;
    HAL_Delay(1); // TCSH, Chip Select Hold Time (Write), 60ns
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);   // cs high, stop transmit
    HAL_Delay(1); // TCHW, SCL “H” Pulse Width (Write), 15ns
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

    // Software Reset
    if (lcd_send_cmd(handle, 0x01) != LCD_SUCCESS) return LCD_ERROR;
    HAL_Delay(120); // no matter what mode,it will be necessary to wait 120ms before sending next command

    // Turn into sleep out mode
    if (lcd_send_cmd(handle, 0x11) != LCD_SUCCESS) return LCD_ERROR;

    // Turn into display On mode
    if (lcd_send_cmd(handle, 0x29) != LCD_SUCCESS) return LCD_ERROR;
    return LCD_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
