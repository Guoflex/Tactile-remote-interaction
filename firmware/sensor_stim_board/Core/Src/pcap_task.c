#include "user_comm.h"

// 200ms 执行一次
void pcap_task(void)
{
    static uint32_t last_run_ts = 0;
    uint32_t current_ts = HAL_GetTick();
    if (current_ts - last_run_ts < 200)
        return;

    static uint8_t pcap_ch_idx = 0;

    // 读取PCAP数据
    // pcap_send_PCAP01_MEAS_CAP();
    set_cap_ch(pcap_ch_idx, HV2801_CH_ON);

    delay_ms(10);
    float result = 0;
    // result = pcap01_read_result(0);
    result = pcap01_read_result(1);
    // result = pcap01_read_result(2);
    // result = pcap01_read_result(3);
    set_cap_ch(pcap_ch_idx, HV2801_CH_OFF);

    pcap_ch_idx++;
    if (pcap_ch_idx >= TOTAL_POINTS)
        pcap_ch_idx = 0;

    // // 打开采集通道0
    // set_cap_ch(0, HV2801_CH_ON);
    // delay_ms(10);
    // float result = pcap01_read_result(1);
    // // 关闭采集通道0
    // set_cap_ch(0, HV2801_CH_OFF);

    // print
    // DBG_PRINTF("PCAP01 Result: %.5f\r\n", result);

    last_run_ts = current_ts;
    // 在这里添加PCAP任务处理代码
}
