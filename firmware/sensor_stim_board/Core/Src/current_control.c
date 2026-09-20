#include "user_comm.h"

// 目前电流档位从0-100
uint8_t current_level = 0;

uint16_t current_level_to_mv(uint8_t level)
{
    // 输出电压从600V到750mV线性变化
    if (level > 100)
        level = 100;

    return 600 + (uint16_t)(level * CURRENT_CTL_SCALE);
}

static void test_sen(void)
{
    set_DAC80502_output(current_level_to_mv(current_level), DAC_CH_A);
}

// 100ms 执行一次
void current_control_task(void)
{
    static uint32_t last_run_ts = 0;
    if (HAL_GetTick() - last_run_ts < 100)
        return;

    set_DAC80502_output(current_level_to_mv(current_level), DAC_CH_A);

    last_run_ts = HAL_GetTick();
}