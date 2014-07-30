/*
 Ping.cpp - Library for using Ping))) Sensors with Arduino - Version 2
 Copyright (c) 2009 Caleb Zulawski.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"
#include "Ping.h"

Ping::Ping(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _in = 0;
  _cm = 0;
  _duration = -1;
}
Ping::Ping(int pin, double in, double cm)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _in = in;
  _cm = cm;
  _duration = -1;
}

void Ping::fire()
{
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(_pin, LOW);
  pinMode(_pin, INPUT);
  _duration = pulseIn(_pin, HIGH);
  
}

int Ping::microseconds()
{
  return _duration;
}

double Ping::inches()
{
  if(_duration != -1){
    return _duration / (74+_in) / 2;
  }else{
    return -1;
  }
}

double Ping::centimeters()
{
  if(_duration != -1){
    return _duration / (29+_cm) / 2;
  }else{
    return -1;
  }
}

/* 
 * pulseIn Function for the Spark Core - Version 0.1.1 (Beta)
 * Copyright (2014) Timothy Brown - See: LICENSE
 *
 * Due to the current timeout issues with Spark Cloud
 * this will return after 10 seconds, even if the
 * input pulse hasn't finished.
 *
 * Input: Trigger Pin, Trigger State
 * Output: Pulse Length in Microseconds (10uS to 10S)
 *
 */

unsigned long Ping::pulseIn(uint16_t pin, uint8_t state) {
    
    GPIO_TypeDef* portMask = (PIN_MAP[pin].gpio_peripheral); // Cache the target's peripheral mask to speed up the loops.
    uint16_t pinMask = (PIN_MAP[pin].gpio_pin); // Cache the target's GPIO pin mask to speed up the loops.
    unsigned long pulseCount = 0; // Initialize the pulseCount variable now to save time.
    unsigned long loopCount = 0; // Initialize the loopCount variable now to save time.
    unsigned long loopMax = 20000000; // Roughly just under 10 seconds timeout to maintain the Spark Cloud connection.
    
    // Wait for the pin to enter target state while keeping track of the timeout.
    while (GPIO_ReadInputDataBit(portMask, pinMask) != state) {
        if (loopCount++ == loopMax) {
            return 0;
        }
    }
    
    // Iterate the pulseCount variable each time through the loop to measure the pulse length; we also still keep track of the timeout.
    while (GPIO_ReadInputDataBit(portMask, pinMask) == state) {
        if (loopCount++ == loopMax) {
            return 0;
        }
        pulseCount++;
    }
    
    // Return the pulse time in microseconds by multiplying the pulseCount variable with the time it takes to run once through the loop.
    return pulseCount * 0.405; // Calculated the pulseCount++ loop to be about 0.405uS in length.
}
