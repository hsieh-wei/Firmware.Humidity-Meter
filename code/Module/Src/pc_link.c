#include "pc_link.h"
#include <string.h>

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------
static inline void pc_link_disable_dma_half_it(UART_HandleTypeDef *huart) {
    if (huart && huart->hdmarx) {
        __HAL_DMA_DISABLE_IT(huart->hdmarx,
                             DMA_IT_HT);  // turn off DMA Half-Transfer inerrupt
    }
}

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int pc_link_init(PC_LINK_HANDLE *handle) {
    if (!handle || !handle->huart || !handle->rx_buf || handle->rx_buf_len == 0 || !handle->tx_buf || handle->tx_buf_len == 0) {
        return PC_LINK_ERROR;
    }

    // iniial tx complete semaphore
    if (handle->tx_complete_semaphore == NULL) {
        handle->tx_complete_semaphore = xSemaphoreCreateBinary();
        if (handle->tx_complete_semaphore == NULL) {
            return PC_LINK_ERROR;
        }
    }

    // **** using in bare metal ****
    // handle->tx_busy = 0;
    // ****************************

    return PC_LINK_SUCCESS;
}

int pc_link_rx_dma(PC_LINK_HANDLE *handle) {
    if (!handle || !handle->huart || !handle->rx_buf || handle->rx_buf_len == 0) {
        return PC_LINK_ERROR;
    }

    if (HAL_UARTEx_ReceiveToIdle_DMA(handle->huart, handle->rx_buf, handle->rx_buf_len) != HAL_OK) {
        return PC_LINK_ERROR;
    }
    return PC_LINK_SUCCESS;
}

int pc_link_tx_dma(PC_LINK_HANDLE *handle, const uint8_t *data, uint16_t len) {
    if (!handle || !handle->huart || !handle->tx_buf) return PC_LINK_ERROR;

    // **** using in bare metal ****
    // if (handle->tx_busy) return PC_LINK_ERROR;  // tx is busy
    // handle->tx_busy = 1;  // set tx busy
    // ****************************

    memcpy(handle->tx_buf, data, len);

    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(handle->huart, handle->tx_buf, len);
    if (status != HAL_OK) {
        handle->tx_busy = 0;  // if fail clead busy
        return PC_LINK_ERROR;
    }
    return PC_LINK_SUCCESS;
}

// ----------------------------------------------------------------------------
// HAL Weak Callback re define
// ----------------------------------------------------------------------------
void pc_link_uart_tx_cplt(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart) {
    // Check whether it is the specified handler
    if (handle && handle->huart == huart) {
        // clear busy after send message
        handle->tx_busy = 0;
    }
}

void pc_link_uartex_rx_event(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart, uint16_t size)  // rx event is able to change input length
{
    // Check whether it is the specified handler
    if (!handle || huart != handle->huart) return;

    // Check whether tx_buf is exist
    if (!handle->tx_buf || handle->tx_buf_len == 0) return;

    // restart rx
    (void)pc_link_rx_dma(handle);

    // echo rx data if tx_buf has not written
    if (handle->tx_busy == 0) {
        uint16_t len = size;
        if (len > handle->tx_buf_len) len = handle->tx_buf_len;
        (void)pc_link_tx_dma(handle, handle->rx_buf, len);
    }
}
