<div align="center">

#  RC Plane Tracker & Flight Controller Core

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.x-blue?logo=espressif)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
[![Status](https://img.shields.io/badge/Status-In--Development-orange)](https://github.com/)

**Building a high-performance orientation tracker for RC planes, designed from the ground up using ESP-IDF.**

[About](#-about) • [Hardware](#-hardware--tech-stack) • [Development Journey](#-the-development-journey) • [Future Roadmap](#-future-roadmap) • [Setup](#-setup)

---

</div>

##  About

This project is a deep-dive into embedded systems, focused on creating a robust orientation and telemetry tracker for RC aircraft. Starting with raw IMU data processing, the goal is to evolve this project into a fully-fledged flight controller.

The focus is on **learning by doing**: documenting every datasheet read, every SPI/I2C bug, and every success in AHRS (Attitude and Heading Reference System) integration.

##  Hardware & Tech Stack

| Category | Component | Technology |
| :--- | :--- | :--- |
| **Microcontroller** | ESP32 | ESP-IDF (C) |
| **IMU (9-DOF)** | ICM-20948 | I2C / SPI |
| **Telemetry** | LoRa SX127x | SPI |
| **Algorithms** | AHRS | Madgwick / Mahony |
| **Tools** | Linux / VS Code | CMake / Ninja |

## 🗺️ The Development Journey

###  Phase 1: Foundation & Research
- [ ] **Environment:** Setting up the ESP-IDF toolchain on Linux.
- [ ] **Datasheet Deep-Dive:** Decoding the ICM-20948 register map and power management.
- [ ] **Communication:** Implementing reliable I2C/SPI drivers for sensor interfacing.

###  Phase 2: Firmware Core (The Tracker)
- [ ] **Data Acquisition:** High-speed polling of Accel/Gyro/Mag data.
- [ ] **Sensor Fusion:** Converting raw sensor values into stable Euler angles (Pitch, Roll, Yaw).
- [ ] **Architecture:** Designing a modular, task-based firmware using FreeRTOS.

###  Phase 3: Remote Communication
- [ ] **LoRa Integration:** Long-range telemetry using point-to-point LoRa modules.
- [ ] **Protocol Design:** Creating a lightweight binary packet format for telemetry updates.
- [ ] **Ground Station:** A secondary ESP32 to receive and display real-time data.

###  Phase 4: Evolution (Flight Controller)
- [ ] **Control Theory:** Implementing PID loops for stabilization.
- [ ] **Actuation:** Generating PWM and DShot signals for servos and ESCs.
- [ ] **Expansion:** GPS integration for RTH (Return to Home) capabilities.

##  Future Roadmap
While starting as a tracker, the architecture is being built to support:
- **Flight Stabilization:** Turning sensor data into motor/servo commands.
- **On-Screen Display (OSD):** Overlaying data on FPV feeds.
- **Blackbox Logging:** Storing flight data to an SD card for post-flight analysis.

##  Setup

To get started with the project, ensure you have the ESP-IDF environment installed.

```bash
# Clone the repository
git clone https://github.com/yourusername/Tracker.git
cd Tracker

# Set up the environment
. $HOME/esp/esp-idf/export.sh

# Build the project
idf.py build
```

---

<div align="center">
Developed with ❤️ for the RC Community.
</div>
