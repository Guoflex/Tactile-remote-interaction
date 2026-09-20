# Sensor and Stimulation Controller

Firmware for a 47-channel sensor and stimulation controller used in remote tactile interaction experiments.

## Development Environment

| Item | Configuration |
| --- | --- |
| MCU | STM32H723VGTx, LQFP100 |
| STM32Cube firmware package | STM32CubeH7 1.12.1 |
| Project | Keil MDK-ARM 5.32 |
| Serial interfaces | USART1/2/3, 115200 baud, 8N1 |

Open `MDK-ARM/H723_ALL_IN_ONE.uvprojx` in Keil, select the `H723_ALL_IN_ONE` target, then build and flash the firmware. The CubeMX configuration is stored in `H723_ALL_IN_ONE.ioc`.

## Operating Modes

`TX_SIDE` and `RX_SIDE` in `Core/Inc/user_comm.h` select the firmware role:

- The acquisition side reads all 47 capacitive channels sequentially, subtracts the baseline, and transmits the full matrix.
- The stimulation side parses the full matrix and drives the outputs according to the channel mapping, thresholds, and sensitivity settings.

Each full-matrix frame consists of `AA 55`, 47 little-endian `uint16_t` values, one checksum byte, and `CR LF`, for a total of 99 bytes.

## Safety Information

This firmware is a research prototype for the specified experimental hardware, not a medical device. The current code must be used with hardware current limiting, isolation, an emergency stop, and controlled experimental procedures. Stimulation parameters, channel mappings, and shutdown behavior under fault conditions must be verified separately before connecting the hardware to a person. Do not bypass hardware protections or enable stimulation outputs without supervision.

CMSIS and STM32 HAL files are subject to the third-party licenses included in their directories.
