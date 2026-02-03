#ifndef __COMMAND_ISSUE_TASK_H__
#define __COMMAND_ISSUE_TASK_H__

#include "pc_link.h"

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

/*
cmd [0] = item, cmd [1] = sub item, cmd [2] = value

item, sub item
0,0 = temperature,sht30_temperature_upper_threshold
0,1 = temperature,sht30_temperature_lower_threshold
1,0 = humidity,sht30_humidity_upper_threshold;
1,1 = humidity,sht30_humidity_lower_threshold
2,0 = lcd,lcd_brightness
2,1 = lcd,lcd_display_mode
3,0 = period,sht30_measure_period
3,0 = period,pc_link_log_report_period
3,0 = period,lcd_refresh_period
*/
