#include "user_comm.h"

__attribute__((section("dma_buffer"), aligned(32)))
uint8_t uart3_rx_buf[UART_RX_BUF_LEN];

uint8_t got_rx = 0;
uint16_t stim_matrix[TOTAL_POINTS] = {0};
volatile uint8_t stim_matrix_active = 0;
volatile uint32_t stim_matrix_last_rx_ms = 0;

static uint8_t uart2_rx_byte = 0;
static uint8_t uart2_parse_buf[MATRIX_MSG_LEN];
static uint16_t uart2_parse_len = 0;

void start_uart3_rx(void)
{
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);

    HAL_UART_Receive_DMA(&huart3, uart3_rx_buf, UART_RX_BUF_LEN);
}

void start_uart2_rx(void)
{
    HAL_UART_Receive_IT(&huart2, &uart2_rx_byte, 1);
}

void uart3_it_handler(void)
{
    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != RESET)
    {
        // 清除 IDLE 标志
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);

        // 此时说明可能一帧数据接收完了
        // 获取当前 DMA 计数器剩余量, 计算本次接收字节数
        uint16_t remain = __HAL_DMA_GET_COUNTER(huart3.hdmarx);
        if (remain)
        {
            uint16_t data_len = UART_RX_BUF_LEN - remain;
            got_rx = data_len;
        }

        // 处理完毕后，可以选择清零或记录这段数据
        // 然后继续保持 DMA 接收，让它随时准备接收新的数据
        HAL_UART_AbortReceive(&huart3);
        // 重新开启接收
        HAL_UART_Receive_DMA(&huart3, uart3_rx_buf, UART_RX_BUF_LEN);
    }
}

void data_parse(uint8_t *buf, uint16_t len)
{
    if (len != MSG_LEN)
        return;

    if (buf[MSG_LEN - 2] != 0x0D || buf[MSG_LEN - 1] != 0x0A)
        return;

    // 简单校验和
    uint8_t checksum = buf[0] + buf[1] + buf[2];
    if (checksum != buf[3])
        return;

    max_tx_value = (buf[2] << 8) | buf[1];
    max_tx_idx = buf[0];

    // sensitive = max_tx_value / 100;
    max_cap_val_led = (float)max_tx_value;
    // DBG_PRINTF("Received Command - Max Cap Channel: %d, Value: %d\r\n", max_tx_idx, max_tx_value);
}

static uint8_t matrix_data_parse(uint8_t *buf, uint16_t len)
{
    if (len != MATRIX_MSG_LEN)
        return 0;

    if (buf[0] != MATRIX_MSG_HEADER_0 || buf[1] != MATRIX_MSG_HEADER_1)
        return 0;

    if (buf[MATRIX_MSG_LEN - 2] != 0x0D || buf[MATRIX_MSG_LEN - 1] != 0x0A)
        return 0;

    uint8_t sum = 0;
    for (uint16_t i = 0; i < TOTAL_POINTS * 2; i++)
    {
        sum += buf[2 + i];
    }

    if (sum != buf[2 + TOTAL_POINTS * 2])
        return 0;

    uint16_t max_value = 0;
    uint8_t max_index = 0;
    for (uint8_t i = 0; i < TOTAL_POINTS; i++)
    {
        uint16_t value = (uint16_t)buf[2 + i * 2] | ((uint16_t)buf[2 + i * 2 + 1] << 8);
        stim_matrix[i] = value;
        if (value > max_value)
        {
            max_value = value;
            max_index = i;
        }
    }

    max_tx_idx = max_index;
    max_tx_value = max_value;
    max_cap_val_led = (float)max_value;
    stim_matrix_active = (max_value > 0);
    stim_matrix_last_rx_ms = HAL_GetTick();
    return 1;
}

static void uart2_matrix_feed(uint8_t byte)
{
    if (uart2_parse_len == 0 && byte != MATRIX_MSG_HEADER_0)
        return;

    if (uart2_parse_len == 1 && byte != MATRIX_MSG_HEADER_1)
    {
        uart2_parse_len = (byte == MATRIX_MSG_HEADER_0) ? 1 : 0;
        return;
    }

    uart2_parse_buf[uart2_parse_len++] = byte;
    if (uart2_parse_len < MATRIX_MSG_LEN)
        return;

    matrix_data_parse(uart2_parse_buf, MATRIX_MSG_LEN);
    uart2_parse_len = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        uart2_matrix_feed(uart2_rx_byte);
        HAL_UART_Receive_IT(&huart2, &uart2_rx_byte, 1);
    }
}

void rx_task(void)
{
    // sensitive
    if (got_rx)
    {

        // DBG_PRINTF("RX Got Data Len: %d\r\n", got_rx);
        if (!matrix_data_parse(uart3_rx_buf, got_rx))
        {
            data_parse(uart3_rx_buf, got_rx);
            cmd_parse(uart3_rx_buf, got_rx);
        }

        // 处理完毕后，清除标志
        got_rx = 0;
    }
}
