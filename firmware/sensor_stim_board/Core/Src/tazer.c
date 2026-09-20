#include "user_comm.h"
uint32_t tz_start_cnt = 0;
static uint8_t last_idx = 0xFF;

uint8_t if_tazing = 0;

#define TAZER_MATRIX_SLOT_MS 1
#define TAZER_MATRIX_TIMEOUT_MS 1000
#define TAZER_MATRIX_IDLE_MV 600
#define TAZER_MATRIX_MAX_PULSE_TICKS 8

void reset_tazer_state(void)
{
    tz_start_cnt = 0;
    if_tazing = 0;
    last_idx = 0xFF;
}

void set_tazer_state_start(void)
{
    tz_start_cnt = 0;
    if_tazing = 1;
}

void tazer_current_task(void)
{
    static uint32_t last_run_ts = 0;
    uint32_t current_ts = HAL_GetTick();
    if (current_ts - last_run_ts < 20)
        return;

    // step 1 设置强度
    if (max_tx_value > MIN_TAZER_VAL && max_tx_value <= MAX_TAZER_VAL)
    {
        // 值的范围是20~300最大
        // current_level 输入是1-100
        uint16_t currentCtlMV = 600 + max_tx_value * CURRENT_CTL_SCALE;
        set_DAC80502_output(currentCtlMV, DAC_CH_A);
    }
    else
    {
        set_DAC80502_output(600, DAC_CH_A);
    }

    last_run_ts = current_ts;
}

void tazer_25hz_task(void)
{
    static uint32_t last_run_ts = 0;

    uint32_t current_ts = HAL_GetTick();
    if (current_ts - last_run_ts < 40)
        return;

    // 打开电击通道
    if (max_tx_value > MIN_TAZER_VAL && max_tx_value <= MAX_TAZER_VAL && max_tx_idx < TOTAL_POINTS)
    {

        set_tazer_state_start();
        last_idx = max_tx_idx;
        set_tz_ch(max_tx_idx, HV2801_CH_ON);
    }

    last_run_ts = current_ts;
}

void tazer_stop_task(void)
{
    if (tim7_is_up == 0)
        return;

    if (if_tazing)
    {
        tz_start_cnt++;
        if (tz_start_cnt > sensitive)
        {
            set_tz_ch(last_idx, HV2801_CH_OFF);
            reset_tazer_state();
        }
    }
    tim7_is_up = 0;
}

static uint16_t tazer_value_to_mv(uint16_t value)
{
    if (value > MAX_TAZER_VAL)
        value = MAX_TAZER_VAL;

    return TAZER_MATRIX_IDLE_MV + (uint16_t)(value * CURRENT_CTL_SCALE);
}

static void tazer_matrix_stop(uint8_t active_idx)
{
    if (active_idx < TOTAL_POINTS)
    {
        set_tz_ch(active_idx, HV2801_CH_OFF);
    }
    set_DAC80502_output(TAZER_MATRIX_IDLE_MV, DAC_CH_A);
}

void tazer_matrix_scan_task(void)
{
    static uint32_t last_slot_ts = 0;
    static uint8_t scan_idx = 0;
    static uint8_t active_idx = 0xFF;
    static uint8_t pulse_ticks = 0;

    uint32_t current_ts = HAL_GetTick();

    if (stim_matrix_active && current_ts - stim_matrix_last_rx_ms > TAZER_MATRIX_TIMEOUT_MS)
    {
        memset(stim_matrix, 0, sizeof(uint16_t) * TOTAL_POINTS);
        stim_matrix_active = 0;
    }

    if (tim7_is_up)
    {
        if (pulse_ticks > 0)
        {
            pulse_ticks--;
            if (pulse_ticks == 0)
            {
                tazer_matrix_stop(active_idx);
                active_idx = 0xFF;
            }
        }
        tim7_is_up = 0;
    }

    if (!stim_matrix_active)
    {
        if (active_idx != 0xFF)
        {
            tazer_matrix_stop(active_idx);
            active_idx = 0xFF;
        }
        return;
    }

    if (current_ts - last_slot_ts < TAZER_MATRIX_SLOT_MS)
        return;
    last_slot_ts = current_ts;

    if (active_idx != 0xFF)
    {
        tazer_matrix_stop(active_idx);
        active_idx = 0xFF;
        pulse_ticks = 0;
    }

    uint8_t ch = scan_idx;
    scan_idx++;
    if (scan_idx >= TOTAL_POINTS)
        scan_idx = 0;

    uint16_t value = stim_matrix[ch];
    if (value <= MIN_TAZER_VAL || value > MAX_TAZER_VAL)
        return;

    set_DAC80502_output(tazer_value_to_mv(value), DAC_CH_A);
    set_tz_ch(ch, HV2801_CH_ON);
    active_idx = ch;

    pulse_ticks = sensitive;
    if (pulse_ticks == 0)
        pulse_ticks = 1;
    if (pulse_ticks > TAZER_MATRIX_MAX_PULSE_TICKS)
        pulse_ticks = TAZER_MATRIX_MAX_PULSE_TICKS;
}

void tazer_fast_task_(void)
{

    static tazer_state_t flag = TAZER_STATE_OFF;
    static uint8_t last_idx = 0xFF;
    if (tim7_is_up)
    {
        if (flag == TAZER_STATE_OFF)
        {
            if (last_idx != 0xFF)
                set_tz_ch(last_idx, HV2801_CH_OFF);

            // 关闭电击通道
            flag = TAZER_STATE_ON;
        }
        else
        {
            // 打开电击通道
            if (max_tx_value > MIN_TAZER_VAL && max_tx_value <= MAX_TAZER_VAL && max_tx_idx < TOTAL_POINTS)
            {
                set_tz_ch(max_tx_idx, HV2801_CH_ON);
                last_idx = max_tx_idx;
            }
            else
            {
                if (last_idx != 0xFF)
                    set_tz_ch(last_idx, HV2801_CH_OFF);

                last_idx = 0xFF;
            }

            flag = TAZER_STATE_OFF;
        }
        tim7_is_up = 0;
    }
}
