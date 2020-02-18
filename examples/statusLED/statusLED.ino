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

  // AVR platform:
#if defined __AVR_ATmega32U4__ || __AVR_ATmega328P__
  greenLED.begin(5); // Green LED on pin 5
  redLED.begin(6); // Red LED on pin 6

  // ESP32 platform: use a separate timer for each LED, but not timer 0, because of millis()
#else
  greenLED.begin(33, 2, 31500); // Green LED on pin 33, Timer 2, 31500Hz
  redLED.begin(32, 3, 31500); // Green LED on pin 32, Timer 3, 31500Hz
#endif

  // LED demo (1x executed)
  redLED.on();
  delay(2000);
  greenLED.on();
  delay(1000);
  redLED.pwm(10); // PWM capable pins required!
  delay(1000);
  greenLED.pwm(50);
  delay(1000);
  redLED.off();
  delay(1000);
  greenLED.off();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Flashing sequences
  redLED.flash(140, 150, 700, 5, 70); // ON, OFF, PAUSE, PULSES, (OPTIONAL DELAY FOR FIRST PASS)
  greenLED.flash(40, 350, 0, 0);


}
