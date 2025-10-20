#ifndef PC_LINK_H
#define PC_LINK_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

// --------------------------------------------------------------------------
// Error Codes 
// --------------------------------------------------------------------------
typedef enum {
    PC_LINK_SUCCESS  = 0,
    PC_LINK_ERROR    = -1,
} PC_LINK_ERR_t;

// --------------------------------------------------------------------------
// Handle
// --------------------------------------------------------------------------
typedef struct {
    UART_HandleTypeDef *huart;  // UART HAL handle（EX：&huart3） 
    uint8_t  *rx_buf;           // rx buffer DMA 
    uint16_t  rx_buf_len;       // rx buffer length 
    uint8_t  *tx_buf;           // tx buffer DMA 
    uint16_t  tx_buf_len;       // tx buffer length 
    volatile uint8_t busy_tx;   // driver using, do not change in main 
} PC_LINK_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int pc_link_init(PC_LINK_HANDLE *handle);
int pc_link_rx_dma(PC_LINK_HANDLE *handle);
int pc_link_tx_dma(PC_LINK_HANDLE *handle, const uint8_t *data, uint16_t len);

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
void pc_link_irq_rx_event(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart, uint16_t size);
void pc_link_irq_tx_cplt(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart);
#endif // PC_LINK_H 
