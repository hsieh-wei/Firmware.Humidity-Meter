#include <string.h>
#include "pc_link.h"

// --------------------------------------------------------------------------
// Global Variable
// --------------------------------------------------------------------------
PC_LINK_HANDLE g_pc_link_handle;

// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------
static inline void pc_link_disable_dma_half_it(UART_HandleTypeDef *huart) {
    if (huart && huart->hdmarx) {
        __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);  // turn off DMA Half-Transfer inerrupt
    }
}

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int pc_link_init(PC_LINK_HANDLE *handle)
{
    if (!handle || !handle->huart) {
        return PC_LINK_ERROR;
    }
    static uint8_t g_pc_link_buf_rx[PC_LINK_RX_BUF_SIZE];
    static uint8_t g_pc_link_buf_tx[PC_LINK_TX_BUF_SIZE];

    // initial variable
    handle->rx_buf     = g_pc_link_buf_rx;
    handle->rx_buf_len = sizeof(g_pc_link_buf_rx);
    handle->tx_buf     = g_pc_link_buf_tx;
    handle->tx_buf_len = sizeof(g_pc_link_buf_tx);
    handle->tx_busy = 0;

    //turn off Half-Transfer IT
    pc_link_disable_dma_half_it(handle->huart);
    return PC_LINK_SUCCESS;
}

int pc_link_rx_dma(PC_LINK_HANDLE *handle)
{
    if (!handle || !handle->huart || !handle->rx_buf || handle->rx_buf_len == 0) {
        return PC_LINK_ERROR;
    }

    //turn off Half-Transfer IT
    pc_link_disable_dma_half_it(handle->huart);

    HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(handle->huart, handle->rx_buf, handle->rx_buf_len);
    return (status == HAL_OK) ? PC_LINK_SUCCESS : PC_LINK_ERROR;
}

int pc_link_tx_dma(PC_LINK_HANDLE *handle, const uint8_t *data, uint16_t len)
{
    if (!handle || !handle->huart || !handle->tx_buf) return PC_LINK_ERROR;
    if (handle->tx_busy) return PC_LINK_ERROR; // tx is busy

    handle->tx_busy = 1; // set tx busy

    memcpy(handle->tx_buf, data, len);

    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(handle->huart, handle->tx_buf, len);
    if (status != HAL_OK) {
        handle->tx_busy = 0; // if fail clead busy
        return PC_LINK_ERROR;
    }
    return PC_LINK_SUCCESS;
}


// ----------------------------------------------------------------------------
// HAL Weak Callback re define 
// ----------------------------------------------------------------------------
void pc_link_irq_rx_event(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart, uint16_t size)
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

void pc_link_irq_tx_cplt(PC_LINK_HANDLE *handle, UART_HandleTypeDef *huart)
{
    // Check whether it is the specified handler
    if (handle && handle->huart == huart) {
        // clear busy after send message
        handle->tx_busy = 0;
    }
}
