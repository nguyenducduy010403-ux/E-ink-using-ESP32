<div align="center">
  <h1>ESP32-S3 E-Ink Display Driver</h1>
  <p><strong>Hardware configuration and minimal driver setup for 2.13" B/W/R (GDEH0213Z98) E-Paper Display using ESP32-S3</strong></p>
</div>

---

##  Overview

This repository provides the hardware pinout and software configuration required to drive a **2.13-inch Black/White/Red E-Ink Display (Model: GDEH0213Z98)** using an **ESP32-S3 (N16R8)** microcontroller. 

The project focuses on giving you a quick start with the correct pin mappings, initialization, and driver configuration without cluttering your workspace with unnecessary code.

##  Hardware Pinout

Please refer to the `pinout.png` diagram included in this repository for the exact wiring. The table below maps the E-Ink pads to the ESP32-S3 GPIOs:

| E-Ink Pad | ESP32-S3 GPIO | Priority / Wiring Notes |
| :--- | :--- | :--- |
| **TP14** | `GPIO 12` | **SCL (Clock)**   *Crucial: Keep wire as short as possible* |
| **TP15** | `GPIO 11` | **SDA (MOSI)**  *Crucial: Keep wire as short as possible* |
| **TP13** | `GPIO 10` | **CSB (Chip Select)** *Should be short, but less strict than SCL/SDA* |
| **TP12** | `GPIO 9` | **DC**  *Low priority: Slow state change* |
| **TP11** | `GPIO 8` | **RST_N**  *Low priority: Boot state change only* |
| **TP6** | `GPIO 7` | **BUSY_N**  *Low priority: 0/1 state read (Input)* |
| **TP5** | `GND` | **BS**  *Low priority: Hardwired to Ground* |
| **Pin C1 (Confirmed)** | `3.3V` | **VDD**  *Keep short + Mandatory decoupling capacitor nearby* |
| **D6 Pin 8** | `3.3V` | **VDDIO**  *Keep short + Decoupling capacitor nearby* |
| **GND1** | `GND` | **VSS (GND)**  *Keep short: A short GND path reduces overall system noise* |

> ⚠️ **GPIO Selection Note for ESP32-S3:**
> Pins `GPIO 7` to `GPIO 12` are chosen specifically to avoid conflicts. Do **NOT** use:
> * Strapping pins (0, 3, 45, 46).
> * PSRAM / Octal SPI Flash pins on N16R8 variants (GPIO 26 - 37).
> * Native USB pins (19, 20) if you plan on using USB features.

## 🚀 Driver Configuration & Minimal Demo

### Dependencies
This project uses the excellent **GxEPD2** library. You can install it directly from the Arduino Library Manager:
* `GxEPD2` by ZinggJM

### Code Snippet (Configuration & Init)

Here is the minimal code required to initialize the display. You don't need to copy massive image arrays—just use this boilerplate to start rendering your own graphics:

```cpp
#include <GxEPD2_3C.h> // Include library for 3-Color E-Ink displays

// --- PIN DEFINITIONS ---
#define PIN_CS    10
#define PIN_DC    9
#define PIN_RST   8
#define PIN_BUSY  7
#define PIN_SCK   12
#define PIN_MOSI  11

// --- DRIVER INSTANTIATION ---
// Using GDEH0213Z98c class for the 2.13" B/W/R display
GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(
  GxEPD2_213_Z98c(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY)
);

void setup() {
  Serial.begin(115200);
  
  // Initialize the display driver
  display.init(115200); 

  // --- MINIMAL DEMO ---
  display.setRotation(0); // 0 = portrait, 1 = landscape
  display.setFullWindow();
  
  // Start drawing sequence
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    
    // Draw some text or shapes here
    // e.g., display.setTextColor(GxEPD_RED);
    //       display.print("Hello Global!");
    
  } while (display.nextPage());

  // Put display to sleep after rendering to save power
  display.hibernate();
}

void loop() {
  // E-Ink displays do not require continuous refreshing
  // Once the image is drawn, you can put the ESP32 to Deep Sleep.
}
```

## 📝 License
This project is open-source and released under the **MIT License**. Feel free to use, modify, and distribute it for your own projects.
