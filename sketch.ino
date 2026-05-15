
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

  // ── Light (LDR) ───────
  Serial.print("Light Level : "); Serial.println(lightLevel);
  Blynk.virtualWrite(V5, lightLevel);
  static bool lightAlertSent = false;
  if (lightLevel < 800) {
    digitalWrite(safetyLED, HIGH);
    Serial.println("Safety LED  : ON");
    if (!lightAlertSent) {
      Blynk.logEvent("low_light", "It is getting dark! Safety LED turned ON.");
      lightAlertSent = true;
    }
  } else {
    digitalWrite(safetyLED, LOW);
    Serial.println("Safety LED  : OFF");
    lightAlertSent = false;
  }

  // ── Obstacle ────
  Serial.print("Distance    : "); Serial.print(distance); Serial.println(" cm");
  if (distance > 0 && distance < 50) {
    emergency = true;
    Blynk.virtualWrite(V1, "OBSTACLE!");
    Serial.println("ALERT: OBSTACLE DETECTED!");
  } else {
    Blynk.virtualWrite(V1, "Path Clear");
  }

  // ── Heart Rate ──
  Serial.print("Heart Rate  : "); Serial.print(heartRate); Serial.println(" BPM");
  Blynk.virtualWrite(V4, heartRate);
  if (heartRate > 100) {
    emergency = true;
    Serial.println("ALERT: HIGH HEART RATE!");
    Blynk.logEvent("high_heart_rate", "High heart rate detected!");
  }

  // ── SOS Button ──
  if (digitalRead(sosButton) == LOW) {
    emergency = true;
    Blynk.virtualWrite(V2, "SOS ACTIVE");
    Serial.println("ALERT: SOS BUTTON PRESSED!");
    Blynk.logEvent("sos_alert", "Emergency! SOS button pressed!");
  } else {
    Blynk.virtualWrite(V2, "Normal");
  }

  // ── Fall Detection ──
  if (mpuFound) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    if (abs(a.acceleration.x) > 15 || abs(a.acceleration.y) > 15) {
      emergency = true;
      Blynk.virtualWrite(V7, "FALL DETECTED!");
      Serial.println("ALERT: FALL DETECTED!");
      Blynk.logEvent("fall_detected", "Fall detected! User may need help.");
    } else {
      Blynk.virtualWrite(V7, "Device Stable");
    }
  }

 // ── Buzzer ──
if (emergency) {
  tone(buzzer, 1000); // play alarm sound
} else {
  noTone(buzzer);     // stop sound
}

}

//   SETUP
void setup() {
  Serial.begin(115200);
  pinMode(trigPin,   OUTPUT);
  pinMode(echoPin,   INPUT);
  pinMode(buzzer,    OUTPUT);
  pinMode(sosButton, INPUT_PULLUP);
  pinMode(ldrPin,    INPUT);
  pinMode(safetyLED, OUTPUT);

  Serial.println("\n===== INITIALIZING =====");

  WiFi.begin("Wokwi-GUEST", "");
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
  Serial.println("Blynk Connected!");

  Wire.begin();
  if (mpu.begin()) {
    mpuFound = true;
    Serial.println("MPU6050 Ready!");
  } else {
    Serial.println("MPU6050 not found.");
  }

  Serial.println("SYSTEM READY!\n");
  timer.setInterval(2000L, checkSystem);
}

//   LOOP
void loop() {
  Blynk.run();
  timer.run();
}
