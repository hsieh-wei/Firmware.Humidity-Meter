#include "pc_link.h"

/*===========================================================
Global massage
===========================================================*/
uint8_t pc_link_massage[50];

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
        PC_LINK_Send_Data();
        PC_LINK_Get_Data();
    }
}