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

bool shouldRun = false; // global state variable for remote functions
int buzzerCountDown = 0;

Ping sonar(sonarPin);

void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  
  Spark.variable("status", &shouldRun, INT);
  Spark.function("start",remoteStart);
  Spark.function("stop",remoteStop);
}

void loop() 
{
  if (shouldRun) {
    // Running
    sonar.fire();
    double distance = sonar.centimeters();

    if ((distance < sonarDistanceMin) && (buzzerCountDown < 0)) {
      Spark.publish("alarm", NULL, 60, PRIVATE);
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
  }
  else if (buzzerCountDown != -1) {
    // Should Stop
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    buzzerCountDown = -1;
  }
  delay(loopDuration);
}

/*
 * Remote functions
 */
int remoteStart(String args) 
{
    shouldRun = true;
    return shouldRun;
}

int remoteStop(String args) 
{
    shouldRun = false;
    return shouldRun;
}
