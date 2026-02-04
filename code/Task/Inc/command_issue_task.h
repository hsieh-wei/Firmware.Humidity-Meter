#ifndef __COMMAND_ISSUE_TASK_H__
#define __COMMAND_ISSUE_TASK_H__

#include "pc_link.h"
#include "stream_buffer.h"
#include "FreeRTOS.h"

// --------------------------------------------------------------------------
// Notify Element
// --------------------------------------------------------------------------
extern StreamBufferHandle_t command_stream_buffer;

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    PC_LINK_HANDLE *target_pc_link;
} COMMAND_ISSUE_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void command_issue_task(void *parameter);

#endif  //__COMMAND_ISSUE_TASK_H__
