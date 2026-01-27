#ifndef __LOG_REPORT_H__
#define __LOG_REPORT_H__

#include "pc_link.h"

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    PC_LINK_HANDLE *target_pc_link;
} LOG_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void log_report_task(void *parameter);

#endif  //__LOG_REPORT_H__
