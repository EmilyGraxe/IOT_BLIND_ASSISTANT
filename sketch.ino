
//   SMART BLIND STICK 
//   Only Blynk + Sensors

#define BLYNK_TEMPLATE_ID   "TMPL3Y_NBiYqi"
#define BLYNK_TEMPLATE_NAME "Smart-Blind-Stick"
#define BLYNK_AUTH_TOKEN    "IMVRh7MjLEm-eZOU06sox5vjEjl18Zab"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// ── Pins ──
const int trigPin      = 5;
const int echoPin      = 18;
const int buzzer       = 2;
const int sosButton    = 4;
const int heartRatePin = 32;
const int ldrPin       = 34;
const int safetyLED    = 25;

Adafruit_MPU6050 mpu;
bool mpuFound = false;
BlynkTimer timer;

//   SENSOR CHECK — every 2 seconds

void checkSystem() {

  // Ultrasonic distance
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int  distance = (duration * 0.034) / 2;

  int  heartRate  = map(analogRead(heartRatePin), 0, 4095, 60, 160);
  int  lightLevel = analogRead(ldrPin);
  bool emergency  = false;

  Serial.println("\n===== LIVE MONITORING =====");

  