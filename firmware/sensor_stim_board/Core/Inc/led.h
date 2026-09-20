#ifndef _LED_H_
#define _LED_H_

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} led_gpio_t;

void sens_led_task(void);

void working_led_task(void);

void max_cap_led_task(void);

extern float max_cap_val_led;

void LED_Test(void);

#endif //_LED_H_
