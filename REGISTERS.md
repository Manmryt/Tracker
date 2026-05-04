# Component Research and Register Map

This document provides exact technical specifications for the core hardware used in this project and a consolidated register map for quick reference.

## Core Components Detail

| Component | Exact Model / Version | Key Specifications | Role |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | **ESP32-WROOM-32D** | Dual-core 240MHz, 4MB Flash, 520KB SRAM | System Controller |
| **IMU** | **InvenSense ICM-20948** | 9-DOF (Accel/Gyro/Mag), I2C/SPI up to 7MHz | Motion Tracking |
| **LoRa Module** | **Semtech SX1276 / RFM95W** | 868/915MHz (Region Dep.), SPI interface | Long Range Telemetry |
| **OLED Display** | **SSD1306 (0.96")** | 128x64 resolution, I2C interface | Data Visualization |
| **SD Card** | **MicroSD Module (Generic)** | SPI interface, FAT32 support | Blackbox Logging |

---

## ICM-20948 Register Map
The ICM-20948 uses a **banked architecture**. Write the bank number to `REG_BANK_SEL` (0x7F) before accessing registers in that bank.

### User Bank 0: Data and Power
| Hex | Register Name | Default | Description |
| :--- | :--- | :--- | :--- |
| 0x00 | `WHO_AM_I` | 0xEA | Device Identification |
| 0x06 | `PWR_MGMT_1` | 0x41 | [7]Reset [6]Sleep [5]LP_En [3]Temp_Dis |
| 0x07 | `PWR_MGMT_2` | 0x00 | [5:3]Gyro_Disable [2:0]Accel_Disable |
| 0x2D | `ACCEL_XOUT_H` | - | Accel X-axis High Byte |
| 0x2E | `ACCEL_XOUT_L` | - | Accel X-axis Low Byte |
| 0x33 | `GYRO_XOUT_H` | - | Gyro X-axis High Byte |
| 0x34 | `GYRO_XOUT_L` | - | Gyro X-axis Low Byte |
| 0x7F | `REG_BANK_SEL` | 0x00 | [5:4] User Bank Select (0-3) |

### User Bank 2: Configuration
| Hex | Register Name | Default | Description |
| :--- | :--- | :--- | :--- |
| 0x01 | `GYRO_CONFIG_1` | 0x01 | [5:3]DLPF_Cfg [2:1]FSR (00=250dps, 11=2000dps) |
| 0x14 | `ACCEL_CONFIG` | 0x01 | [5:3]DLPF_Cfg [2:1]FSR (00=2g, 11=16g) |

---

## SX1276 LoRa Mode Register Map
*Note: Only applicable when LoRa mode is enabled in `RegOpMode`.*

| Hex | Register Name | Description |
| :--- | :--- | :--- |
| 0x01 | `RegOpMode` | [7]LoRa_Mode [2:0]Mode (000=Sleep, 001=Stdby, 011=TX, 101=RX) |
| 0x06 | `RegFrfMsb` | Carrier Frequency MSB |
| 0x09 | `RegPaConfig` | [7]PaSelect [6:4]MaxPower [3:0]OutputPower |
| 0x12 | `RegIrqFlags` | [6]RxDone [3]TxDone [5]PayloadCrcError |
| 0x1D | `RegModemConfig1` | [7:4]BW (7=125kHz) [3:1]CodingRate [0]ImplicitHeader |
| 0x1E | `RegModemConfig2` | [7:4]SpreadingFactor (6=64, 12=4096) [2]CrcOn |
| 0x39 | `RegSyncWord` | LoRa Sync Word (Default: 0x12) |
| 0x42 | `RegVersion` | Silicon Revision (0x12) |

---

## Magnetometer (AK09916)
The Magnetometer is an internal I2C slave to the ICM-20948. It is typically accessed via the ICM's I2C Master interface (Bank 3).

| Hex | Register Name | Description |
| :--- | :--- | :--- |
| 0x01 | `WIA2` | Device ID (0x09) |
| 0x11 | `HXL` | Mag X-axis Low Byte |
| 0x13 | `HYL` | Mag Y-axis Low Byte |
| 0x15 | `HZL` | Mag Z-axis Low Byte |
| 0x31 | `CNTL2` | [4:0]Mode (0=PowerDown, 2=Cont10Hz, 4=Cont20Hz, 8=Cont100Hz) |
