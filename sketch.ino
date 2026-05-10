/*
========================================================
SMARTASSIST
IoT Hybrid Navigation System
for Visually Impaired Persons

Features:
- Front obstacle detection
- Ground obstacle detection
- Left/Right wearable vibration alerts
- Buzzer warning
- ESP32 WiFi IoT
- Blynk notifications

Board: ESP32
========================================================
*/

#define BLYNK_TEMPLATE_ID "TMPLxxxx"
#define BLYNK_TEMPLATE_NAME "SmartAssist"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WIFI
char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

// ==========================
// FRONT SENSOR (CANE)
// ==========================
#define trigFront 5
#define echoFront 18

// ==========================
// GROUND SENSOR (DOWNWARD)
// ==========================
#define trigGround 17
#define echoGround 16

// ==========================
// LEFT WRIST SENSOR
// ==========================
#define trigLeft 4
#define echoLeft 2

// ==========================
// RIGHT WRIST SENSOR
// ==========================
#define trigRight 15
#define echoRight 13

// ==========================
// VIBRATION MOTORS
// ==========================
#define motorLeft 25
#define motorRight 26

// ==========================
// BUZZER
// ==========================
#define buzzer 27

// ==========================
// VARIABLES
// ==========================
long duration;

int frontDistance;
int groundDistance;
int leftDistance;
int rightDistance;

// ==========================
// SETUP
// ==========================
void setup() {

  Serial.begin(115200);

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // FRONT SENSOR
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);

  // GROUND SENSOR
  pinMode(trigGround, OUTPUT);
  pinMode(echoGround, INPUT);

  // LEFT SENSOR
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);

  // RIGHT SENSOR
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  // MOTORS
  pinMode(motorLeft, OUTPUT);
  pinMode(motorRight, OUTPUT);

  // BUZZER
  pinMode(buzzer, OUTPUT);

  Serial.println("================================");
  Serial.println("SMARTASSIST SYSTEM STARTED");
  Serial.println("================================");
}

// ==========================
// DISTANCE FUNCTION
// ==========================
int getDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  int distance = duration * 0.034 / 2;

  return distance;
}

// ==========================
// MAIN LOOP
// ==========================
void loop() {

  Blynk.run();

  // READ DISTANCES
  frontDistance = getDistance(trigFront, echoFront);

  groundDistance = getDistance(trigGround, echoGround);

  leftDistance = getDistance(trigLeft, echoLeft);

  rightDistance = getDistance(trigRight, echoRight);

  // ==========================
  // SERIAL MONITOR
  // ==========================
  Serial.print("Front: ");
  Serial.print(frontDistance);
  Serial.println(" cm");

  Serial.print("Ground: ");
  Serial.print(groundDistance);
  Serial.println(" cm");

  Serial.print("Left: ");
  Serial.print(leftDistance);
  Serial.println(" cm");

  Serial.print("Right: ");
  Serial.print(rightDistance);
  Serial.println(" cm");

  Serial.println("---------------------------");

  // ==========================
  // FRONT OBSTACLE
  // ==========================
  if(frontDistance < 80) {

    digitalWrite(buzzer, HIGH);

    Blynk.virtualWrite(V0, "Front Obstacle Detected");

    Serial.println("WARNING: FRONT OBSTACLE");

  } else {

    digitalWrite(buzzer, LOW);
  }

  // ==========================
  // GROUND / STAIRS DETECTION
  // ==========================
  if(groundDistance > 100) {

    digitalWrite(buzzer, HIGH);

    Blynk.virtualWrite(V1, "Ground Gap / Stairs Detected");

    Serial.println("WARNING: STAIRS OR HOLE");

  }

  // ==========================
  // LEFT OBSTACLE
  // ==========================
  if(leftDistance < 60) {

    digitalWrite(motorLeft, HIGH);

    Blynk.virtualWrite(V2, "Obstacle On Left");

    Serial.println("LEFT OBSTACLE");

  } else {

    digitalWrite(motorLeft, LOW);
  }

  // ==========================
  // RIGHT OBSTACLE
  // ==========================
  if(rightDistance < 60) {

    digitalWrite(motorRight, HIGH);

    Blynk.virtualWrite(V3, "Obstacle On Right");

    Serial.println("RIGHT OBSTACLE");

  } else {

    digitalWrite(motorRight, LOW);
  }

  delay(200);
}