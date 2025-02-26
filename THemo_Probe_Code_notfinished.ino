/*************
Sebastian Psoada
thermo probe code
Used alot of ChatGPT
*******************/


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include "images.h"  // Include your images header file

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // I2C address for the OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Temperature Sensor Settings
#define TEMP_SENSOR_PIN 8
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

// MPU6050 Settings
MPU6050 mpu;
int16_t ax, ay, az;
int screenRotation = 0;

// Button Settings
#define UNIT_BUTTON_PIN 5
#define LOCK_BUTTON_PIN 3         // Lock Button with ISR
#define CALIBRATION_PIN 4         // Calibration Button
#define BACKLIGHT_BUTTON_PIN 6    // Backlight Toggle Button
#define ON_OFF_BUTTON_PIN 13      // On/Off Button
#define LED_PIN 10                // LED (Backlight)

// Variables
float temperature = 0;
int unit = 1;          // 1 = Fahrenheit (Default), 0 = Celsius
volatile bool isLocked = false; // Temperature lock state (volatile for ISR)
volatile unsigned long lastLockPressTime = 0; // Debouncing for ISR

// On/Off State
int deviceState = 1;   // 1 = ON (Default), 0 = OFF
int onOffButtonState = HIGH;
int lastOnOffButtonState = HIGH;
unsigned long lastOnOffDebounceTime = 0;
unsigned long debounceDelay = 50;  // 50 ms debounce delay

// Backlight State
int ledState = 0; 
int backlightButtonState = LOW;
int lastBacklightButtonState = LOW;
unsigned long lastBacklightDebounceTime = 0;

// Calibration Button
int lastCalibrationButtonState = LOW;

// Debouncing for Unit Button
unsigned long lastUnitButtonPress = 0;
int lastUnitButtonState = HIGH;
int unitButtonState = HIGH;

// ======== Function Definitions ========

void toggleLock() {
  if (millis() - lastLockPressTime > 200) {
    isLocked = !isLocked;
    lastLockPressTime = millis();
  }
}

void checkOrientation() {
  mpu.getAcceleration(&ax, &ay, &az);

  if (ay > 16000) {
    screenRotation = 3;  // 270° (Tilted Left)
  } else if (ay < -16000) {
    screenRotation = 1;  // 90° (Tilted Right)
  } else if (az > 16000) {
    screenRotation = 2;  // 180° (Upside Down)
  } else {
    screenRotation = 0;  // 0° (Normal)
  }

  display.setRotation(screenRotation);
}

void updateTemperature() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) {
    temperature = (unit == 1) ? (tempC * 9.0 / 5.0) + 32.0 : tempC;
  } else {
    Serial.println("Error: Could not read temperature data");
  }
}

void checkUnitButton() {
  unitButtonState = digitalRead(UNIT_BUTTON_PIN);

  if (unitButtonState == LOW && lastUnitButtonState == HIGH) {
    if (millis() - lastUnitButtonPress > debounceDelay) {
      unit = 1 - unit;
      lastUnitButtonPress = millis();
    }
  }
  lastUnitButtonState = unitButtonState;
}

void displayTemperature() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(temperature, 2);
  display.print(" ");
  display.print(unit == 1 ? "F" : "C");
}

void displayLockIcon() {
  if (isLocked) {
    display.drawBitmap(0, 0, images[0], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);  // Locked Icon
  } else {
    display.drawBitmap(0, 0, images[1], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);  // Unlocked Icon
  }
}

void handleOnOffButton() {
  int reading = digitalRead(ON_OFF_BUTTON_PIN);

  if (reading != lastOnOffButtonState) {
    lastOnOffDebounceTime = millis();
  }

  if ((millis() - lastOnOffDebounceTime) > debounceDelay) {
    if (reading != onOffButtonState) {
      onOffButtonState = reading;
      if (onOffButtonState == LOW) {
        deviceState = 1 - deviceState;
        Serial.println(deviceState ? "Temp-Probe ON" : "Temp-Probe OFF");
      }
    }
  }
  lastOnOffButtonState = reading;
}

void handleBacklightButton() {
  int reading = digitalRead(BACKLIGHT_BUTTON_PIN);

  if (reading != lastBacklightButtonState) {
    lastBacklightDebounceTime = millis();
  }

  if ((millis() - lastBacklightDebounceTime) > debounceDelay) {
    if (reading != backlightButtonState) {
      backlightButtonState = reading;
      ledState = 1 - ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
  lastBacklightButtonState = reading;
}

void handleCalibrationButton() {
  int buttonState = digitalRead(CALIBRATION_PIN);

  if (buttonState == LOW && lastCalibrationButtonState == HIGH) {
    Serial.println("Temp Sensor is Calibrating");
    delay(200); // Debounce delay
  }
  lastCalibrationButtonState = buttonState;
}

// ======== setup() and loop() ========

void setup() {
  Serial.begin(9600);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setRotation(0); // Start with normal rotation

  sensors.begin();

  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }

  pinMode(UNIT_BUTTON_PIN, INPUT);
  pinMode(LOCK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CALIBRATION_PIN, INPUT); 
  pinMode(BACKLIGHT_BUTTON_PIN, INPUT); 
  pinMode(LED_PIN, OUTPUT);         
  pinMode(ON_OFF_BUTTON_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(LOCK_BUTTON_PIN), toggleLock, FALLING);
}

void loop() {
  handleOnOffButton();
  if (deviceState == 1) {
    checkOrientation();
    if (!isLocked) updateTemperature();
    checkUnitButton();
    displayTemperature();
    displayLockIcon();
    handleBacklightButton();
    handleCalibrationButton();
    display.display();
    delay(1000);
  } else {
    display.clearDisplay();
    display.display();
    digitalWrite(LED_PIN, LOW);
  }
}
