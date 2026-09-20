#include "user_comm.h"
#include "app_main.h"
uint8_t tx_buf[6] = {0};
volatile uint16_t u1_got_rx = 0;
uint8_t u1_rx_buf[UART_RX_BUF_LEN] = {0};

void print_max_info(void)
{
    static uint8_t print_cnt = 0;
    print_cnt++;
    if (print_cnt < 10)
        return;
    print_cnt = 0;
    DBG_PRINTF("PCAP01 Max Cap Channel: %d, Value: %d\r\n", max_tx_idx, max_tx_value);
}

// tx的端口是uart3
void tx_task(void)
{

    static uint32_t last_run_ts = 0;
    uint32_t current_ts = HAL_GetTick();
    if (current_ts - last_run_ts < 100)
        return;
    print_max_info();

    tx_buf[0] = max_tx_idx;
    tx_buf[1] = (uint8_t)(max_tx_value & 0xFF);
    tx_buf[2] = (uint8_t)((max_tx_value >> 8) & 0xFF);
    tx_buf[3] = tx_buf[0] + tx_buf[1] + tx_buf[2]; // 简单校验和
    tx_buf[4] = 0x0D;                              // \r
    tx_buf[5] = 0x0A;                              // \n

    HAL_UART_Transmit(&huart3, (uint8_t *)tx_buf, sizeof(tx_buf), HAL_MAX_DELAY);

    last_run_ts = current_ts;
}

void cmd_parse(uint8_t *buf, uint16_t len)
{
    if (len != CMD_LEN)
        return;

    if (buf[CMD_LEN - 2] != 0x0D || buf[CMD_LEN - 1] != 0x0A)
        return;

    switch (buf[0])
    {
    case RESET_BASELINE_CMD:
        DBG_PRINTF("Received Reset Baseline Command\r\n");
        if_reset_baseline = 1;
        break;

    case ZERO_BASELINE_CMD:
        DBG_PRINTF("Received Zero Baseline Command\r\n");
        zero_base_line();
        break;
    }

    // DBG_PRINTF("Received Command - Max Cap Channel: %d, Value: %d\r\n", max_tx_idx, max_tx_value);
}

void tx_cmd_task(void)
{
    if (u1_got_rx)
    {

        // cmd_parse(u1_rx_buf, u1_got_rx);
        cmd_parse(u1_rx_buf, u1_got_rx);
        // 处理完毕后，清除标志
        u1_got_rx = 0;
    }
}

void uart1_it_handler(void)
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
    {
        // 清除 IDLE 标志
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);

        // 此时说明可能一帧数据接收完了
        // 获取当前 DMA 计数器剩余量, 计算本次接收字节数
        uint16_t remain = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
        if (remain)
        {
            uint16_t data_len = UART_RX_BUF_LEN - remain;
            u1_got_rx = data_len;
        }

        // 处理完毕后，可以选择清零或记录这段数据
        // 然后继续保持 DMA 接收，让它随时准备接收新的数据
        HAL_UART_AbortReceive(&huart1);
        // 重新开启接收
        HAL_UART_Receive_DMA(&huart1, u1_rx_buf, UART_RX_BUF_LEN);
    }
}