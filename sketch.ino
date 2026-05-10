#define BLYNK_TEMPLATE_ID "TMPLxxxx"
#define BLYNK_TEMPLATE_NAME "Blind Assistant"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

// LEFT SENSOR
#define trigLeft 5
#define echoLeft 18

// CENTER SENSOR
#define trigCenter 17
#define echoCenter 16

// RIGHT SENSOR
#define trigRight 4
#define echoRight 2

// VIBRATION MOTORS
#define motorLeft 25
#define motorCenter 26
#define motorRight 27

// BUZZER
#define buzzer 14

long duration;

int distanceLeft;
int distanceCenter;
int distanceRight;

void setup() {

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);

  pinMode(trigCenter, OUTPUT);
  pinMode(echoCenter, INPUT);

  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  pinMode(motorLeft, OUTPUT);
  pinMode(motorCenter, OUTPUT);
  pinMode(motorRight, OUTPUT);

  pinMode(buzzer, OUTPUT);

  Serial.println("Smart Blind Navigation Assistant Started");
}

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

void loop() {

  Blynk.run();

  distanceLeft = getDistance(trigLeft, echoLeft);
  distanceCenter = getDistance(trigCenter, echoCenter);
  distanceRight = getDistance(trigRight, echoRight);

  Serial.print("Left: ");
  Serial.print(distanceLeft);
  Serial.println(" cm");

  Serial.print("Center: ");
  Serial.print(distanceCenter);
  Serial.println(" cm");

  Serial.print("Right: ");
  Serial.print(distanceRight);
  Serial.println(" cm");

  // LEFT OBSTACLE
  if (distanceLeft < 50) {

    digitalWrite(motorLeft, HIGH);

    Blynk.virtualWrite(V0, "Obstacle Left");

  } else {

    digitalWrite(motorLeft, LOW);
  }

  // CENTER OBSTACLE
  if (distanceCenter < 50) {

    digitalWrite(motorCenter, HIGH);

    digitalWrite(buzzer, HIGH);

    Blynk.virtualWrite(V1, "Obstacle Ahead");

  } else {

    digitalWrite(motorCenter, LOW);

    digitalWrite(buzzer, LOW);
  }

  // RIGHT OBSTACLE
  if (distanceRight < 50) {

    digitalWrite(motorRight, HIGH);

    Blynk.virtualWrite(V2, "Obstacle Right");

  } else {

    digitalWrite(motorRight, LOW);
  }

  delay(200);
}