#ifndef _I2C_APP_H_
#define _I2C_APP_H_

error_t i2c_write_reg(I2C_HandleTypeDef hi2c, uint16_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size);

error_t i2c_read_reg(I2C_HandleTypeDef hi2c, uint16_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size);

#endif