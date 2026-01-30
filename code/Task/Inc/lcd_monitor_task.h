#ifndef __LOG_REPORT_H__
#define __LOG_REPORT_H__

#include "lcd.h"

// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    LCD_HANDLE *target_lcd;
} LCD_MONITOR_TASK_HANDLE;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void log_report_task(void *parameter);

#endif  //__LOG_REPORT_H__
