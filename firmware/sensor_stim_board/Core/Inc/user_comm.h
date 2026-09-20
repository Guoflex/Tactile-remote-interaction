#ifndef _USER_COMM_H_
#define _USER_COMM_H_

#include "main.h"
#include "adc_task.h"
#include "led.h"
#include "sys.h"
#include "pcap.h"
#include "i2c.h"
#include "pcap_param.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "iwdg.h"
#include "pacap_task.h"
#include "hv2801.h"
#include "tazer.h"
#include "dac80502_gpio.h"
#include "current_control.h"
#include "app_main.h"
#include "tx_task.h"
#include "usart.h"
#include "rx_task.h"

#define UART_RX_BUF_LEN 128

#define DEBUG_PRINT 1

#if DEBUG_PRINT
#define DBG_PRINTF(fmt, ...) \
    printf("[%s:%d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define DBG_PRINTF(...) ((void)0)
#endif

#define ADC_BUF_LEN 100
#define I2C_RX_MAX 32
typedef int32_t error_t;

/* 通用错误 */
#define ERR_OK 0
#define ERR_TIMEOUT -1
#define ERR_BUSY -2
#define ERR_INVALID_PARAM -3

/* I2C 错误域 */
#define ERR_I2C_BASE (-100)
#define ERR_I2C_NACK (ERR_I2C_BASE - 1)
#define ERR_I2C_BUS (ERR_I2C_BASE - 2)
#define ERR_I2C_ERROR

/* PCAP 错误域 */
#define ERR_PCAP_BASE (-200)

#define TOTAL_POINTS 47

#define CURRENT_CTL_SCALE 1.5f
#define DAC_CH_A 1
#define DAC_CH_B 2

#define CAP_LED_STEP 100.0f
#define TOTAL_LED 10

#define TX_SIDE 0
#define RX_SIDE 1

#define MSG_LEN 6
#define MATRIX_MSG_HEADER_0 0xAA
#define MATRIX_MSG_HEADER_1 0x55
#define MATRIX_MSG_LEN (2 + TOTAL_POINTS * 2 + 1 + 2)

#define CMD_LEN 3

#define RESET_BASELINE_CMD 0x01
#define ZERO_BASELINE_CMD 0x02

#define MIN_TAZER_VAL 20
#define MAX_TAZER_VAL 300

#define MAX_CAP 1000.0f

#endif
