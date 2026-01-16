# Open Source Handheld Fluorometer 🧪💡

A low-cost, portable, **ESP32-based** fluorometer designed for field chemical analysis. This project integrates precise optical sensing with modern digital processing to measure fluorescence in liquid samples.

---

## 📖 Table of Contents
* [Project Overview](#project-overview)
* [System Architecture](#system-architecture)
* [Hardware Documentation](#hardware-documentation)
* [Software & Firmware](#software-firmware)
* [Mechanical Enclosure](#mechanical-enclosure)
* [Setup & Usage](#setup--usage)
* [Known Issues & Future Improvements](#known-issues--future-improvements)

---

## Project Overview
This device measures the concentration of a sample based on its fluorescence. It utilizes a **90-degree optical geometry**: an excitation light source (UV/Blue) strikes the sample, and a photodiode detects the emitted light perpendicular to the source to minimize interference from the excitation beam.

[Image of 90-degree fluorescence spectroscopy optical path diagram]

### Key Features
* **High Precision:** 16-bit ADC (**ADS1115**) for high-resolution signal conversion.
* **Real-time Display:** 2.4" TFT screen showing raw values and concentration.
* **Portable:** Battery-operated and handheld form factor.
* **Modular:** Designed with interchangeable LED modules for different fluorophores.

---

## System Architecture
The signal chain flows as follows:
1. **Excitation:** Constant current driver powers a 3V Blue LED or 3W UV LED.
2. **Detection:** Vishay **BPW34** photodiode captures fluorescence emission.
3. **Amplification:** Transimpedance Amplifier (**TIA**) converts photocurrent to voltage.
4. **Digitization:** ADS1115 converts voltage to digital signal (I2C).
5. **Processing:** ESP32 filters noise and calculates results.

[Image of electronic system block diagram for a fluorometer]

---

## Hardware Documentation

### Prototype Build (Current DIP Components)
*Used for the current working breadboard/perfboard version.*

| Component | Part Number | Description |
| :--- | :--- | :--- |
| **MCU** | ESP32-WROOM | Main controller DevKit. |
| **ADC** | ADS1115 | 16-Bit, 4-Channel ADC with PGA. |
| **Op-Amp** | MCP602-I/P | Dual Channel, Rail-to-Rail, DIP package. |
| **Photodiode** | Vishay BPW34 | Broad spectral range (430nm-1100nm). |
| **Display** | 2.4" TFT LCD | ILI9341 Driver, SPI Interface. |
| **LED Driver** | Generic 2W/3W | 350mA Constant Current Module. |

### Target Design (SMD Components)
*Selected for the final PCB version to minimize footprint.*
* **Op-Amp:** OPA320 (Precision, 20MHz, 0.2pA bias current).
* **Switching:** ADG704 Analog Mux (For switching feedback resistors/gain).
* **MOSFETs:** HXY N-Channel SOT-23 (For LED/Power control).
* **Feedback:** 0805 SMD Resistors/Caps (100k, 1M, 10M / 33pF, 330pF).

### Wiring Map (Prototype)

**ESP32 to ILI9341 TFT:**
```text
MOSI -> GPIO 23
CLK  -> GPIO 18
CS   -> GPIO 15
DC   -> GPIO 2
RST  -> GPIO 4
