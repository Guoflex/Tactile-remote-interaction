#include "user_comm.h"

static uint8_t ch_idx = 0;

uint16_t results_buffer[TOTAL_POINTS] = {0};
uint16_t baseline_buffer[TOTAL_POINTS] = {0};

uint8_t if_reset_baseline = 0;

uint16_t max_tx_value = 0;
uint8_t max_tx_idx = 0;

void pcap_run(void)
{
    set_cap_ch(ch_idx, HV2801_CH_ON);
    delay_ms(4);
    uint16_t result = 0;
    result = (uint16_t)pcap01_read_result(1);
    set_cap_ch(ch_idx, HV2801_CH_OFF);
    results_buffer[ch_idx] = result;
}

void reset_baseline(void)
{
    if (if_reset_baseline == 0)
        return;

    for (uint8_t i = 0; i < TOTAL_POINTS; i++)
    {
        baseline_buffer[i] = results_buffer[i];
    }

    if_reset_baseline = 0;
}

void cut_base_line(void)
{
    for (uint8_t i = 0; i < TOTAL_POINTS; i++)
    {
        if (results_buffer[i] < baseline_buffer[i])
        {
            results_buffer[i] = 0;
        }
        else
        {
            results_buffer[i] = results_buffer[i] - baseline_buffer[i];
        }
    }
}

void zero_base_line(void)
{
    uint8_t i;
    for (i = 0; i < TOTAL_POINTS; i++)
    {
        baseline_buffer[i] = 0;
    }
}

void find_max_cap_point(void)
{

    float max_value = -10000.0f;
    uint8_t max_index = 0xFF;

    for (uint8_t i = 0; i < TOTAL_POINTS; i++)
    {
        if (results_buffer[i] > max_value)
        {
            max_value = results_buffer[i];
            max_index = i;
        }
    }

    max_cap_val_led = max_value;
    max_tx_idx = max_index;
    max_tx_value = (uint16_t)max_value;

    // if (max_tx_value < 100 || max_tx_value > 1000)
    // {
    //     max_tx_idx = 0xff;
    // }

    memset(results_buffer, 0, sizeof(results_buffer));
}

void print_results_buffer(void)
{
    printf("RB: ");
    for (uint8_t i = 0; i < TOTAL_POINTS; i++)
    {
        printf("%d ", results_buffer[i]);
    }
    printf("\r\n");
}

uint8_t tx_buf_big[2 + TOTAL_POINTS*2 + 1 + 2];

void send_full_array(void)
{
    uint8_t sum = 0;

    tx_buf_big[0] = 0xAA;
    tx_buf_big[1] = 0x55;

    for (int i = 0; i < TOTAL_POINTS; i++)
    {
        tx_buf_big[2 + i*2]     = results_buffer[i] & 0xFF;
        tx_buf_big[2 + i*2 +1]  = (results_buffer[i] >> 8) & 0xFF;

        sum += tx_buf_big[2 + i*2];
        sum += tx_buf_big[2 + i*2 +1];
    }

    tx_buf_big[2 + TOTAL_POINTS*2] = sum;
    tx_buf_big[3 + TOTAL_POINTS*2] = 0x0D;
    tx_buf_big[4 + TOTAL_POINTS*2] = 0x0A;

    HAL_UART_Transmit(&huart2, tx_buf_big, sizeof(tx_buf_big), 20);
}

void ch_index_increment(void)
{
    ch_idx++;
    if (ch_idx < TOTAL_POINTS)
        return;

    ch_idx = 0;

    reset_baseline();

    cut_base_line();

    //print_results_buffer();

    send_full_array();

    find_max_cap_point();
}

// 20ms 执行一次
void main_cap_task(void)
{
    static uint32_t last_run_ts = 0;
    uint32_t current_ts = HAL_GetTick();
    if (current_ts - last_run_ts < 4)
        return;

    pcap_run();

    ch_index_increment();

    last_run_ts = current_ts;
}

