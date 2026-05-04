<div align="center">

# RC Plane Tracker and Flight Controller Core

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.x-blue?logo=espressif)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
[![Status](https://img.shields.io/badge/Status-In--Development-orange)](https://github.com/)

**Building a high-performance orientation tracker for RC planes, designed from the ground up using ESP-IDF.**

[About](#about) • [Hardware](#hardware-and-tech-stack) • [Development Journey](#the-development-journey) • [Future Roadmap](#future-roadmap) • [Setup](#setup)

---

</div>

## About

This project is a deep-dive into embedded systems, focused on creating a robust orientation and telemetry tracker for RC aircraft. Starting with raw IMU data processing, the goal is to evolve this project into a fully-fledged flight controller.

The focus is on **learning by doing**: documenting every datasheet read, every SPI/I2C bug, and every success in AHRS (Attitude and Heading Reference System) integration.

## Hardware and Tech Stack

| Category | Component | Technology |
| :--- | :--- | :--- |
| **Microcontroller** | ESP32 | ESP-IDF (C) |
| **IMU (9-DOF)** | ICM-20948 | I2C / SPI |
| **Telemetry** | LoRa SX127x | SPI |
| **Algorithms** | AHRS | Madgwick / Mahony |
| **Tools** | Linux / VS Code | CMake / Ninja |

## The Development Journey

### Phase 1: Foundation and Research
- [x] **Environment:** Setting up the ESP-IDF toolchain on Linux.
- [x] **Datasheet Deep-Dive:** Decoding the ICM-20948 register map and power management.
- [x] **Communication:** Implementing reliable I2C/SPI drivers for sensor interfacing.

### Phase 2: Firmware Core (The Tracker)
- [x] **Data Acquisition:** High-speed polling of Accel/Gyro/Mag data.
- [x] **Sensor Fusion:** Converting raw sensor values into stable Euler angles (Pitch, Roll, Yaw).
- [x] **Calibration:** NVS-persistent calibration for offsets.
- [x] **Dead Reckoning:** Basic relative path mapping (X, Y, Z).

### Phase 3: Remote Communication
- [x] **LoRa Integration:** Long-range telemetry using point-to-point LoRa modules.
- [x] **Protocol Design:** Creating a lightweight binary packet format for telemetry updates.
- [x] **Ground Station:** A secondary ESP32 to receive and display real-time data.
- [x] **3D Visualization:** Real-time GCS dashboard for intuitive flight monitoring.

### Phase 4: Evolution (Flight Controller)
- [ ] **Control Theory:** Implementing PID loops for stabilization.
- [ ] **Actuation:** Generating PWM and DShot signals for servos and ESCs.
- [ ] **Expansion:** GPS integration for RTH (Return to Home) capabilities.

## Ground Control Station (GCS) Dashboard
The GCS dashboard provides a real-time 3D visualization of the plane's orientation and flight path.

### Prerequisites
- Python 3.x
- `vpython` and `pyserial` libraries:
  ```bash
  pip install vpython pyserial
  ```

### Running the Dashboard
1. Connect the **Ground Station ESP32** to your PC via USB.
2. Identify the serial port (e.g., `/dev/ttyUSB0` or `COM3`).
3. Update `gcs/dashboard.py` with your serial port.
4. Run the script:
   ```bash
   python gcs/dashboard.py
   ```
   A browser window will open showing the 3D plane and the "Ribbon" flight path.

## Calibration Guide
To ensure accurate flight data and path mapping, you must calibrate the IMU:
1.  **Level and Still:** Place the plane on a perfectly level surface and keep it absolutely still.
2.  **Trigger:** Hold the **BOOT button (GPIO 0)** while powering on or resetting the ESP32.
3.  **Process:** The firmware will take 500 samples to calculate offsets and save them to **NVS (Non-Volatile Storage)**.
4.  **Auto-Load:** On subsequent boots, the offsets are loaded automatically.

## Setup

### 1. Install ESP-IDF
Follow the [official Espressif guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) to install the ESP-IDF toolchain for your OS.

### 2. Export Environment Variables
In your terminal, navigate to your ESP-IDF directory and run the export script:
```bash
. $HOME/esp/esp-idf/export.sh
```

### 3. Build and Flash the Tracker (Air Unit)
From the project root:
```bash
# Navigate to tracker project (root)
cd Tracker
# Set target to esp32
idf.py set-target esp32
# Build, Flash, and Monitor
idf.py build flash monitor
```

### 4. Build and Flash the Ground Station
```bash
# Navigate to ground station directory
cd ground_station
# Set target to esp32
idf.py set-target esp32
# Build, Flash, and Monitor
idf.py build flash monitor
```

## Future Roadmap
While starting as a tracker, the architecture is being built to support:
- **Flight Stabilization:** Turning sensor data into motor/servo commands.
- **On-Screen Display (OSD):** Overlaying data on FPV feeds.
- **Blackbox Logging:** Storing flight data to an SD card for post-flight analysis.

---

<div align="center">
Developed with care for the RC Community.
</div>
