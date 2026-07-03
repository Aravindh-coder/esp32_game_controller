# ESP32 Game Controller

A Bluetooth game controller built with an ESP32, MPU6050 motion sensor, and flex sensors for brake and acceleration input.

## Features
- Bluetooth gamepad output using the ESP32
- MPU6050-based motion input for steering
- Flex sensors for brake and acceleration
- Vibration motor feedback
- PlatformIO project structure

## Hardware
- ESP32 DevKit
- MPU6050 accelerometer/gyroscope
- 2 flex sensors
- 2 vibration motors
- Breadboard and jumper wires

## Wiring
- MPU6050 on I2C pins 21 (SDA) and 22 (SCL)
- Flex sensor brake on GPIO 34
- Flex sensor acceleration on GPIO 35
- Vibration motors on GPIO 25 and 33

## Software
- PlatformIO
- Arduino framework
- BleGamepad library
- MPU6050 library

## Build and Upload
1. Install PlatformIO
2. Open the project folder
3. Run:
   ```bash
   pio run
   pio run --target upload
   ```

## Notes
- The Bluetooth controller is intended for use with compatible gamepad-supported applications.
- Calibration values may need adjustment depending on your sensor and flex sensor setup.
