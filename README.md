# 🎮 ESP32 BLE Racing Controller

A motion-sensing, pedal-equipped Bluetooth gamepad — built from scratch on an ESP32. Tilt to steer, squeeze flex sensors to brake and accelerate. No dongle, no wires (once flashed) — it shows up as a native Bluetooth HID gamepad on any Linux, Windows, or Android device.

▶️ **[Watch the demo video](./esp_output_video.mp4)**

## Overview

This project turns an ESP32 + MPU6050 + flex sensors into a fully wireless racing controller that any BLE-HID-compatible system recognizes out of the box — verified working in [SuperTuxKart](https://supertuxkart.net/), the Linux `jstest` joystick tool, and the browser Gamepad API (`hardwaretester.com/gamepad`).

## Features
- **Native Bluetooth HID gamepad** — no bridge software, no drivers, no companion app. The ESP32 advertises itself directly as a BLE gamepad using `ESP32-BLE-Gamepad` + NimBLE.
- **Motion-based steering** — MPU6050 accelerometer/gyroscope tilt mapped to steering axis, with smoothing to reduce jitter.
- **Analog pedals** — two flex sensors for brake and acceleration, read as continuous analog axes (not just on/off buttons).
- **Haptic feedback** — vibration motors for in-game feedback.
- **Cross-platform** — recognized as a standard gamepad by Linux (`jstest`, kernel `hid-generic`), browsers (Gamepad API), and any BLE HID host.

## Hardware
| Component | Purpose |
|---|---|
| ESP32 DevKit | Main controller, BLE HID host |
| MPU6050 | Accelerometer/gyroscope for tilt-steering |
| 2x Flex sensors | Brake and acceleration input |
| 2x Vibration motors | Haptic feedback |
| Breadboard + jumper wires | Prototyping |

## Wiring
| Signal | Pin |
|---|---|
| MPU6050 SDA | GPIO 21 |
| MPU6050 SCL | GPIO 22 |
| Flex sensor (brake) | GPIO 34 |
| Flex sensor (accel) | GPIO 35 |
| Vibration motor 1 | GPIO 25 |
| Vibration motor 2 | GPIO 33 |

## Software Stack
- **PlatformIO** + Arduino framework
- [`ESP32-BLE-Gamepad`](https://github.com/lemmingDev/ESP32-BLE-Gamepad) (NimBLE-based BLE HID gamepad library)
- `MPU6050` library for motion sensing

## Build & Flash
```bash
pio run
pio run --target upload
```

## Pairing & Testing
1. Flash the firmware, power the ESP32 (USB or battery — both work simultaneously).
2. Pair via Bluetooth settings, or manually:
```bash
   bluetoothctl
   scan on
   pair <ESP32_MAC_ADDRESS>
   trust <ESP32_MAC_ADDRESS>
   connect <ESP32_MAC_ADDRESS>
```
3. Verify it's recognized as a gamepad:
```bash
   ls /dev/input/js*
   jstest /dev/input/jsX
```
   Or open `hardwaretester.com/gamepad` in a browser.

## Notes
- Calibration constants (tilt center/range, smoothing factor) may need adjustment for your specific MPU6050 mounting orientation and flex sensor characteristics — see the calibration section in `main.cpp`.
- Axis mapping is currently non-standard (not auto-recognized as an Xbox-style "STANDARD GAMEPAD"), so games with manual controller remapping (e.g. SuperTuxKart) work best.
- Built and debugged end-to-end on Ubuntu, including working around several NimBLE/PlatformIO build quirks along the way.
