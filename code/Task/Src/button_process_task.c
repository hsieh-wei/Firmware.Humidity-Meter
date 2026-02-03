#include "FreeRTOS.h"
#include "task.h"
#include "button_process_task.h"
#include "button.h"

// --------------------------------------------------------------------------
// Internal Helpers
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Task
// --------------------------------------------------------------------------
void button_process_task(void *parameter) {
    // avoid null pointer crash
    if (parameter == NULL) {
        vTaskDelete(NULL);  // kill itself
    }

    // change void pointer into argument pointer
    BUTTON_PROCESS_TASK_PARAMETER *task_parameter = (BUTTON_PROCESS_TASK_PARAMETER *)parameter;

    // Get task parameter
    BUTTON_PROCESS_TASK_PARAMETER *button_k0 = task_parameter->target_button_k0;
    BUTTON_PROCESS_TASK_PARAMETER *button_k1 = task_parameter->target_button_k1;

    // initial
    uint32_t received_bits;

    // infinite loop
    while (1) {
        if (xTaskNotifyWait(BUTTON_K0_NOTIFY, BUTTON_K0_NOTIFY, , portMAX_DELAY)) }
}
