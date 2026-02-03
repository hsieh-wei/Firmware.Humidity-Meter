#include "pc_link.h"
#include <string.h>

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------
static inline void pc_link_disable_dma_half_it(UART_HandleTypeDef *huart) {
    if (huart && huart->hdmarx) {
        __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);  // turn off DMA Half-Transfer inerrupt
    }
}

static int pc_link_wait_tx_complete(PC_LINK_HANDLE *handle) {
    if (xSemaphoreTake(handle->tx_complete_semaphore, pdMS_TO_TICKS(100)) != pdTRUE) {
        return PC_LINK_TIMEOUT;
    }
    return PC_LINK_SUCCESS;
}

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int pc_link_tx_init(PC_LINK_HANDLE *handle) {
    if (!handle || !handle->huart || !handle->rx_buf || handle->rx_buf_len == 0 || !handle->tx_buf || handle->tx_buf_len == 0) {
        return PC_LINK_ERROR;
    }

    // intial tx complete semaphore
    if (handle->tx_complete_semaphore == NULL) {
        handle->tx_complete_semaphore = xSemaphoreCreateBinary();
        if (handle->tx_complete_semaphore == NULL) {
            return PC_LINK_ERROR;
        }
        xSemaphoreGive(handle->tx_complete_semaphore);
    }

    pc_link_disable_dma_half_it(handle->huart);

    // **** using in bare metal ****
    // handle->tx_busy = 0;
    // ****************************

    return PC_LINK_SUCCESS;
}

int pc_link_rx_init(PC_LINK_HANDLE *handle) {
    if (!handle || !handle->huart || !handle->rx_buf || handle->rx_buf_len == 0 || !handle->tx_buf || handle->tx_buf_len == 0) {
        return PC_LINK_ERROR;
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

    if (handle->tx_buf_len < len) return PC_LINK_ERROR;

    // **** using in bare metal ****
    // if (handle->tx_busy) return PC_LINK_ERROR;  // tx is busy
    // handle->tx_busy = 1;  // set tx busy
    // ****************************

    if (pc_link_wait_tx_complete(handle) != PC_LINK_SUCCESS) {
        return PC_LINK_ERROR;
    }

    memcpy(handle->tx_buf, data, len);

    if (HAL_UART_Transmit_DMA(handle->huart, handle->tx_buf, len) != HAL_OK) {
        // **** using in bare metal ****
        // handle->tx_busy = 0;  // if fail clead busy
        // ****************************
        xSemaphoreGive(handle->tx_complete_semaphore);
        return PC_LINK_ERROR;
    }
    return PC_LINK_SUCCESS;
}

// ----------------------------------------------------------------------------
// HAL Weak Callback re define
// ----------------------------------------------------------------------------
void pc_link_uartex_rx_event(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart, uint16_t size)  // rx event is able to change input length
{
    // Check whether it is the specified handler
    if (!handle || huart != handle->huart) return;

    // TODO

    // restart rx
    (void)pc_link_rx_dma(handle);
}

void pc_link_uart_tx_cplt(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart) {
    // Check whether it is the specified handler
    if (handle && handle->huart == huart) {
        // **** using in bare metal ****
        // handle->tx_busy = 0;
        // ****************************
        BaseType_t yield = pdFALSE;
        xSemaphoreGiveFromISR(handle->tx_complete_semaphore, &yield);
        portYIELD_FROM_ISR(yield);
    }
}
