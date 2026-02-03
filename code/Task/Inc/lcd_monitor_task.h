#ifndef __LCD_MONITOR_TASK_H__
#define __LCD_MONITOR_TASK_H__

#include "lcd.h"
// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    LCD_HANDLE *target_lcd;
} LCD_MONITOR_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void lcd_monitor_task(void *parameter);

#endif  //__LCD_MONITOR_TASK_H__
