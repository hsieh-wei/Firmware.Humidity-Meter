#include "FreeRTOS.h"
#include "command_issue_task.h"
#include "pc_link.h"

// --------------------------------------------------------------------------
// Task
// --------------------------------------------------------------------------
void command_issue_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);  // kill itself
    }

    // change void pointer into argument pointer
    COMMAND_ISSUE_TASK_PARAMETER *task_parameter = (COMMAND_ISSUE_TASK_PARAMETER *)parameter;

    // Get task parameter
    PC_LINK_HANDLE *pc_link_handle = task_parameter->target_pc_link;

    // init pc link

    // infinite loop
    while (1) {
    }
}
