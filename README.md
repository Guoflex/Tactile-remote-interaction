# Remote Interaction Firmware

用于远程触觉交互实验的显示设备固件、采集与刺激控制板固件，以及 OneNET 数据转发工具。

## 系统流程

```text
采集与刺激控制板（采集端）
  -> Device A（调零、热图、峰值检测）
  -> OneNET 物模型属性上报
  -> HTTP 转发服务
  -> OneNET 设置 Device B 属性
  -> Device B 点亮对应通道
  -> 采集与刺激控制板（刺激端）
```

## 目录

```text
firmware/device_a/         Device A 的 ESP-IDF 工程
firmware/device_b/         Device B 的 ESP-IDF 工程
firmware/sensor_stim_board/ STM32H723 采集与刺激控制板工程
firmware/docs/onenet_data_forwarding.md
                           OneNET 配置与测试流程
firmware/tools/onenet_forward_server.py
                           HTTP 转发服务
```

## 硬件与软件

| 项目 | 配置 |
| --- | --- |
| MCU | ESP32-S3 |
| 采集与刺激控制 MCU | STM32H723VGTx |
| 显示屏 | 360 x 360 ST77916 |
| 触摸 | CST816S |
| 传感输入 | UART2，GPIO43/44，115200 baud |
| 传感通道 | 47 |
| ESP-IDF | 5.5.3 |
| LVGL | 9.5.0 |
| esp_lvgl_port | 2.7.2 |

两个固件的精确组件版本分别记录在各自的 `dependencies.lock` 中；两端使用相同硬件基础配置，因此共享同一份 `sdkconfig.defaults` 内容。

## 配置

公开代码使用以下占位符，不包含真实凭据：

```text
<WIFI_SSID>
<WIFI_PASSWORD>
<ONENET_PRODUCT_ID>
<ONENET_PRODUCT_ACCESS_KEY>
<ONENET_DEVICE_A_NAME>
<ONENET_DEVICE_B_NAME>
```

构建前，在以下文件中填入本地测试值：

- `firmware/device_a/main/main.c`
- `firmware/device_a/components/BSP/IOT/onenet_mqtt.h`
- `firmware/device_b/main/main.c`
- `firmware/device_b/components/BSP/IOT/onenet_mqtt.h`

不要把真实 Wi-Fi 密码、AccessKey、authorization 或生成的 `sdkconfig` 提交到仓库。

## 构建

在 ESP-IDF 5.5.3 终端中执行：

```powershell
cd firmware/<device_a_or_device_b>
$env:PYTHONUTF8="1"
idf.py set-target esp32s3
idf.py build
idf.py -p <SERIAL_PORT> flash monitor
```

`build/`、`managed_components/` 和 `sdkconfig` 都是本地生成内容，不需要提交。

采集与刺激控制板使用 STM32CubeH7 1.12.1 和 Keil MDK-ARM 工程，构建方法及安全说明见 [板端文档](firmware/sensor_stim_board/README.md)。

## Device A 行为

- UART 帧由 `AA 55`、47 个小端 `uint16_t`、1 个 checksum 字节和 `CR LF` 构成，总长 99 字节。
- 当前固件按帧头解析数据，但尚未验证 checksum 和结尾字节。
- ZERO 按钮把当前 47 通道原始值记录为零点。
- 热图显示范围为 0-80，通道与手部位置映射见 `firmware/device_a/components/BSP/LVGL/ui_matrix.c`。
- 峰值达到 20 时立即上报；活动期间每 100 ms 上报一次；低于或等于 15 连续 3 帧后上报一次清零。

## Device B 行为

- 订阅自身的 OneNET 物模型属性设置主题，并只接受 `source_id` 与 Device A 名称一致的数据。
- 收到 `matrix_data` 时刷新全部 47 个点；兼容仅包含 `max_tx_idx` 和 `max_tx_value` 的单点数据。
- 连续 1 秒未收到有效信号时自动清空热图，避免保留过期状态。

## OneNET

Device A 上报 `source_id`、`frame_id`、`max_tx_idx`、`max_tx_value` 和 47 点 `matrix_data`。平台规则、HTTP 推送、转发服务和 Device B 下发流程见 [OneNET 数据流转文档](firmware/docs/onenet_data_forwarding.md)。

## 引用与许可

引用信息见 [CITATION.cff](firmware/CITATION.cff)。代码按 Apache License 2.0 发布，详见 [LICENSE](firmware/LICENSE)。第三方组件仍适用各自许可证。
