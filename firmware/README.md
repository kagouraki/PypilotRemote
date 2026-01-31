# Firmware

This directory contains the firmware for the **Pypilot Remote** device.

The firmware is written for the ESP32-S3 platform using the Arduino framework and is developed
primarily with **Eclipse Sloeber**, but it can also be built using the Arduino IDE when the
requirements below are met.

---

## Compilation Requirements

### ESP32 Core
- **ESP32 Board Manager:** `v2.0.14`

---

### Libraries

The following libraries and versions are required:

- **LVGL:** `v8.3.11`  
  - Copy the provided `lv_conf.h` into the **root of the Arduino libraries folder**

- **TFT_eSPI:** `v2.5.43`  
  - Edit `User_Setup.h` with the following configuration:

```c
#define ST7789_DRIVER
#define TFT_RGB_ORDER TFT_BGR

#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  14

#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY 2500000
```

- Replace the TFT driver files:
  - Copy and overwrite the contents of the `st7789` folder
  - Destination: `TFT_eSPI/TFT_Drivers/`

---

### Networking & System Libraries

- **AsyncTCP:** `v1.1.4`
- **ESPAsyncWebServer:** `v3.1.0`

The following ESP32 core libraries are also used:

- FS  
- Preferences  
- SPI  
- SPIFFS  
- Update  
- WiFi  

---

## Partition Table

The firmware requires the **custom partition table provided in this repository**.  
Make sure it is selected before compiling and flashing.

---

## Flashing & Programming

### First Flash (Required)

- The **first flash must be done using a UART adapter**
- This is required to bootstrap the device

---

### Subsequent Programming Options

After the initial flash, the firmware can be updated using:

- **USB via SeatalkNG cable**
- **OTA (Over-The-Air)** updates

#### USB via SeatalkNG

Connect:
- **D+**
- **D−**
- **GND**

from a USB cable to the SeatalkNG connector.

⚠️ **Power must still be supplied separately.**

---

### Serial Output over USB

When programming via USB on the SeatalkNG cable:

- Enable **CDC on boot**
- This is required to get serial output after flashing

---

## Notes

- Library versions matter — mismatched versions may result in build failures
- TFT and LVGL configuration must match the hardware exactly
- This firmware is tightly coupled to the display and pinout used in the hardware design

---

## License

This firmware is licensed under the  
**GNU General Public License v3.0 (GPL-3.0-only)**.

See the `LICENSE` file in the root of the repository for details.
