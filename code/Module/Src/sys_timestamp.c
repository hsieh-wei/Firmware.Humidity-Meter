#include "sys_timestamp.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"

// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int sys_timestamp_init(SYS_TIMESTAMP_HANDLE *handle){
    if (!handle || !handle->htim){
        return SYS_TIMESTAMP_ERROR;
    }
    handle->timestamp = 0;
    HAL_TIM_Base_Start_IT(handle->htim); // start timer, interrupt per sec
    
    return SYS_TIMESTAMP_SUCCESS;
}

int sys_timestamp_get_current_timestamp(SYS_TIMESTAMP_HANDLE *handle){
    if (!handle || !handle->htim){
        return SYS_TIMESTAMP_ERROR;
    }
    handle->timestamp = handle->htim->Instance->CNT; // Reference Manual 20.4.6

    return SYS_TIMESTAMP_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
void sys_timestamp_tim_period_elapsed_callback(SYS_TIMESTAMP_HANDLE *handle, TIM_HandleTypeDef *htim)
{
    if(handle && handle->htim == htim){
        handle->timestamp ++;
    }
}