# Remote Interaction Firmware

Display device firmware, sensor and stimulation controller firmware, and OneNET data forwarding tools for remote tactile interaction experiments.

## System Workflow

```text
Sensor and stimulation controller (acquisition side)
  -> Device A (zero calibration, heatmap, peak detection)
  -> OneNET thing model property reporting
  -> HTTP forwarding service
  -> OneNET sets Device B properties
  -> Device B activates the corresponding channel
  -> Sensor and stimulation controller (stimulation side)
```

## Directory Structure

```text
firmware/device_a/         ESP-IDF project for Device A
firmware/device_b/         ESP-IDF project for Device B
firmware/sensor_stim_board/ STM32H723 sensor and stimulation controller project
firmware/docs/onenet_data_forwarding.md
                           OneNET configuration and testing procedure
firmware/tools/onenet_forward_server.py
                           HTTP forwarding service
```

## Hardware and Software

| Item | Configuration |
| --- | --- |
| MCU | ESP32-S3 |
| Sensor and stimulation controller MCU | STM32H723VGTx |
| Display | 360 x 360 ST77916 |
| Touch controller | CST816S |
| Sensor input | UART2, GPIO43/44, 115200 baud |
| Sensor channels | 47 |
| ESP-IDF | 5.5.3 |
| LVGL | 9.5.0 |
| esp_lvgl_port | 2.7.2 |

Exact component versions are recorded in each device's `dependencies.lock`. Both devices use the same base hardware configuration and therefore have identical `sdkconfig.defaults` contents.

## Configuration

The public source code uses the following placeholders instead of real credentials:

```text
<WIFI_SSID>
<WIFI_PASSWORD>
<ONENET_PRODUCT_ID>
<ONENET_PRODUCT_ACCESS_KEY>
<ONENET_DEVICE_A_NAME>
<ONENET_DEVICE_B_NAME>
```

Before building, enter your local test values in these files:

- `firmware/device_a/main/main.c`
- `firmware/device_a/components/BSP/IOT/onenet_mqtt.h`
- `firmware/device_b/main/main.c`
- `firmware/device_b/components/BSP/IOT/onenet_mqtt.h`

Do not commit real Wi-Fi passwords, AccessKey values, authorization credentials, or generated `sdkconfig` files to the repository.

## Building

Run the following commands from the repository root in an ESP-IDF 5.5.3 terminal, replacing the placeholders with the appropriate device directory and serial port:

```powershell
cd firmware/<device_a_or_device_b>
$env:PYTHONUTF8="1"
idf.py set-target esp32s3
idf.py build
idf.py -p <SERIAL_PORT> flash monitor
```

`build/`, `managed_components/`, and `sdkconfig` are generated locally and do not need to be committed.

The sensor and stimulation controller uses STM32CubeH7 1.12.1 and a Keil MDK-ARM project. See the [board documentation](firmware/sensor_stim_board/README.md) for build instructions and safety information.

## Device A Behavior

- Each UART frame consists of `AA 55`, 47 little-endian `uint16_t` values, one checksum byte, and `CR LF`, for a total of 99 bytes.
- The current firmware parses data using the frame header but does not yet validate the checksum or trailing bytes.
- The ZERO button records the current raw values of all 47 channels as the zero baseline.
- The heatmap display range is 0-80. See `firmware/device_a/components/BSP/LVGL/ui_matrix.c` for the mapping between channels and hand locations.
- A report is sent immediately when the peak reaches 20, then every 100 ms while active. A single reset report is sent after the value remains at or below 15 for three consecutive frames.

## Device B Behavior

- Subscribes to its OneNET thing model property-setting topic and accepts only data whose `source_id` matches the Device A name.
- Updates all 47 points when `matrix_data` is received. Single-point data containing only `max_tx_idx` and `max_tx_value` is also supported.
- Automatically clears the heatmap after one second without a valid signal to avoid retaining a stale state.

## OneNET

Device A reports `source_id`, `frame_id`, `max_tx_idx`, `max_tx_value`, and the 47-point `matrix_data` array. See the [OneNET data forwarding documentation](firmware/docs/onenet_data_forwarding.md) for platform rules, HTTP push configuration, the forwarding service, and property delivery to Device B.

## Citation and License

See [CITATION.cff](firmware/CITATION.cff) for citation information. The code is released under the Apache License 2.0; see [LICENSE](firmware/LICENSE). Third-party components remain subject to their respective licenses.
