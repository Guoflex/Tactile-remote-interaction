#include "user_comm.h"
static uint32_t adc_counter = 0;
__attribute__((section("dma_buffer"), aligned(32)))
uint16_t adc_buf[ADC_BUF_LEN];

uint8_t sensitive = 0;

void count_adc_average(uint16_t *buf, uint16_t len, uint16_t *average)
{
    uint32_t sum = 0;
    for (uint16_t i = 0; i < len; i++)
    {
        sum += buf[i];
    }

    *average = sum / len;
}

void adc_task(void)
{
    static uint32_t last_run_ts = 0;

    // 每 10ms 运行一次
    if (HAL_GetTick() - last_run_ts < 10)
    {
        return;
    }
    last_run_ts = HAL_GetTick();

    uint16_t adc_average = 0;
    count_adc_average(adc_buf, ADC_BUF_LEN, &adc_average);

    // --- 核心修正逻辑 ---
    
    // 1. 最小值判断：如果小于 16000，则强制等于 16000，解决“下溢出”和“死区”问题
    if (adc_average < 16000)
    {
        adc_average = 16000;
    }

    // 2. 映射计算：使用 int32_t 进行中间运算，防止 (adc_average * 10) 超过 uint16_t 的 65535 上限
    // 公式：(当前值 - 最小值) * 10 / (最大范围)
    sensitive = (uint8_t)(((int32_t)(adc_average - 16000) * 10) / (65535 - 16000));

    // -------------------

    // 调试打印：每 200ms 输出一次（10ms * 20）
    // static uint8_t debug_cnt = 0;
    // if (debug_cnt++ > 20)
    // {
    //     printf("ADC Raw: %d | Sens: %d\r\n", adc_average, sensitive);
    //     debug_cnt = 0;
    // }

    adc_counter = 0;
}

// adc callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // ADC conversion complete
    if (hadc->Instance == ADC1)
    {
        adc_counter++;
    }
}

