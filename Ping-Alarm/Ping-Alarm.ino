// This #include statement was automatically added by the Spark IDE.
#include "Ping.h"

// This #include statement was automatically added by the Spark IDE.
#include "Arduino.h"

const int ledPin = D0;
const int buzzerPin = D1;
const int sonarPin = D7;
const int loopDuration = 500;

const int sonarDistanceMin = 100;
const int buzzerDuration = 2000;
const int buzzerFrequence = 2000;

int buzzerCountDown = 0;

Ping sonar(sonarPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
}

void loop() {
  sonar.fire();
  double distance = sonar.centimeters();

  if ((distance < sonarDistanceMin) && (buzzerCountDown < 0)) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, buzzerFrequence, 0);
    buzzerCountDown = buzzerDuration;
  }
  else if (buzzerCountDown == 0) {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    buzzerCountDown = -1;
  }
  else if (buzzerCountDown > 0) {
    buzzerCountDown -= loopDuration;
  }
  delay(loopDuration);
}
