#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <BleGamepad.h>

#define LED_PIN        2
#define FLEX_BRAKE     34
#define FLEX_ACCEL     35
#define VIB_MOTOR_1    25
#define VIB_MOTOR_2    33

MPU6050 mpu;
BleGamepad bleGamepad("ESP32 Racing Controller", "Aravindh", 100);

int16_t ax, ay, az;
int16_t gx, gy, gz;

int flexBrakeMin = 340,  flexBrakeMax = 720;
int flexAccelMin = 1150, flexAccelMax = 1410;

int AX_CENTER = 1850, AX_RANGE = 200;
int AY_CENTER = 840,  AY_RANGE = 200;

float smoothX = 0, smoothY = 0;
const float SMOOTHING = 0.2;

int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(FLEX_BRAKE, INPUT);
  pinMode(FLEX_ACCEL, INPUT);
  pinMode(VIB_MOTOR_1, OUTPUT);
  pinMode(VIB_MOTOR_2, OUTPUT);
  digitalWrite(VIB_MOTOR_1, HIGH);
  digitalWrite(VIB_MOTOR_2, HIGH);

  Wire.begin(21, 22);
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("MPU6050 connection FAILED");
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(150);
      digitalWrite(LED_PIN, LOW);
      delay(150);
    }
  }

  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setAutoReport(false);
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setButtonCount(4);
  bleGamepadConfig.setWhichAxes(true, true, true, true, false, false, false, false);
  bleGamepadConfig.setAxesMin(0);
  bleGamepadConfig.setAxesMax(255);

  bleGamepad.begin(&bleGamepadConfig);
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int rawBrake = analogRead(FLEX_BRAKE);
  int rawAccel = analogRead(FLEX_ACCEL);

  int brakeValue = map(rawBrake, flexBrakeMax, flexBrakeMin, 0, 255);
  int accelValue = map(rawAccel, flexAccelMax, flexAccelMin, 0, 255);
  brakeValue = constrain(brakeValue, 0, 255);
  accelValue = constrain(accelValue, 0, 255);

  int rawX = clampInt((int)((ax - AX_CENTER) / (float)AX_RANGE * 127 + 127), 0, 255);
  int rawY = clampInt((int)((ay - AY_CENTER) / (float)AY_RANGE * 127 + 127), 0, 255);

  smoothX = smoothX + SMOOTHING * (rawX - smoothX);
  smoothY = smoothY + SMOOTHING * (rawY - smoothY);

  int steerX = clampInt((int)smoothX, 0, 255);
  int steerY = clampInt((int)smoothY, 0, 255);

  if (brakeValue > 50) {
    digitalWrite(VIB_MOTOR_1, LOW);
  } else {
    digitalWrite(VIB_MOTOR_1, HIGH);
  }

  if (abs(ax) > 4500 || abs(ay) > 4500) {
    digitalWrite(VIB_MOTOR_2, LOW);
  } else {
    digitalWrite(VIB_MOTOR_2, HIGH);
  }

  if (bleGamepad.isConnected()) {
    bleGamepad.setAxes(steerX, steerY, brakeValue, accelValue, 0, 0, 0, 0);
    bleGamepad.sendReport();
  }

  Serial.print("Steer_X: "); Serial.print(steerX);
  Serial.print(" Steer_Y: "); Serial.print(steerY);
  Serial.print(" Brake: "); Serial.print(brakeValue);
  Serial.print(" Accel: "); Serial.println(accelValue);

  delay(20);
}
