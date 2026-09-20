#include "user_comm.h"

error_t i2c_write_reg(I2C_HandleTypeDef hi2c, uint16_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(&hi2c, dev_addr, reg_addr, 1, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        DBG_PRINTF("I2C Write Reg Failed: Dev 0x%02X Reg 0x%02X\r\n", dev_addr, reg_addr);
        return ERR_I2C_NACK;
    }

    status = HAL_I2C_Master_Transmit(&hi2c, dev_addr, data, size, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        DBG_PRINTF("I2C Write Reg Failed (Send Data): Dev 0x%02X Reg 0x%02X\r\n", dev_addr, reg_addr);
        return ERR_I2C_NACK;
    }

    return ERR_OK;
}

error_t i2c_read_reg(I2C_HandleTypeDef hi2c, uint16_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(&hi2c, dev_addr, &reg_addr, 1, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        DBG_PRINTF("I2C Read Reg Failed (Send Reg Addr): Dev 0x%02X Reg 0x%02X\r\n", dev_addr, reg_addr);
        return ERR_I2C_NACK;
    }

    status = HAL_I2C_Master_Receive(&hi2c, dev_addr, data, size, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        DBG_PRINTF("I2C Read Reg Failed (Read Data): Dev 0x%02X Reg 0x%02X\r\n", dev_addr, reg_addr);
        return ERR_I2C_NACK;
    }

    return ERR_OK;
}