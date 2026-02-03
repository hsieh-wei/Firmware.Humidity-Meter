#ifndef __BUTTON_PROCESS_TASK_H__
#define __BUTTON_PROCESS_TASK_H__

#include "button.h"
// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    BUTTON_HANDLE *target_button_k0;
    BUTTON_HANDLE *target_button_k1;
} BUTTON_PROCESS_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
#define BUTTON_K0_NOTIFY 1 << 0  // 0x01
#define BUTTON_K1_NOTIFY 1 << 1  // 0x02
void button_process_task(void *parameter);

#endif  //__BUTTON_PROCESS_TASK_H__
