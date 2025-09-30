#ifndef __PC__LINK__H__
#define __PC__LINK__H__

#include "stm32f4xx.h" 
#include "usart.h"
/*===========================================================
Global massage
===========================================================*/
extern uint8_t pc_link_massage[50];

/*===========================================================
API
===========================================================*/
void PC_LINK_Get_Data();

void PC_LINK_Send_Data();

/*===========================================================
HAL_UART_IRQHandler Re Declare
===========================================================*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
#endif