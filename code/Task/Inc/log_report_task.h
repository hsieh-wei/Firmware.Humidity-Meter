#ifndef __LOG_REPORT_TASK_H__
#define __LOG_REPORT_TASK_H__

#include "pc_link.h"
#include "sys_timestamp.h"

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    PC_LINK_HANDLE *target_pc_link;
    SYS_TIMESTAMP_HANDLE *target_sys_timestamp;
} LOG_REPORT_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void log_report_task(void *parameter);

#endif  //__LOG_REPORT_TASK_H__
