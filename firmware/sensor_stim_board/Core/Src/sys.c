#include "user_comm.h"

// 低通函数
uint16_t lowpass_filter(uint16_t last_value, uint16_t new_value, float alpha)
{
    return (uint16_t)(alpha * new_value + (1 - alpha) * last_value);
}


static uint32_t fac_us = 0;
void delay_init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); // SysTick频率为HCLK
    fac_us = 550;
}

void delay_ms(uint16_t ms)
{
    uint16_t i;
    for (i = 0; i < ms; i++)
    {
        delay_us(1000);
        // 喂狗
        HAL_IWDG_Refresh(&hiwdg1);
    }
}

void delay_us(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD; // LOAD的值
    ticks = us * fac_us;             // 需要的节拍数
    told = SysTick->VAL;             // 刚进入时的计数器值
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow; // 这里注意一下SYSTICK是一个递减的计数器就可以了.
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break; // 时间超过/等于要延迟的时间,则退出.
        }
    };
}
