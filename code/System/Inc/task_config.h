#ifndef __TASK_CONFIG_H__
#define __TASK_CONFIG_H__

#include "FreeRTOS.h"
#include "task.h"
#include "sensor_measure_task.h"
#include "log_report_task.h"
#include "lcd_monitor_task.h"
#include "button_process_task.h"
#include "command_issue_task.h"

// --------------------------------------------------------
// task handles define(using in suspend and resume)
// --------------------------------------------------------
extern TaskHandle_t g_sensor_task_handle;
extern TaskHandle_t g_log_report_task_handle;
extern TaskHandle_t g_lcd_monitor_task_handle;
extern TaskHandle_t g_button_process_task_handle;
extern TaskHandle_t g_command_issue_task_handle;

// --------------------------------------------------------
// task parameters
// --------------------------------------------------------
extern SENSOR_MEASURE_TASK_PARAMETER g_sensor_task_param;
extern LOG_REPORT_TASK_PARAMETER g_log_report_task_param;
extern LCD_MONITOR_TASK_PARAMETER g_lcd_monitor_task_param;
extern BUTTON_PROCESS_TASK_PARAMETER g_button_process_task_param;
extern COMMAND_ISSUE_TASK_PARAMETER g_command_issue_task_param;

// --------------------------------------------------------
// API
// --------------------------------------------------------
void tasks_create(void);  // create all task

#endif