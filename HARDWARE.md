#  Hardware & Wiring Guide

This guide details the components and connections required to build the RC Plane Tracker and the Ground Station.

##  Bill of Materials (BOM)

| Component | Description | Quantity |
| :--- | :--- | :--- |
| **ESP32 DevKit V1** | Dual-core MCU with WiFi/BT | 2 |
| **ICM-20948** | 9-DOF IMU (Accel/Gyro/Mag) | 1 |
| **LoRa SX1276/78** | Long-range radio module (433/868/915MHz) | 2 |
| **SSD1306 OLED** | 128x64 I2C Display (Ground Station) | 1 |
| **Micro SD Card Module** | SPI interface for data logging | 1 |
| **Level Shifter** | Optional (if using 5V SD modules) | 1 |

---

##  Tracker (Air Unit) Wiring

The Tracker uses **I2C** for the IMU and **SPI** for the LoRa module and SD card.

| ESP32 Pin | Component | Component Pin | Protocol |
| :--- | :--- | :--- | :--- |
| **3V3** | All | VCC | Power |
| **GND** | All | GND | Power |
| **GPIO 22** | ICM-20948 | SCL | I2C |
| **GPIO 21** | ICM-20948 | SDA | I2C |
| **GPIO 18** | LoRa / SD | SCK | SPI |
| **GPIO 23** | LoRa / SD | MOSI | SPI |
| **GPIO 19** | LoRa / SD | MISO | SPI |
| **GPIO 5** | LoRa | CS | SPI (Select 1) |
| **GPIO 4** | SD Card | CS | SPI (Select 2) |

---

##  Ground Station Wiring

The Ground Station uses **SPI** for LoRa and **I2C** for the OLED display.

| ESP32 Pin | Component | Component Pin | Protocol |
| :--- | :--- | :--- | :--- |
| **3V3** | All | VCC | Power |
| **GND** | All | GND | Power |
| **GPIO 22** | OLED | SCL | I2C |
| **GPIO 21** | OLED | SDA | I2C |
| **GPIO 18** | LoRa | SCK | SPI |
| **GPIO 23** | LoRa | MOSI | SPI |
| **GPIO 19** | LoRa | MISO | SPI |
| **GPIO 5** | LoRa | CS | SPI |

---

##  Critical Notes
1. **Antennas:** Never power the LoRa modules without their antennas attached, as this can destroy the RF power amplifier.
2. **Voltage:** Ensure all modules are powered by **3.3V**. Some SD card modules are 5V; use a level shifter or the 3.3V rail if supported.
3. **SPI Sharing:** The LoRa module and SD card share the same MOSI, MISO, and SCK lines but have **different CS (Chip Select)** pins.
