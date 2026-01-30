#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_tim.h"
#include <stdint.h>
#include <string.h>

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------
static int lcd_wait_tx_complete(LCD_HANDLE *handle) {
    // timeout means semaphore take will wait maximum of xxms
    // if tx complete less than 15ms, return SHT30_SUCCESS
    // if tx complete more than 15ms, return SHT30_TIMEOUT
    if (xSemaphoreTake(handle->tx_complete_semaphore, pdMS_TO_TICKS(20)) != pdTRUE) {
        return LCD_TIMEOUT;
    }
    return LCD_SUCCESS;
}

static int lcd_set_ccr(LCD_HANDLE *handle, uint32_t value) {  // ccr is capture/compare register using in pwm
    if (!handle || !handle->blk.htim) return LCD_ERROR;

    switch (handle->blk.channel) {
        case TIM_CHANNEL_1:
            handle->blk.htim->Instance->CCR1 = value;
            handle->blk.brightness_value = value;
            return LCD_SUCCESS;

        case TIM_CHANNEL_2:
            handle->blk.htim->Instance->CCR2 = value;
            handle->blk.brightness_value = value;
            return LCD_SUCCESS;

        case TIM_CHANNEL_3:
            handle->blk.htim->Instance->CCR3 = value;
            handle->blk.brightness_value = value;
            return LCD_SUCCESS;

        case TIM_CHANNEL_4:
            handle->blk.htim->Instance->CCR4 = value;
            handle->blk.brightness_value = value;
            return LCD_SUCCESS;

        default:
            return LCD_ERROR;
    }
}

static int lcd_send_cmd(LCD_HANDLE *handle, uint8_t cmd) {
    handle->tx_buf[0] = cmd;
    HAL_GPIO_WritePin(handle->dc.gpiox, handle->dc.gpio_pin, GPIO_PIN_RESET);  // dc send command
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_RESET);  // cs low, ready to transmit

    // TCSS, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    if (HAL_SPI_Transmit(handle->hspi, handle->tx_buf, 1, 100) != HAL_OK) return LCD_ERROR;

    // TCSH, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);  // cs high, stop transmit

    // TCHW, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    return LCD_SUCCESS;
}

static int lcd_send_data(LCD_HANDLE *handle, uint8_t data)  // also can using in send parameter
{
    handle->tx_buf[0] = data;
    HAL_GPIO_WritePin(handle->dc.gpiox, handle->dc.gpio_pin, GPIO_PIN_SET);    // dc send data
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_RESET);  // cs low, ready to transmit

    // TCSS, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    if (HAL_SPI_Transmit(handle->hspi, handle->tx_buf, 1, 100) != HAL_OK) return LCD_ERROR;

    // TCSH, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);  // cs high, stop transmit

    // TCHW, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    return LCD_SUCCESS;
}

static int lcd_send_data_dma(LCD_HANDLE *handle,
                             uint16_t data_size)  // also can using in send parameter, data_size indicates how much buffer is used.
{
    HAL_GPIO_WritePin(handle->dc.gpiox, handle->dc.gpio_pin, GPIO_PIN_SET);    // dc send data
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_RESET);  // cs low, ready to transmit

    // TCSS, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

    // **** using in bare metal ****
    // handle->tx_busy = 1;  // set tx busy
    // if (HAL_SPI_Transmit_DMA(handle->hspi, handle->tx_buf, (uint16_t)data_size) != HAL_OK) {
    //     handle->tx_busy = 0;  // clear tx busy
    //     HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);
    //     return LCD_ERROR;
    // }
    // ****************************

    if (HAL_SPI_Transmit_DMA(handle->hspi, handle->tx_buf, (uint16_t)data_size) != HAL_OK) {
        xSemaphoreGive(handle->tx_complete_semaphore);
        HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);
        return LCD_ERROR;
    }
    // TCSH, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.
    return LCD_SUCCESS;
}

static int lcd_set_coordinate(LCD_HANDLE *handle, uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end) {
    // CASET, set write data begin point(x)
    if (lcd_send_cmd(handle, 0x2A) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, (uint8_t)(x_start >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR;  // parameter 1
    if (lcd_send_data(handle, (uint8_t)x_start & 0xFF) != LCD_SUCCESS) return LCD_ERROR;         // parameter 2
    if (lcd_send_data(handle, (uint8_t)(x_end >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR;    // parameter 3
    if (lcd_send_data(handle, (uint8_t)x_end & 0xFF) != LCD_SUCCESS) return LCD_ERROR;           // parameter 4

    // RASET, set write data begin point(y)
    if (lcd_send_cmd(handle, 0x2B) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, (uint8_t)(y_start >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR;  // parameter 1
    if (lcd_send_data(handle, (uint8_t)y_start & 0xFF) != LCD_SUCCESS) return LCD_ERROR;         // parameter 2
    if (lcd_send_data(handle, (uint8_t)(y_end >> 8) & 0xFF) != LCD_SUCCESS) return LCD_ERROR;    // parameter 3
    if (lcd_send_data(handle, (uint8_t)y_end & 0xFF) != LCD_SUCCESS) return LCD_ERROR;           // parameter 4

    return LCD_SUCCESS;
}
// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int lcd_init(LCD_HANDLE *handle) {
    if (!handle || !handle->hspi) {
        return LCD_ERROR;
    }

    // iniial tx complete semaphore
    if (handle->tx_complete_semaphore == NULL) {
        handle->tx_complete_semaphore = xSemaphoreCreateBinary();
        if (handle->tx_complete_semaphore == NULL) {
            return LCD_ERROR;
        }
    }
    xSemaphoreGive(handle->tx_complete_semaphore);

    // Initial Control Pin
    HAL_GPIO_WritePin(handle->rst.gpiox, handle->rst.gpio_pin, GPIO_PIN_SET);                  // unreset status
    HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);                    // cs high, stop transmit
    if (HAL_TIM_PWM_Start(handle->blk.htim, handle->blk.channel) != HAL_OK) return LCD_ERROR;  // start pwm
    if (lcd_set_ccr(handle, (uint32_t)100) != LCD_SUCCESS) return LCD_ERROR;                   // blk light status

    // Hardware Reset
    HAL_GPIO_WritePin(handle->rst.gpiox, handle->rst.gpio_pin, GPIO_PIN_RESET);
    // HAL_Delay(1);  // ensure reset pulse duration > 10us
    vTaskDelay(pdMS_TO_TICKS(1));
    HAL_GPIO_WritePin(handle->rst.gpiox, handle->rst.gpio_pin, GPIO_PIN_SET);
    // HAL_Delay(5);  // It is necessary to wait 5m sec after releasing RESX before sending commands.
    vTaskDelay(pdMS_TO_TICKS(5));

    // SWRESET, Software Reset
    if (lcd_send_cmd(handle, 0x01) != LCD_SUCCESS) return LCD_ERROR;
    // HAL_Delay(120);  // no matter what mode,it will be necessary to wait 120ms before sending next command
    vTaskDelay(pdMS_TO_TICKS(120));

    // SLPOUT, Turn into sleep out mode
    if (lcd_send_cmd(handle, 0x11) != LCD_SUCCESS) return LCD_ERROR;
    // HAL_Delay(120);  // no matter what mode,it will be necessary to wait 120ms before sending next command
    vTaskDelay(pdMS_TO_TICKS(120));

    // MADCTL, Memory access control
    if (lcd_send_cmd(handle, 0x36) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, 0xA0) != LCD_SUCCESS)
        return LCD_ERROR;  // coordinate:xy exchange + y mirror, reflash:top→bottom & left→right, color:RGB

    // COLMOD, Set Interface Pixel Format as 16bit (2 byte per pixel)
    if (lcd_send_cmd(handle, 0x3A) != LCD_SUCCESS) return LCD_ERROR;
    if (lcd_send_data(handle, 0x05) != LCD_SUCCESS) return LCD_ERROR;  // COLMOD Parameter, IFPF 16-bit/pixel

    // DISPON, Turn into display On mode
    if (lcd_send_cmd(handle, 0x29) != LCD_SUCCESS) return LCD_ERROR;

    return LCD_SUCCESS;
}

int lcd_adjust_backlight(LCD_HANDLE *handle, uint32_t value) {
    if (!handle || !handle->blk.htim) return LCD_ERROR;

    // avoid unreasonable backlight
    if ((int)value > 100U) return LCD_ERROR;

    // set ccr to change pwm pulse
    if (lcd_set_ccr(handle, value) != LCD_SUCCESS) return LCD_ERROR;

    return LCD_SUCCESS;
}

int lcd_fill_screen(LCD_HANDLE *handle, uint16_t color) {
    if (!handle || !handle->hspi) return LCD_ERROR;

    // set coordinate
    if (lcd_set_coordinate(handle, 0, LCD_WIDTH_X - 1, 0, LCD_HEIGHT_Y - 1) != LCD_SUCCESS) return LCD_ERROR;

    // RAMWR Memory Write
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t color_high_bit = (color >> 8) & 0xFF;
    uint8_t color_low_bit = color & 0xFF;
    for (int i = 0; i < LCD_WIDTH_X * LCD_HEIGHT_Y; i++) {
        if (lcd_send_data(handle, color_high_bit) != LCD_SUCCESS) return LCD_ERROR;  // high bit of color
        if (lcd_send_data(handle, color_low_bit) != LCD_SUCCESS) return LCD_ERROR;   // low bit of color
    }
    return LCD_SUCCESS;
}

int lcd_fill_screen_dma(LCD_HANDLE *handle, uint16_t color) {
    if (!handle || !handle->hspi) return LCD_ERROR;

    // **** using in bare metal ****
    // if (handle->tx_busy == 1) return LCD_ERROR;
    // ****************************
    if (lcd_wait_tx_complete(handle) != LCD_SUCCESS) {
        return LCD_TIMEOUT;
    }

    // set coordinate
    if (lcd_set_coordinate(handle, 0, LCD_WIDTH_X - 1, 0, LCD_HEIGHT_Y - 1) != LCD_SUCCESS) return LCD_ERROR;

    // RAMWR Memory Write
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t color_high_bit = (color >> 8) & 0xFF;
    uint8_t color_low_bit = color & 0xFF;
    for (int i = 0; i < LCD_WIDTH_X * LCD_HEIGHT_Y * 2; i++) {
        if (i % 2 == 0)
            handle->tx_buf[i] = color_high_bit;
        else
            handle->tx_buf[i] = color_low_bit;
    }

    if (lcd_send_data_dma(handle, (uint16_t)sizeof(handle->tx_buf)) != LCD_SUCCESS) return LCD_ERROR;

    return LCD_SUCCESS;
}

int lcd_print_font(LCD_HANDLE *handle, char font, const LCD_FONT_HANDLE *lookup_table, uint16_t x_start, uint16_t y_start, uint16_t font_color,
                   uint16_t background_color) {
    if (!handle || !handle->hspi) return LCD_ERROR;

    // set coordinate
    uint16_t x_end = x_start + lookup_table->width - 1;
    uint16_t y_end = y_start + lookup_table->height - 1;
    if (x_start >= LCD_WIDTH_X || y_start >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (x_end >= LCD_WIDTH_X || y_end >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (lcd_set_coordinate(handle, x_start, x_end, y_start, y_end) != LCD_SUCCESS) return LCD_ERROR;

    // RAMWR Memory Write
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t font_color_high_bit = (font_color >> 8) & 0xFF;
    uint8_t font_color_low_bit = font_color & 0xFF;
    uint8_t background_color_high_bit = (background_color >> 8) & 0xFF;
    uint8_t background_color_low_bit = background_color & 0xFF;
    int font_index = (font - 0x20) * lookup_table->height;  // ascii code "space" is 0x20, and lookup table is start at "space"
    // start to print font
    for (int y = 0; y < lookup_table->height; y++) {
        uint16_t row_bit = lookup_table->data[font_index];  // row bit which is x data
        for (int x = 0; x < lookup_table->width; x++) {
            int bit_index = 15 - x;  // start bit is from msb
            if ((row_bit >> bit_index) & 1) {
                // print font color
                if (lcd_send_data(handle, font_color_high_bit) != LCD_SUCCESS) return LCD_ERROR;
                if (lcd_send_data(handle, font_color_low_bit) != LCD_SUCCESS) return LCD_ERROR;
            } else {
                // print background color
                if (lcd_send_data(handle, background_color_high_bit) != LCD_SUCCESS) return LCD_ERROR;
                if (lcd_send_data(handle, background_color_low_bit) != LCD_SUCCESS) return LCD_ERROR;
            }
        }
        font_index++;  // next row
    }
    return LCD_SUCCESS;
}

int lcd_print_font_dma(LCD_HANDLE *handle, char font, const LCD_FONT_HANDLE *lookup_table, uint16_t x_start, uint16_t y_start, uint16_t font_color,
                       uint16_t background_color) {
    if (!handle || !handle->hspi) return LCD_ERROR;

    // **** using in bare metal ****
    // if (handle->tx_busy == 1) return LCD_ERROR;
    // ****************************
    if (lcd_wait_tx_complete(handle) != LCD_SUCCESS) {
        return LCD_TIMEOUT;
    }

    // set coordinate
    uint16_t x_end = x_start + lookup_table->width - 1;
    uint16_t y_end = y_start + lookup_table->height - 1;
    if (x_start >= LCD_WIDTH_X || y_start >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (x_end >= LCD_WIDTH_X || y_end >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (lcd_set_coordinate(handle, x_start, x_end, y_start, y_end) != LCD_SUCCESS) return LCD_ERROR;

    // RAMWR Memory Write
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t font_color_high_bit = (font_color >> 8) & 0xFF;
    uint8_t font_color_low_bit = font_color & 0xFF;
    uint8_t background_color_high_bit = (background_color >> 8) & 0xFF;
    uint8_t background_color_low_bit = background_color & 0xFF;
    int font_index = (font - 0x20) * lookup_table->height;  // ascii code "space" is 0x20, and lookup table is start at "space"
    // start to print font
    for (int y = 0; y < lookup_table->height; y++) {
        uint16_t row_bit = lookup_table->data[font_index];  // row bit which is x data
        for (int x = 0; x < lookup_table->width; x++) {
            int bit_index = 15 - x;  // start bit is from msb
            if ((row_bit >> bit_index) & 1) {
                // print font color
                handle->tx_buf[(y * lookup_table->width + x) * 2] = font_color_high_bit;
                handle->tx_buf[(y * lookup_table->width + x) * 2 + 1] = font_color_low_bit;
            } else {
                // print background color
                handle->tx_buf[(y * lookup_table->width + x) * 2] = background_color_high_bit;
                handle->tx_buf[(y * lookup_table->width + x) * 2 + 1] = background_color_low_bit;
            }
        }
        font_index++;  // next row
    }

    if ((lcd_send_data_dma(handle, (uint16_t)lookup_table->width * lookup_table->height * 2)) != LCD_SUCCESS) return LCD_ERROR;

    return LCD_SUCCESS;
}

int lcd_print_icon(LCD_HANDLE *handle, const LCD_ICON_HANDLE *lookup_table, uint16_t x_start, uint16_t y_start, uint16_t icon_color,
                   uint16_t background_color) {
    if (!handle || !handle->hspi) return LCD_ERROR;

    // set full screen coordinate
    uint16_t x_end = x_start + lookup_table->width - 1;
    uint16_t y_end = y_start + lookup_table->height - 1;
    if (x_start >= LCD_WIDTH_X || y_start >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (x_end >= LCD_WIDTH_X || y_end >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (lcd_set_coordinate(handle, x_start, x_end, y_start, y_end) != LCD_SUCCESS) return LCD_ERROR;

    // RAMWR Memory Write
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t icon_color_high_bit = (icon_color >> 8) & 0xFF;
    uint8_t icon_color_low_bit = icon_color & 0xFF;
    uint8_t background_color_high_bit = (background_color >> 8) & 0xFF;
    uint8_t background_color_low_bit = background_color & 0xFF;
    int bytes_per_row = (lookup_table->width + 7) / 8;  // count how mant bytes per row
    // start to print font
    for (int y = 0; y < lookup_table->height; y++) {
        for (int x = 0; x < lookup_table->width; x++) {
            int byte_index = x / 8;
            int bit_index = 7 - (x % 8);  // start bit is from msb
            if ((lookup_table->data[y * bytes_per_row + byte_index] >> bit_index) & 1) {
                // print font color
                if (lcd_send_data(handle, icon_color_high_bit) != LCD_SUCCESS) return LCD_ERROR;
                if (lcd_send_data(handle, icon_color_low_bit) != LCD_SUCCESS) return LCD_ERROR;
            } else {
                // print background color
                if (lcd_send_data(handle, background_color_high_bit) != LCD_SUCCESS) return LCD_ERROR;
                if (lcd_send_data(handle, background_color_low_bit) != LCD_SUCCESS) return LCD_ERROR;
            }
        }
    }
    return LCD_SUCCESS;
}

int lcd_print_icon_dma(LCD_HANDLE *handle, const LCD_ICON_HANDLE *lookup_table, uint16_t x_start, uint16_t y_start, uint16_t icon_color,
                       uint16_t background_color) {
    if (!handle || !handle->hspi) return LCD_ERROR;

    // **** using in bare metal ****
    // if (handle->tx_busy == 1) return LCD_ERROR;
    // ****************************
    if (lcd_wait_tx_complete(handle) != LCD_SUCCESS) {
        return LCD_TIMEOUT;
    }

    // set full screen coordinate
    uint16_t x_end = x_start + lookup_table->width - 1;
    uint16_t y_end = y_start + lookup_table->height - 1;
    if (x_start >= LCD_WIDTH_X || y_start >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (x_end >= LCD_WIDTH_X || y_end >= LCD_HEIGHT_Y) return LCD_ERROR;
    if (lcd_set_coordinate(handle, x_start, x_end, y_start, y_end) != LCD_SUCCESS) return LCD_ERROR;

    // RAMWR Memory Write
    if (lcd_send_cmd(handle, 0x2C) != LCD_SUCCESS) return LCD_ERROR;
    uint8_t icon_color_high_bit = (icon_color >> 8) & 0xFF;
    uint8_t icon_color_low_bit = icon_color & 0xFF;
    uint8_t background_color_high_bit = (background_color >> 8) & 0xFF;
    uint8_t background_color_low_bit = background_color & 0xFF;
    int bytes_per_row = (lookup_table->width + 7) / 8;  // count how mant bytes per row
    // start to print font
    for (int y = 0; y < lookup_table->height; y++) {
        for (int x = 0; x < lookup_table->width; x++) {
            int byte_index = x / 8;
            int bit_index = 7 - (x % 8);  // start bit is from msb
            if ((lookup_table->data[y * bytes_per_row + byte_index] >> bit_index) & 1) {
                // print font color
                handle->tx_buf[(y * lookup_table->width + x) * 2] = icon_color_high_bit;
                handle->tx_buf[(y * lookup_table->width + x) * 2 + 1] = icon_color_low_bit;
            } else {
                // print background color
                handle->tx_buf[(y * lookup_table->width + x) * 2] = background_color_high_bit;
                handle->tx_buf[(y * lookup_table->width + x) * 2 + 1] = background_color_low_bit;
            }
        }
    }
    if ((lcd_send_data_dma(handle, (uint16_t)lookup_table->width * lookup_table->height * 2)) != LCD_SUCCESS) return LCD_ERROR;
    return LCD_SUCCESS;
}
// --------------------------------------------------------------------------
// HAL Weak Callback re define
// --------------------------------------------------------------------------
void lcd_spi_tx_cplt(LCD_HANDLE *handle, SPI_HandleTypeDef *hspi) {
    if (handle && handle->hspi == hspi) {
        HAL_GPIO_WritePin(handle->cs.gpiox, handle->cs.gpio_pin, GPIO_PIN_SET);  // cs high, stop transmit
        // TCHW, Guaranteed by SPI timing and HAL, the program does not need to insert delays manually.

        // **** using in bare metal ****
        // handle->tx_busy = 0;
        // ****************************
        BaseType_t yield = pdFALSE;
        xSemaphoreGiveFromISR(handle->tx_complete_semaphore, &yield);
        portYIELD_FROM_ISR(yield);
    }
}