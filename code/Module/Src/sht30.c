#include "sht30.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_i2c.h"

// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------
#define SHT30_ADDRESS 0x44<<1
// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int SHT30_init(SHT30_HANDLE *handle)
{
    uint8_t cmd_buffer[2];

    if (!handle || !handle->hi2c || !handle->humidity || handle->temperature == 0) {
        return SHT30_ERROR;
    }

    //Soft Reset
    cmd_buffer = {0x30, 0xA2};
    HAL_I2C_Master_Transmit(handle->hi2c, SHT30_ADDRESS, cmd_buffer, sizeof(cmd_buffer), HAL_MAX_DELAY);
    HAL_Delay(1); // minimal waiting time before another command 

    //Soft Reset
    cmd_buffer = {0x30, 0x39};
    HAL_I2C_Master_Transmit(handle->hi2c, SHT30_ADDRESS, cmd_buffer, sizeof(cmd_buffer), HAL_MAX_DELAY);
    HAL_Delay(1); // minimal waiting time before another command 
    
    return SHT30_SUCCESS;
}

int SHT30_read(SHT30_HANDLE *s){
    return SHT30_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
