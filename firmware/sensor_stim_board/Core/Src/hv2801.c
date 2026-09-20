#include "user_comm.h"

#define HV2801_SW(n) (1UL << (n)) // 如果你定义 Dn ↔ SWn

// just for test HV2801_1
#define HV_LE_GPIO_Port HV2801_1_CS_GPIO_Port
#define HV_LE_Pin HV2801_1_CS_Pin

static const hv2801_gpio_t hv2801_1_gpio =
    {
        .clk_port = HV2801_1_CLK_GPIO_Port,
        .clk_pin = HV2801_1_CLK_Pin,
        .data_port = HV2801_1_DATA_GPIO_Port,
        .data_pin = HV2801_1_DATA_Pin,
        .le_port = HV_LE_GPIO_Port,
        .le_pin = HV_LE_Pin,
};

static const hv2801_gpio_t hv2801_2_gpio =
    {
        .clk_port = HV2801_2_CLK_GPIO_Port,
        .clk_pin = HV2801_2_CLK_Pin,
        .data_port = HV2801_2_DATA_GPIO_Port,
        .data_pin = HV2801_2_DATA_Pin,
        .le_port = HV2801_2_CS_GPIO_Port,
        .le_pin = HV2801_2_CS_Pin,
};

static const hv2801_gpio_t hv2801_3_gpio =
    {
        .clk_port = HV2801_3_CLK_GPIO_Port,
        .clk_pin = HV2801_3_CLK_Pin,
        .data_port = HV2801_3_DATA_GPIO_Port,
        .data_pin = HV2801_3_DATA_Pin,
        .le_port = HV2801_3_CS_GPIO_Port,
        .le_pin = HV2801_3_CS_Pin,
};

static inline void hv_delay(void)
{
    delay_us(1);
}

void init_hv(void)
{
    HAL_GPIO_WritePin(HV2801_1_CS_GPIO_Port, HV2801_1_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HV2801_1_CLK_GPIO_Port, HV2801_1_CLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_1_DATA_GPIO_Port, HV2801_1_DATA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_1_CLR_GPIO_Port, HV2801_1_CLR_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(HV2801_2_CS_GPIO_Port, HV2801_2_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HV2801_2_CLK_GPIO_Port, HV2801_2_CLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_2_DATA_GPIO_Port, HV2801_2_DATA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_2_CLR_GPIO_Port, HV2801_2_CLR_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(HV2801_3_CS_GPIO_Port, HV2801_3_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(HV2801_3_CLK_GPIO_Port, HV2801_3_CLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_3_DATA_GPIO_Port, HV2801_3_DATA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_3_CLR_GPIO_Port, HV2801_3_CLR_Pin, GPIO_PIN_SET);
}

void send_cap_clock_pulse(uint8_t hv_num)
{

    switch (hv_num)
    {
    case 1:
        HAL_GPIO_WritePin(HV2801_1_CLK_GPIO_Port, HV2801_1_CLK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(HV2801_1_CLK_GPIO_Port, HV2801_1_CLK_Pin, GPIO_PIN_RESET);
        break;
    case 2:
        HAL_GPIO_WritePin(HV2801_2_CLK_GPIO_Port, HV2801_2_CLK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(HV2801_2_CLK_GPIO_Port, HV2801_2_CLK_Pin, GPIO_PIN_RESET);
        break;
    case 3:
        HAL_GPIO_WritePin(HV2801_3_CLK_GPIO_Port, HV2801_3_CLK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(HV2801_3_CLK_GPIO_Port, HV2801_3_CLK_Pin, GPIO_PIN_RESET);
        break;
    default:
        return;
    }
}

// 记住, 这里是mask, 不是ch_nmb
static void hv2801_sim_spi_core(const hv2801_gpio_t *gpio, uint32_t ch_mask)
{
    // CLK = 0
    gpio->clk_port->BSRR = (uint32_t)gpio->clk_pin << 16U;

    // LE = 1 (shift enabled)
    gpio->le_port->BSRR = gpio->le_pin;

    for (int i = 0; i < 32; i++)
    {
        // MSB first
        if (ch_mask & (1UL << (31 - i)))
            gpio->data_port->BSRR = gpio->data_pin;
        else
            gpio->data_port->BSRR = (uint32_t)gpio->data_pin << 16U;

        hv_delay();

        // CLK low → high
        gpio->clk_port->BSRR = (uint32_t)gpio->clk_pin << 16U;
        hv_delay();
        gpio->clk_port->BSRR = gpio->clk_pin;
    }

    // latch
    hv_delay();
    gpio->le_port->BSRR = (uint32_t)gpio->le_pin << 16U;
    hv_delay();
    gpio->le_port->BSRR = gpio->le_pin;
}

void start_hv2801_output(void)
{
    // 拉低所有的CLR
    HAL_GPIO_WritePin(HV2801_1_CLR_GPIO_Port, HV2801_1_CLR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_2_CLR_GPIO_Port, HV2801_2_CLR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(HV2801_3_CLR_GPIO_Port, HV2801_3_CLR_Pin, GPIO_PIN_RESET);
}

void get_cap_ch_info(uint8_t ch_nmb, uint8_t *sw_nmb, uint8_t *hv_num)
{
    uint8_t hv_ch = ch_nmb % 16; // 0~15
    *hv_num = ch_nmb / 16 + 1;   // 1, 2, 3
    *sw_nmb = hv_ch * 2;         // 采集的通道都是偶数SW, 即0,2,4,6,8,10,12,14
}

void get_tz_ch_info(uint8_t ch_nmb, uint8_t *sw_nmb, uint8_t *hv_num)
{
    uint8_t hv_ch = ch_nmb % 16; // 0~15
    *hv_num = ch_nmb / 16 + 1;   // 1, 2, 3
    *sw_nmb = hv_ch * 2 + 1;     // 温度通道都是奇数SW, 即1,3,5,7,9,11,13,15
}

void shift_hv2801_data(uint8_t hv_num, uint8_t ON_OFF, uint8_t sw_nmb)
{
    // step 3. 发送模拟SPI数据
    switch (hv_num)
    {
    case 1:
        if (ON_OFF)
            hv2801_sim_spi_core(&hv2801_1_gpio, HV2801_SW(sw_nmb));
        else
            hv2801_sim_spi_core(&hv2801_1_gpio, 0);
        break;
    case 2:
        if (ON_OFF)
            hv2801_sim_spi_core(&hv2801_2_gpio, HV2801_SW(sw_nmb));
        else
            hv2801_sim_spi_core(&hv2801_2_gpio, 0);
        break;
    case 3:
        if (ON_OFF)
            hv2801_sim_spi_core(&hv2801_3_gpio, HV2801_SW(sw_nmb));
        else
            hv2801_sim_spi_core(&hv2801_3_gpio, 0);
        break;
    default:
        return;
    }
}

// ch_nmb: 0~(TOTAL_POINTS-1)
void set_cap_ch(uint8_t ch_nmb, uint8_t ON_OFF)
{
    // step 1. 通过ch_nmb找到hv2801的编号
    // 每个2801用来采集的通道有16个
    uint8_t hv_num = 0; // 1, 2, 3
    // step 2. 采集的通道都是偶数SW, 即0,2,4,6,8,10,12,14
    uint8_t sw_nmb = 0;

    get_cap_ch_info(ch_nmb, &sw_nmb, &hv_num);

    // DBG_PRINTF("Set Cap Ch: Ch %d -> HV%d SW%d %s\r\n", ch_nmb, hv_num, sw_nmb, ON_OFF ? "ON" : "OFF");

    shift_hv2801_data(hv_num, ON_OFF, sw_nmb);
}

void set_tz_ch(uint8_t ch_nmb, uint8_t ON_OFF)
{
    uint8_t hv_num = 0; // 1, 2, 3
    uint8_t sw_nmb = 0;

    if (ch_nmb >= TOTAL_POINTS)
        return;

    get_tz_ch_info(ch_nmb, &sw_nmb, &hv_num);

    // DBG_PRINTF("Set TZ Ch: Ch %d -> HV%d SW%d %s\r\n", ch_nmb, hv_num, sw_nmb, ON_OFF ? "ON" : "OFF");
    shift_hv2801_data(hv_num, ON_OFF, sw_nmb);
}