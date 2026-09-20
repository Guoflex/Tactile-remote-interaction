# Sensor and Stimulation Controller

用于远程触觉交互实验的 47 通道采集与刺激控制板固件。

## 开发环境

| 项目 | 配置 |
| --- | --- |
| MCU | STM32H723VGTx，LQFP100 |
| STM32Cube 固件包 | STM32CubeH7 1.12.1 |
| 工程 | Keil MDK-ARM 5.32 |
| 串口 | USART1/2/3，115200，8N1 |

使用 Keil 打开 `MDK-ARM/H723_ALL_IN_ONE.uvprojx`，选择 `H723_ALL_IN_ONE` 目标后构建和烧录。CubeMX 配置保存在 `H723_ALL_IN_ONE.ioc`。

## 工作模式

`Core/Inc/user_comm.h` 中的 `TX_SIDE` 和 `RX_SIDE` 控制固件角色：

- 采集端依次读取 47 个电容通道，扣除基线并发送完整矩阵。
- 刺激端解析完整矩阵，根据通道映射、阈值和灵敏度驱动输出。

完整矩阵帧由 `AA 55`、47 个小端 `uint16_t`、1 字节校验和及 `CR LF` 组成，总长 99 字节。

## 安全说明

该固件是指定实验硬件的研究原型，不是医疗设备。当前代码必须配合硬件限流、隔离、急停和受控实验流程使用；刺激参数、通道映射和故障关闭行为应在连接人体前单独验证。禁止绕过硬件保护或在无人监督条件下启用刺激输出。

CMSIS 与 STM32 HAL 文件适用其目录中的第三方许可证。
