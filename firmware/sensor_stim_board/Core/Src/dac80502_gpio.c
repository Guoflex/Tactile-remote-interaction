#include "user_comm.h"

static inline void spi_delay(void)
{
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    // 你可以加减 NOP 调速度
}

uint8_t spi_gpio_tx_byte(uint8_t tx)
{
    uint8_t rx = 0;

    for (int i = 0; i < 8; i++)
    {
        // MSB first
        if (tx & 0x80)
            SPI_MOSI_HIGH();
        else
            SPI_MOSI_LOW();

        tx <<= 1;

        spi_delay();

        // 第 1 边沿：上升沿（CPHA=1 → 改变数据）
        SPI_SCK_HIGH();
        spi_delay();

        // 第 2 边沿：下降沿（采样）
        SPI_SCK_LOW();

        spi_delay();
    }

    return rx;
}

void spi_gpio_transmit(uint8_t *tx, uint16_t len)
{
    SPI_CS_LOW();

    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t t = tx ? tx[i] : 0xFF;
        spi_gpio_tx_byte(t);
    }

    SPI_CS_HIGH();
}

/**
 * @brief  写 DAC80502 寄存器（24bit SPI）
 * @param  reg  寄存器地址（0x00~0x0F）
 * @param  value 16bit 数据
 */
void DAC80502_WriteReg(uint8_t reg, uint16_t value)
{
    uint8_t tx[3];

    // Byte0: [23:16] = 0b0000 | reg_addr
    // R/W = 0 (写)，Command 字段 = 0
    tx[0] = reg & 0x0F;

    // Byte1: 高 8 位
    tx[1] = (value >> 8) & 0xFF;

    // Byte2: 低 8 位
    tx[2] = value & 0xFF;
    spi_gpio_transmit(tx, 3);

    // HAL_GPIO_WritePin(DAC80502_SYNC_GPIO_Port, DAC80502_SYNC_Pin, GPIO_PIN_RESET); // SYNC 拉低
    // HAL_SPI_Transmit(&hspi2, tx, 3, 10);                                           // 发送 3 字节
    // HAL_GPIO_WritePin(DAC80502_SYNC_GPIO_Port, DAC80502_SYNC_Pin, GPIO_PIN_SET);   // SYNC 拉高，更新寄存器
}

void DAC80502_Init(void)
{
    HAL_Delay(2);

    // CONFIG (0x03) — 使能内部参考、使能两个通道
    DAC80502_WriteReg(0x03, 0x0000);

    // GAIN (0x04) — BUFx=1x, REF_DIV=0 → 0~2.5V 输出
    // DAC80502_WriteReg(0x04, 0x0002);
    // DAC80502_WriteReg(0x04, 0x0000);
    // DAC80502_WriteReg(0x04, 0x0103);

    // 2) GAIN (0x04)
    // BUFF-GAIN-A = 1（×2）
    // BUFF-GAIN-B = 1（×2）
    // REF-DIV = 0（不分频）
    DAC80502_WriteReg(0x04, 0x0003);

    // TRIGGER (0x05) — 正常操作
    // DAC80502_WriteReg(0x05, 0x0000);
}

// 已测 可用
void DAC_Convert_mV_to_Code(uint16_t mV, uint8_t *highByte, uint8_t *lowByte)
{
    // 限制范围（0 ~ 2500 mV）
    if (mV > 2500)
        mV = 2500;

    // 计算 16-bit DAC CODE
    uint32_t code = (uint32_t)mV * 65535 / 2500;

    // 转成高低字节
    *highByte = (code >> 8) & 0xFF;
    *lowByte = (code >> 0) & 0xFF;
}

void set_DAC80502_output_test(void)
{

    uint8_t gain[] = {0x04, 0x01, 0x03};
    spi_gpio_transmit(gain, sizeof(gain));
    uint8_t dacA_700mV[3] = {0x08, 0x47, 0xAE};
    uint8_t dacA_0[3] = {0x08, 0x80, 0x00};
    uint8_t dacA_buf[3] = {0x08, 0x00, 0x00};
    while (1)
    {

        DAC_Convert_mV_to_Code(700, &dacA_buf[1], &dacA_buf[2]);
        spi_gpio_transmit(dacA_buf, sizeof(dacA_buf));
        delay_ms(1000);

        DAC_Convert_mV_to_Code(0, &dacA_buf[1], &dacA_buf[2]);
        spi_gpio_transmit(dacA_buf, sizeof(dacA_buf));
        delay_ms(1000);
    }
}

void set_DAC80502_output(uint16_t mV, uint8_t channel)
{
    uint8_t dac_buf[3] = {0x00, 0x00, 0x00};
    if (channel == DAC_CH_A)
    {
        dac_buf[0] = 0x08;
    }
    else if (channel == DAC_CH_B)
    {
        dac_buf[0] = 0x09;
    }
    else
    {
        return;
    }

    DAC_Convert_mV_to_Code(mV, &dac_buf[1], &dac_buf[2]);
    spi_gpio_transmit(dac_buf, sizeof(dac_buf));
}

void init_dac80502(void)
{
    uint8_t rx_buf[] = {0, 0, 0};
    // GAIN 寄存器（地址 0x04）
    // REF-DIV=1, A/B GAIN=2
    uint8_t gain[] = {0x04, 0x01, 0x03};
    spi_gpio_transmit(gain, sizeof(gain));

    uint8_t dacA_buf[3] = {0x08, 0x00, 0x00};
    uint8_t dacB_buf[3] = {0x09, 0x00, 0x00};

    // A通道输出为700mV
    DAC_Convert_mV_to_Code(700, &dacA_buf[1], &dacA_buf[2]);
    spi_gpio_transmit(dacA_buf, sizeof(dacA_buf));

    // B通道输出为52mV
    DAC_Convert_mV_to_Code(52, &dacB_buf[1], &dacB_buf[2]);
    spi_gpio_transmit(dacB_buf, sizeof(dacB_buf));

    // HAL_GPIO_WritePin(DAC80502_SYNC_GPIO_Port, DAC80502_SYNC_Pin, GPIO_PIN_RESET); // SYNC 拉低
    // HAL_SPI_TransmitReceive(&hspi2, gain, rx_buf, sizeof(gain), HAL_MAX_DELAY);
    // HAL_GPIO_WritePin(DAC80502_SYNC_GPIO_Port, DAC80502_SYNC_Pin, GPIO_PIN_SET);

    // 设置A通道为1/2REV 0x08 0x40 0x00
    // uint8_t configData[] = {0x08, 0x80, 0x00};

    // uint8_t dacA_700mV[3] = {0x08, 0x47, 0xAE};
    // spi_gpio_transmit(dacA_700mV, sizeof(dacA_700mV));

    // 750mV输出
    // 650mV, 作用在掌心, 钝感, 2-5Hz频率
    // 600mV就没感觉了
    // DAC_Convert_mV_to_Code(700, &dacA_700mV[1], &dacA_700mV[2]);
    // HAL_GPIO_WritePin(DAC_SYNC_GPIO_Port, DAC_SYNC_Pin, GPIO_PIN_RESET);
    // HAL_SPI_TransmitReceive(&hspi2, dacA_700mV, rx_buf, sizeof(dacA_700mV), HAL_MAX_DELAY);
    // HAL_GPIO_WritePin(DAC_SYNC_GPIO_Port, DAC_SYNC_Pin, GPIO_PIN_SET);

    // 设置B通道为1/2REV 1250mV     0x09 0x80 0x00
    // uint8_t dacB_1250mV[3] = {0x09, 0x80, 0x00};
    // spi_gpio_transmit(dacB_1250mV, sizeof(dacB_1250mV));
    // HAL_GPIO_WritePin(DAC_SYNC_GPIO_Port, DAC_SYNC_Pin, GPIO_PIN_RESET);
    // HAL_SPI_TransmitReceive(&hspi2, dacB_1250mV, rx_buf, sizeof(dacB_1250mV), HAL_MAX_DELAY);
    // HAL_GPIO_WritePin(DAC_SYNC_GPIO_Port, DAC_SYNC_Pin, GPIO_PIN_SET);
}