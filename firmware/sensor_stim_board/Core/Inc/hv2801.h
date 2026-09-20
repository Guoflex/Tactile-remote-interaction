#ifndef _HV2801_H_
#define _HV2801_H_

typedef struct
{
    GPIO_TypeDef *clk_port;
    uint16_t clk_pin;

    GPIO_TypeDef *data_port;
    uint16_t data_pin;

    GPIO_TypeDef *le_port; // LE / CS
    uint16_t le_pin;
} hv2801_gpio_t;

#define HV2801_CH_ON 1
#define HV2801_CH_OFF 0

void set_cap_ch(uint8_t ch_nmb, uint8_t ON_OFF);

void init_hv(void);

void hv2801_test(void);

void set_tz_ch(uint8_t ch_nmb, uint8_t ON_OFF);

void start_hv2801_output(void);

#endif
