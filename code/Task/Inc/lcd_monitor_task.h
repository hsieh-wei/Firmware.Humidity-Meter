#ifndef __LCD_MONITOR_TASK_H__
#define __LCD_MONITOR_TASK_H__

#include "lcd.h"
#include "button.h"
// --------------------------------------------------------------------------
// Parameter Into Task
// --------------------------------------------------------------------------
typedef struct {
    LCD_HANDLE *target_lcd;
    BUTTON_HANDLE *target_button_k0;
    BUTTON_HANDLE *target_button_k1;
} LCD_MONITOR_TASK_PARAMETER;

// --------------------------------------------------------------------------
// TASK
// --------------------------------------------------------------------------
void lcd_monitor_task(void *parameter);

#endif  //__LCD_MONITOR_TASK_H__
