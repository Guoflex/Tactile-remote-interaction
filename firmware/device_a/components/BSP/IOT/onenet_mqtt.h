#ifndef __ONENET_MQTT_H__
#define __ONENET_MQTT_H__
#include "esp_err.h"
#include <stdint.h>

//产品ID、设备名称、设备密钥
#define ONENET_PRODUCT_ID "<ONENET_PRODUCT_ID>"
#define ONENET_PRODUCT_ACCESS_KEY "<ONENET_PRODUCT_ACCESS_KEY>"
#define ONENET_DEVICE_NAME "<ONENET_DEVICE_A_NAME>"

// 传感器最大值结构体
typedef struct {
    uint8_t  channel;    // 0-46
    uint16_t intensity;  // 校准后的值
} sensor_peak_t;

// 启动 OneNET MQTT 连接
esp_err_t onenet_start(void);

// 找 47 通道中的最大值
sensor_peak_t find_max_channel(const uint16_t *data);

// 找最大值 + 阈值过滤 + 100ms 限流 + MQTT 发布；同时上报 47 点阵列，信号结束时清零一次
void sensor_publish_max_channel(const uint16_t *data);

#endif
