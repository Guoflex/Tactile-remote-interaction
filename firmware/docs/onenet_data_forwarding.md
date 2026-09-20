# OneNET 数据流转

本文档说明 Device A 如何通过 OneNET 和本地 HTTP 转发服务，把实时通道数据下发给 Device B。所有 `<...>` 均为占位符。

## 数据流

```text
Device A
  -> OneNET 属性上报
  -> 规则引擎
  -> cpolar HTTPS
  -> tools/onenet_forward_server.py
  -> OneNET 设置设备属性 API
  -> Device B
```

## 物模型属性

Device A 与 Device B 使用相同属性：

| 属性 | 类型 | 含义 |
| --- | --- | --- |
| `source_id` | string | 数据来源设备 |
| `frame_id` | integer | Device A 递增帧号 |
| `max_tx_idx` | integer | 最大值通道，范围 0-46 |
| `max_tx_value` | integer | 调零后的最大值 |
| `matrix_data` | array | 47 通道调零后数据 |

## Device A 上报

Topic：

```text
$sys/<ONENET_PRODUCT_ID>/<ONENET_DEVICE_A_NAME>/thing/property/post
```

示例：

```json
{
  "id": "1",
  "version": "1.0",
  "params": {
    "source_id": { "value": "<ONENET_DEVICE_A_NAME>" },
    "frame_id": { "value": 1 },
    "max_tx_idx": { "value": 46 },
    "max_tx_value": { "value": 218 },
    "matrix_data": { "value": [0, 0, 0] }
  }
}
```

`matrix_data` 示例为缩写，实际固定包含 47 个数值。峰值达到 20 时进入 active；活动期间最短 100 ms 上报一次；数值低于或等于 15 连续 3 帧后发送一次 `{max_tx_idx: 0, max_tx_value: 0}`。

## OneNET 规则

规则 SQL：

```sql
select * from notify where data.params.source_id.value = "<ONENET_DEVICE_A_NAME>"
```

规则动作为 HTTP 推送，实例配置：

```text
URL: https://<CPOLAR_DOMAIN>/onenet/forward
token: <FORWARD_SHARED_TOKEN>
消息模式: 明文模式
```

OneNET 校验成功时，本地服务会记录 `url verification passed`。

## 启动转发服务

在仓库根目录打开 PowerShell：

```powershell
$env:FORWARD_SHARED_TOKEN="<FORWARD_SHARED_TOKEN>"
$env:TARGET_DEVICE_NAME="<ONENET_DEVICE_B_NAME>"
$env:EXPECTED_SOURCE_ID="<ONENET_DEVICE_A_NAME>"
$env:ONENET_PRODUCT_ID="<ONENET_PRODUCT_ID>"
python .\tools\onenet_forward_server.py
```

未设置 OneNET API 参数时，服务运行在 dry-run 模式，只打印准备下发的请求。

另开终端启动内网穿透：

```powershell
cpolar http 3000
```

将 cpolar 生成的最新 HTTPS 域名填入 OneNET HTTP 推送实例。免费域名重启后可能变化。

## 启用真实下发

在启动转发服务前补充：

```powershell
$env:ONENET_SET_PROPERTY_URL="https://iot-api.heclouds.com/thingmodel/set-device-property"
$env:ONENET_AUTH_HEADER_NAME="authorization"
$env:ONENET_AUTH_HEADER_VALUE="<ONENET_AUTHORIZATION>"
```

`authorization` 包含过期时间，失效后需要重新生成。不要把它写入源码、文档或 Git 历史。

Device B 接收 Topic：

```text
$sys/<ONENET_PRODUCT_ID>/<ONENET_DEVICE_B_NAME>/thing/property/set
```

收到 `source_id=<ONENET_DEVICE_A_NAME>` 后，Device B 根据 `max_tx_idx` 和 `max_tx_value` 更新对应通道；收到清零消息后关闭输出。

## 验证顺序

1. 启动 Python 服务，确认监听 `127.0.0.1:3000/onenet/forward`。
2. 启动 cpolar，并更新 OneNET HTTP 推送 URL。
3. 启用 OneNET 规则。
4. 启动 Device B，确认 MQTT 已连接并订阅属性设置 Topic。
5. 启动 Device A，并在传感阵列上产生信号。
6. 检查 Device A 上报、Python 转发响应和 Device B 通道更新。
7. 释放信号，确认 Device A 上报一次清零且 Device B 关闭输出。

## 常见问题

- `401 bad token`：HTTP 实例 token 与 `FORWARD_SHARED_TOKEN` 不一致。
- 只有 GET 没有 POST：URL 校验成功，但 OneNET 规则未命中或未启用。
- 一直是 `DRY-RUN`：没有设置 `ONENET_SET_PROPERTY_URL` 和有效 authorization。
- 突然收到旧数据：OneNET 可能重试历史通知；转发脚本会按属性时间过滤重复或过旧消息。
- cpolar 无请求：确认 URL 使用当前 HTTPS 域名并包含 `/onenet/forward`。

本地 Python 与 cpolar 方案适合实验复现，不建议直接作为长期生产部署方案。
