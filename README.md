Open Source Handheld Fluorometer 🧪💡

A low-cost, portable, ESP32-based fluorometer designed for field chemical analysis. This project integrates precise optical sensing with modern digital processing to measure fluorescence in liquid samples.

📖 Table of Contents

Project Overview

System Architecture

Hardware Documentation

Prototype Build (Current)

Target Design (SMD)

Software & Firmware

Mechanical Enclosure

Setup & Usage

Known Issues & Future Improvements

Project Overview

This device measures the concentration of a sample based on its fluorescence. It uses a 90-degree optical geometry where an excitation light source (UV/Blue) strikes the sample, and a photodiode detects the emitted light perpendicular to the source.

Key Features:

High Precision: 16-bit ADC (ADS1115) for high-resolution signal conversion.

Real-time Display: 2.4" TFT screen showing raw values and concentration.

Portable: Battery-operated and handheld form factor.

Modular: Designed with interchangeable LED modules for different fluorophores.

System Architecture

The signal chain flows as follows:

Excitation: Constant current driver powers a 3V Blue LED or 3W UV LED.

Detection: Vishay BPW34 photodiode captures fluorescence emission.

Amplification: Transimpedance Amplifier (TIA) converts photocurrent to voltage.

Digitization: ADS1115 converts voltage to digital signal (I2C).

Processing: ESP32 filters noise and calculates results.

[Insert Image of Block Diagram Here]

Hardware Documentation

Prototype Build (Current DIP Components)

Used for the current working breadboard/perfboard version.

Component

Part Number

Description

MCU

ESP32-WROOM

Main controller DevKit.

ADC

ADS1115

16-Bit, 4-Channel ADC with PGA.

Op-Amp

MCP602-I/P

Dual Channel, Rail-to-Rail, DIP package.

Photodiode

Vishay BPW34

Broad spectral range (430nm-1100nm).

Display

2.4" TFT LCD

ILI9341 Driver, SPI Interface.

LED Driver

Generic 2W/3W Module

350mA Constant Current.

Target Design (SMD Components)

Selected for the final PCB version to minimize size.

Op-Amp: OPA320 (Precision, 20MHz, 0.2pA bias current).

Switching: ADG704 Analog Mux (For switching feedback resistors/gain).

MOSFETs: HXY N-Channel SOT-23 (For LED/Power control).

Feedback: 0805 SMD Resistors/Caps (100k, 1M, 10M / 33pF, 330pF).

Wiring Map (Prototype)

ESP32 to ILI9341 TFT:

MOSI -> GPIO 23

CLK  -> GPIO 18

CS   -> GPIO 15

DC   -> GPIO 2

RST  -> GPIO 4

ESP32 to ADS1115:

SDA -> GPIO 21

SCL -> GPIO 22

Software & Firmware

The firmware is written in C++ (Arduino Framework).

Required Libraries:

Adafruit_ADS1X15 (For the ADC)

TFT_eSPI (For the Display)

SPI & Wire

Key Logic:
The system uses a differential measurement approach (if applicable) or simple averaging to reduce noise.

Dark Current Subtraction: Measures PD output with LED OFF.

Excitation: Turns LED ON via MOSFET/Driver.

Sampling: Takes N samples via ADS1115 and averages them.

Calculation: Result = (Active_Reading - Dark_Reading) * Calibration_Factor

Mechanical Enclosure

The enclosure is 3D printed and houses the cuvette, LED, and Photodiode.

Geometry: 90-degree alignment to minimize scattering interference.

Material: Printed in Black PLA (recommended painting inside with matte black to reduce reflection).

📂 Files:

\CAD\Enclosure_v1.stl: Main housing body.

\CAD\Lid_v1.stl: Light-tight lid.

⚠️ Current Mechanical Status:
The current v1 model has tolerance issues. The slot for the cuvette is slightly too tight, and the photodiode alignment needs to be raised by 2mm. Do not print v1 for final usage. Reference v2_wip branch for updates.

Setup & Usage

Wiring: Assemble the breadboard circuit according to the schematics/breadboard_wiring.png.

Libraries: Install the required libraries in Arduino IDE/PlatformIO.

Config: Check config.h to select your LED wavelength and Gain resistor values.

Flash: Upload the code to the ESP32.

Calibration:

Insert a blank (solvent only). Press 'Calibrate Zero'.

Insert a known standard. Press 'Calibrate High'.

Known Issues & Future Improvements

This section is for contributors continuing the project.

🔴 Immediate Fixes Needed

3D Print Dimensions: The enclosure needs re-dimensioning. The cuvette fit is too tight, causing potential scratches on the optical faces.

Light Leakage: The current lid allows ambient light in. A gasket or lip-groove design is needed.

🟡 Electronics Improvements

PCB Design: Move from the MCP602/Breadboard setup to the OPA320 SMD design.

Gain Switching: Implement the ADG704 Mux code to allow auto-ranging (switching between 100k, 1M, and 10M feedback resistors automatically based on signal strength).

🟢 Software Goals

SD Card Logging: Utilize the SD slot on the TFT display to save data points.

Battery Management: Add battery level monitoring via a voltage divider on an ESP32 ADC pin.

Credits

Lead Developer: Poojan Virani

Supervisor: Dr. Mainak Banerjee

Institution: Bits Pilani Goa Campus
