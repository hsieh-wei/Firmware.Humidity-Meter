#ifndef SYS_TIMESTAMP
#define SYS_TIMESTAMP

#include "stm32f4xx_hal.h"

// --------------------------------------------------------------------------
// Error Codes
// --------------------------------------------------------------------------
typedef enum {
    SYS_TIMESTAMP_SUCCESS = 0,
    SYS_TIMESTAMP_ERROR = -1,
} SYS_TIMESTAMP_ERR;

// --------------------------------------------------------------------------
// Handle
// --------------------------------------------------------------------------
typedef struct {
    TIM_HandleTypeDef *htim;  // Timer HAL handle (EX: &htim6)
    uint32_t timestamp;       // timestamp per sec (prescaler 8400-1, ARR 10000 in 84MHz
                              // APB1)
} SYS_TIMESTAMP_HANDLE;

// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int sys_timestamp_init(SYS_TIMESTAMP_HANDLE *handle);

// --------------------------------------------------------------------------
// HAL Weak Callback re define
// --------------------------------------------------------------------------
void sys_timestamp_tim_period_elapsed(SYS_TIMESTAMP_HANDLE *handle, TIM_HandleTypeDef *htim);
#endif  // SYS_TIMESTAMP
