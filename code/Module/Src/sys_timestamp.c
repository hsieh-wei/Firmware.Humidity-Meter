#include "sys_timestamp.h"

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
    HAL_TIM_Base_Start(handle->htim);
    return SYS_TIMESTAMP_SUCCESS;
}

int sys_timestamp_get_current_timestamp(SYS_TIMESTAMP_HANDLE *handle){
    if (!handle || !handle->htim){
        return SYS_TIMESTAMP_ERROR;
    }
    handle->timestamp = (int)handle->htim->Instance->CNT; // Reference Manual 20.4.6

    return SYS_TIMESTAMP_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
