<p align="center">
  <img src="assets/logo300x300.png" alt="Pypilot Remote logo" width="300"/>
</p>

# Pypilot Remote

## Overview

**Pypilot Remote** is a Wi-Fi remote control and display for the  
[pypilot](https://github.com/pypilot/pypilot) open-source marine autopilot by Sean D’Epagnier.

It connects to a pypilot server in client mode and provides a **clean, dedicated hardware interface**
to control the autopilot and display essential navigation and system information.

The project is currently housed in a **Raymarine ST70 autopilot display**, giving new life to legacy
marine hardware. The design is intentionally adaptable so it can be reused with other discontinued
or surplus commercial enclosures.

A future goal is a **custom 3D-printed enclosure**, which would make the project more accessible
without relying on expensive legacy hardware. This will require additional work around waterproof
screen mounting, buttons, and connectors.

<p align="center">
  <img src="assets/ready.jpg" alt="Pypilot Remote assembled and ready" width="800"/>
</p>

---

## Hardware

- **MCU:** ESP32-S3 N16R8 with external antenna  
- **Display:** 3.47" transflective color LCD  
  - Resolution: 480×320  
  - Model: UEED035HV-RX40-L001B (Viewe)  
- **Input:** 9 physical pushbuttons (no touchscreen)
- **Power input:** 10–18 V DC
- **Programming:**  
  - USB (without opening the enclosure)  
  - OTA (Over-The-Air updates)

---

## Software & Design Tools

- **Firmware:** ESP32 (Arduino framework)
- **IDE:** Eclipse Sloeber
- **Hardware design:** KiCad v9
- **License:** GPL-3.0-only

---

## Features

- Connects to a pypilot server or creates an AP for configuration
- Autopilot **Auto / Standby** control
- Steering mode selection
- Course changes in Auto mode:
  - −1°, −10°, +1°, +10°
- Adjustable backlight for screen and buttons
- Light and dark UI themes
- Information screens:
  - Pypilot data
  - Device status
- Scrollable console log
- “Return to previous heading” Auto command
- Persistent configuration stored in EEPROM
- Robust Wi-Fi and TCP reconnection handling
- Visual Wi-Fi status indicator

---

## Button Functions

### Autopilot Control
- **Standby**  
  Disengages the autopilot

- **Auto**  
  Engages the autopilot and commands the current heading

- **Auto (long press > 3 s)**  
  Engages autopilot using the **previous heading command**

### Course Adjustment
- **−1** → Port 1°  
- **−10** → Port 10°  
- **+1** → Starboard 1°  
- **+10** → Starboard 10°

### Mode & Interface
- **Mode**  
  Cycles through available steering modes

- **Backlight**  
  Toggles backlight on/off

- **Backlight (long press > 10 s)**  
  Forces AP mode

- **Info**  
  Cycles through information screens and returns to home

- **Backlight + Mode (long press > 5 s)**  
  Restarts the Pypilot Remote

### Notes
- When **not on the home screen**, only **Auto** and **Standby** are active
- On the console log screen:
  - **+1 / +10** scroll up and down

---

## Connection Logic

1. On boot, the device attempts to connect to the configured Wi-Fi SSID  
   → Wi-Fi icon flashes **RED**
2. After the configured number of retries:
   - If unsuccessful → switches to **AP mode** (Wi-Fi icon **YELLOW**)
3. Once Wi-Fi is connected:
   - Tries to connect to the pypilot TCP server indefinitely  
   → Wi-Fi icon flashes **GREEN**
4. When connected to pypilot:
   - Wi-Fi icon becomes **solid GREEN**
   - Device is fully operational

---

## Configuration Web Interface

When in **AP mode**, access the configuration interface at:

```
http://192.168.4.1
```

### Available Settings

- **WiFi SSID** – Network to connect to
- **WiFi Password**
- **Max WiFi Retries** – Attempts before falling back to AP mode
- **Pypilot IP** – IP address of the pypilot server
- **TCP Port** – Default: `23322`
- **UDP Port** – Not used
- **Brightness (Backlight Off)** – 0–255
- **Brightness (Backlight On)** – 0–255
- **Key Backlight Brightness** – 0–255
- **Start Theme** – Theme when backlight is off
- **Backlight Theme** – Theme when backlight is on
- **Rescan Wi-Fi** – Refresh available networks
- **OTA Update** – Upload firmware `.bin` file

---

## Electrical Connections

The device uses **4 pins** from the SeatalkNG connector:

| Color | Signal |
|-----|-------|
| Red | +12 V |
| Black | GND |
| Blue | D+ |
| White | D− |

### Important Warning

⚠️ **DO NOT CONNECT TO A SEATALKNG NETWORK**

- Blue and white lines are **CAN-H / CAN-L**
- Connecting them to the ESP32 **will destroy the MCU**

### USB Programming (Optional)

For USB programming or debugging:
- Splice a USB cable
- Connect **D+**, **D−**, and **GND**
- A serial port will appear on the host computer

---

## Video

📺 **Demo video:**  
*(Placeholder – link will be added once published on YouTube)*

---

## License

This project is licensed under the  
**GNU General Public License v3.0 (GPL-3.0-only)**.

See the `LICENSE` file for details.

---

## Contributing

Contributions, improvements, and adaptations to other hardware platforms
are welcome — as long as they respect the GPL.

Let’s keep open source alive.
