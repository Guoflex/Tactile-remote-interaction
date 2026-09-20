#ifndef __ONENET_MQTT_H__
#define __ONENET_MQTT_H__
#include "esp_err.h"
#include <stdint.h>

// 产品 ID、设备名称和设备密钥
#define ONENET_PRODUCT_ID "<ONENET_PRODUCT_ID>"
#define ONENET_PRODUCT_ACCESS_KEY "<ONENET_PRODUCT_ACCESS_KEY>"
#define ONENET_DEVICE_NAME "<ONENET_DEVICE_B_NAME>"
#define ONENET_EXPECTED_SOURCE_ID "<ONENET_DEVICE_A_NAME>"

// 矩阵单点更新 (由 main.c 实现, MQTT 收到数据后调用)
void matrix_update_from_mqtt(uint8_t ch, uint16_t val);
void matrix_update_all_from_mqtt(const uint16_t *data);
void matrix_clear_from_mqtt(void);

// 传感器最大值结构体
typedef struct {
    uint8_t  channel;    // 0-46
    uint16_t intensity;  // 校准后的值
} sensor_peak_t;

// 启动 OneNET MQTT 连接
esp_err_t onenet_start(void);

// 找 47 通道中的最大值
sensor_peak_t find_max_channel(const uint16_t *data);

// 找最大值 + 全零过滤 + 100ms 限流 + MQTT 发布 (在 uart_task 中调用)
void sensor_publish_max_channel(const uint16_t *data);

#endif
