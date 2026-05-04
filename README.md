<div align="center">

# RC Plane Tracker and Flight Controller Core

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.1-blue?logo=espressif)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
[![Status](https://img.shields.io/badge/Status-In--Development-orange)](https://github.com/)

**Building a high-performance orientation tracker for RC planes, designed from the ground up using ESP-IDF.**

[Software Setup](#software-setup) • [Hardware Roles](#hardware-roles-and-deployment) • [GCS Dashboard](#ground-control-station-gcs-dashboard-setup) • [Calibration](#calibration-guide) • [Firmware Installation](#firmware-installation)

---

</div>

## Software Setup

Follow these steps to prepare your development environment. This guide assumes a Linux (Ubuntu/Debian) environment.

### 1. System Prerequisites
Ensure your system is up to date and has the necessary build tools.
- **Python:** 3.8 or newer
- **Git:** 2.x or newer

```bash
sudo apt-get update
sudo apt-get install git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
```

### 2. Install ESP-IDF (Version 5.1)
We use ESP-IDF v5.1 for this project.
```bash
# Create a directory for the toolchain
mkdir -p ~/esp
cd ~/esp

# Clone the repository
git clone --recursive -b v5.1 https://github.com/espressif/esp-idf.git

# Run the installation script
cd ~/esp/esp-idf
./install.sh esp32
```

### 3. Python Dashboard Dependencies
The Ground Control Station (GCS) requires specific Python libraries.
```bash
# Install libraries globally or in a venv
pip install vpython==7.6.4 pyserial==3.5
```

---

## About

This project is a deep-dive into embedded systems, focused on creating a robust orientation and telemetry tracker for RC aircraft. Starting with raw IMU data processing, the goal is to evolve this project into a fully-fledged flight controller.

The focus is on **learning by doing**: documenting every datasheet read, every SPI/I2C bug, and every success in AHRS (Attitude and Heading Reference System) integration.

## Hardware and Tech Stack

| Category | Component | Technology |
| :--- | :--- | :--- |
| **Microcontroller** | ESP32 | ESP-IDF v5.1 (C) |
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

## Hardware Roles and Deployment

This project requires **two separate ESP32 MCUs**. It is critical to flash the correct firmware to the corresponding hardware role.

### 1. Tracker Unit (The Airplane)
- **Role:** Mounted on the RC plane. Captures motion, processes orientation, and transmits data via LoRa.
- **Components:** ESP32, ICM-20948 IMU, LoRa Module, SD Card Module.
- **Source Code Directory:** Root directory (`/`).

### 2. Ground Station Unit (The Pilot)
- **Role:** Remains with the pilot. Receives LoRa data, displays summary on OLED, and forwards detailed telemetry to a PC via UART/USB for 3D visualization.
- **Components:** ESP32, LoRa Module, SSD1306 OLED.
- **Source Code Directory:** `ground_station/`.

---

## Ground Control Station (GCS) Dashboard Setup

The GCS dashboard is a Python application that provides a real-time 3D visualization of the plane's attitude and flight path.

### 1. UART Connection
The Ground Station ESP32 communicates with the PC dashboard via the **UART-to-USB bridge**.
- Ensure the Ground Station is connected to your PC.
- The firmware outputs data at **115200 baud**.

### 2. Dependency Verification
Run the following command to verify dependencies are correctly installed:
```bash
python3 -c "import vpython; import serial; print('Dependencies verified successfully')"
```

### 3. Running the Dashboard
1. Identify your serial port (Linux: `/dev/ttyUSB0`, Windows: `COM3`).
2. Open `gcs/dashboard.py` and update the `SERIAL_PORT` variable.
3. Run the script:
   ```bash
   python3 gcs/dashboard.py
   ```

---

## Calibration Guide
To ensure accurate flight data and path mapping, you must calibrate the IMU:
1.  **Level and Still:** Place the plane on a perfectly level surface and keep it absolutely still.
2.  **Trigger:** Hold the **BOOT button (GPIO 0)** while powering on or resetting the Tracker ESP32.
3.  **Process:** The firmware will take 500 samples to calculate offsets and save them to **NVS (Non-Volatile Storage)**.
4.  **Auto-Load:** On subsequent boots, the offsets are loaded automatically.

---

## Firmware Installation

### 1. Export ESP-IDF Environment
Before building, you must export the environment variables in every new terminal session:
```bash
. ~/esp/esp-idf/export.sh
```

### 2. Flash the Tracker (Air Unit)
Connect the **Tracker ESP32** to your PC:
```bash
# From the project root
idf.py set-target esp32
idf.py build flash
```

### 3. Flash the Ground Station
Connect the **Ground Station ESP32** to your PC:
```bash
cd ground_station
idf.py set-target esp32
idf.py build flash
```

---

## Future Roadmap
While starting as a tracker, the architecture is being built to support:
- **Flight Stabilization:** Turning sensor data into motor/servo commands.
- **On-Screen Display (OSD):** Overlaying data on FPV feeds.
- **Blackbox Logging:** Storing flight data to an SD card for post-flight analysis.

---

<div align="center">
Developed with care for the RC Community.
</div>
