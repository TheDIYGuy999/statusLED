/*
  statusLED.h - Library for status LED control (blinking, flash sequences, on, off, pwm)
  Created by TheDIYGuy999 May 2016
  Released into the public domain.
*/
#ifndef statusLED_h
#define statusLED_h

#include "Arduino.h"

// Class definition (header) ========================================================================
class statusLED {
  public:
    statusLED(bool inverse);
    void begin(int pin1);
    void flash(unsigned long onDuration, unsigned long offDuration, unsigned long pauseDuration, int pulses);
    void on();
    void off();
    void pwm(int brightness);

  private:
    int _pin1;
    unsigned long _onDuration;
    unsigned long _offDuration;
    unsigned long _pauseDuration;
    int _pulses;
    int _pulseCnt = 0;
    int _brightness;
    unsigned long _previousMillis = 0;
    byte _state = 0;
    bool _inverse;
};

#endif
