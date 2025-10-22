#include "sht30.h"
#include "pc_link.h"
#include <stdint.h>
#include <stdio.h>

// --------------------------------------------------------------------------
// Internal Helpers 
// --------------------------------------------------------------------------
#define SHT30_ADDRESS (0x44 << 1) // ADDR connected to logic low

static uint8_t sht30_crc8(const uint8_t *data) {
    uint8_t crc = 0xFF; // Init
    for (uint32_t i = 0; i < 2; ++i) {
        // crc two byte
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            // MSB first => And 0x80(get the highest bit is 1 or 0)
            // if highest bit is 1 ,left shift, XOR Polynomial(0x31)
            // if highest bit is 0 ,left shift
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc; // XorOut=0x00
}

static void sht30_dump_error(SHT30_HANDLE *handle, const char* tag){
    static uint8_t pc_link_buf_tx[PC_LINK_TX_BUF_SIZE];
    int snprintf_status = 0;
    uint32_t error_code = HAL_I2C_GetError(handle->hi2c);
    uint32_t sht30_status = handle->hi2c->State;

    snprintf_status = snprintf((char*)pc_link_buf_tx, sizeof(pc_link_buf_tx),
                        "[%s]: I2C State=0x%lx Error=0x%lx\r\n", tag, (long)sht30_status, (long)error_code);

    if (snprintf_status > 0 && snprintf_status < PC_LINK_TX_BUF_SIZE) {
      (void)pc_link_tx_dma(&g_pc_link_handle, pc_link_buf_tx, snprintf_status);
    }
}
// --------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int sht30_init(SHT30_HANDLE *handle)
{
    if (!handle || !handle->hi2c){
        return SHT30_ERROR;
    }

    HAL_Delay(1); // minimal waiting time after power up

    // initial variable
    uint8_t cmd_buf[2];
    handle->i2c_address = SHT30_ADDRESS;

    //Soft Reset
    cmd_buf[0] = 0x30; cmd_buf[1] = 0xA2;
    if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, cmd_buf, 2, 100) != HAL_OK) return SHT30_ERROR;
    HAL_Delay(2); // minimal waiting time after soft reset

    //Stop Periodic
    cmd_buf[0] = 0x30; cmd_buf[1] = 0x93;
    if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, cmd_buf, 2, 100) != HAL_OK) return SHT30_ERROR;
    HAL_Delay(1); // minimal waiting time before another command 

    //Disable Heater
    cmd_buf[0] = 0x30; cmd_buf[1] = 0x66;
    if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, cmd_buf, 2, 100) != HAL_OK) return SHT30_ERROR;
    HAL_Delay(1); // minimal waiting time before another command 

    //Clear Status Register
    cmd_buf[0] = 0x30; cmd_buf[1] = 0x41;
    if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, cmd_buf, 2, 100) != HAL_OK) return SHT30_ERROR;
    HAL_Delay(1); // minimal waiting time before another command 
    
    return SHT30_SUCCESS;
}

int sht30_read(SHT30_HANDLE *handle){
    if (!handle || !handle->hi2c){
        return SHT30_ERROR;
    }

    // initial variable
    uint8_t cmd_buf[2];
    uint8_t read_buf[6];
    handle->i2c_address = SHT30_ADDRESS;

    // Repeatability High, Disabled Clock Stretching Measurement
    cmd_buf[0] = 0x24; cmd_buf[1] = 0x00;
    if (HAL_I2C_Master_Transmit(handle->hi2c, handle->i2c_address, cmd_buf, 2, 100) != HAL_OK) {
        // i2c_dump_error(handle, "TX 0x2400");
        return SHT30_ERROR;
    }
    HAL_Delay(15); // minimal waiting time after high repeatability measurement

    // Read Measurement Data
    if (HAL_I2C_Master_Receive(handle->hi2c, handle->i2c_address, read_buf,6, 100) != HAL_OK) return SHT30_ERROR;

    // CRC Validate
    if (sht30_crc8(&read_buf[0]) != read_buf[2]) return SHT30_ERROR;
    if (sht30_crc8(&read_buf[3]) != read_buf[5]) return SHT30_ERROR;

    // Calculate Temperature
    uint16_t raw_temperature = (uint16_t)((read_buf[0] << 8) | read_buf[1]);
    handle->temperature = -45.0f + 175.0f * ((float)raw_temperature / (65535.0f)); // 65535 = 2^16-1

    // Calculate Humidity
    uint16_t raw_humidity = (uint16_t)((read_buf[3] << 8) | read_buf[4]);
    handle->humidity = 100 * (float)raw_humidity / 65535.0f; // 65535 = 2^16-1
    
    return SHT30_SUCCESS;
}

// --------------------------------------------------------------------------
// HAL Weak Callback re define 
// --------------------------------------------------------------------------
