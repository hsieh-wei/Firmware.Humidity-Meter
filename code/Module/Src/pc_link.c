#include <string.h>
#include "pc_link.h"
#include "led.h"

/*===========================================================
Global massage
===========================================================*/
static uint8_t pc_link_rx_buffer[64];
static uint8_t pc_link_tx_buffer[64];

/*===========================================================
API
===========================================================*/
void PC_LINK_StartRx(void) {
    if (huart3.hdmarx) {
        __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
    }
    (void)HAL_UARTEx_ReceiveToIdle_DMA(&huart3, pc_link_rx_buffer, sizeof(pc_link_rx_buffer));
}
void PC_LINK_Send_Data(uint8_t *tx, uint16_t len) {
    HAL_UART_Transmit_DMA(&huart3, tx, len);
}

/*===========================================================
HAL_UART_IRQHandler Re Declare
===========================================================*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart == &huart3) {
        // copy rx message to tx message
        memcpy(pc_link_tx_buffer, pc_link_rx_buffer, Size);

        // send the tx message
        PC_LINK_Send_Data(pc_link_tx_buffer, Size);

        // restart rx
        PC_LINK_StartRx();
    }
}