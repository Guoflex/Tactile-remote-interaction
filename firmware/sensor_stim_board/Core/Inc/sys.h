#ifndef _SYS_H_
#define _SYS_H_

uint16_t lowpass_filter(uint16_t last_value, uint16_t new_value, float alpha);

void delay_init(void);

void delay_us(uint32_t us);

void delay_ms(uint16_t ms);

#endif
