#include <string.h>
#include "pc_link.h"

// ---- internal helpers ---- 
static inline void pc_link_disable_dma_half_it(UART_HandleTypeDef *huart) {
    if (huart && huart->hdmarx) {
        __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);  // turn off DMA Half-Transfer inerrupt
    }
}

// ----------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int pc_link_init(PC_LINK_HANDLE *h)
{
    if (!h || !h->huart || !h->rx_buf || h->rx_len == 0) {
        return PC_LINK_ERROR;
    }

    h->busy_tx = 0;
    //turn off Half-Transfer IT
    pc_link_disable_dma_half_it(h->huart);
    return PC_LINK_SUCCESS;
}

int pc_link_rx(PC_LINK_HANDLE *h)
{
    if (!h || !h->huart || !h->rx_buf || h->rx_len == 0) {
        return PC_LINK_ERROR;
    }

    //turn off Half-Transfer IT
    pc_link_disable_dma_half_it(h->huart);

    HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(h->huart, h->rx_buf, h->rx_len);
    return (status == HAL_OK) ? PC_LINK_SUCCESS : PC_LINK_ERROR;
}

int pc_link_tx_dma(PC_LINK_HANDLE *h, const uint8_t *data, uint16_t len)
{
    if (!h || !h->huart || !data || len == 0) {
        return PC_LINK_ERROR;
    }
    if (h->busy_tx) {
        return PC_LINK_ERROR;  // tx is busy
    }

    h->busy_tx = 1;
    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(h->huart, (uint8_t *)data, len);
    if (status != HAL_OK) {
        h->busy_tx = 0;  // if fail clead busy
        return PC_LINK_ERROR;
    }
    return PC_LINK_SUCCESS;
}

// ----------------------------------------------------------------------------
// HAL Weak Callback re define 
// ----------------------------------------------------------------------------
void pc_link_irq_rx_event(PC_LINK_HANDLE *h, UART_HandleTypeDef *huart, uint16_t size)
{
    // Check whether it is the specified handler
    if (!h || huart != h->huart) {
        return;
    }

    if (size > 0) {
        // echo if there is tx_buf
        if (h->tx_buf && h->tx_len > 0) {
            memcpy(h->tx_buf, h->rx_buf, size);

            // If currently busy, this return is skipped
            if (!h->busy_tx) {
                (void)pc_link_tx_dma(h, h->tx_buf, size);
            }
        }
    }

    // restart rx
    (void)pc_link_rx(h);
}

void pc_link_irq_tx_cplt(PC_LINK_HANDLE *h, UART_HandleTypeDef *huart)
{
    // Check whether it is the specified handler
    if (h && h->huart == huart) {
        // clear busy after send message
        h->busy_tx = 0;
    }
}
