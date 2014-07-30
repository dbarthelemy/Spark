// This #include statement was automatically added by the Spark IDE.
#include "Ping.h"

// This #include statement was automatically added by the Spark IDE.
#include "Arduino.h"

const int ledPin = D0;
const int sonarPin = D7;

Ping sonar(sonarPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  sonar.fire();
  double distance = sonar.centimeters();
  if (distance < 50) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
}
