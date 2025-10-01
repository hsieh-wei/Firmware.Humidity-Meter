#include "pc_link.h"
#include "led.h"

/*===========================================================
Global massage
===========================================================*/
uint8_t pc_link_massage[64];

/*===========================================================
API
===========================================================*/
void PC_LINK_Get_Data(){
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, pc_link_massage, sizeof(pc_link_massage));
}

void PC_LINK_Send_Data(){
    HAL_UART_Transmit_DMA(&huart3, pc_link_massage, sizeof(pc_link_massage));
}

/*===========================================================
HAL_UART_IRQHandler Re Declare
===========================================================*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if (huart == &huart3){
        // PC_LINK_Send_Data();
        // PC_LINK_Get_Data();
        Led_Toggle(GPIOA, GPIO_PIN_7);
        HAL_UART_Transmit_DMA(&huart3, pc_link_massage, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, pc_link_massage, sizeof(pc_link_massage));
        // __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
    }
}