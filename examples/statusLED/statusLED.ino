/*
  statusLED.h - Library for status LED control (blinking, flash sequences, on, off, pwm)
  Created by TheDIYGuy999 May 2016
  Released into the public domain.
*/

#include <statusLED.h> // TheDIYGuy999 library: https://github.com/TheDIYGuy999/statusLED

// Status LED objects
statusLED greenLED(false); // False = not inversed
statusLED redLED(false);

void setup() {
  // put your setup code here, to run once:

  // LED setup
  greenLED.begin(5); // Green LED on pin 5
  redLED.begin(6); // Red LED on pin 6

// LED demo (1x executed)
  redLED.on();
  delay(2000);
  greenLED.on();
  delay(1000);
  redLED.pwm(10); // PWM capable pins required!
  greenLED.pwm(20);
  delay(2000);
  greenLED.off();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Flashing sequences
  redLED.flash(140, 150, 700, 5); // ON, OFF, PAUSE, PULSES
  greenLED.flash(40, 350, 0, 0);
}
