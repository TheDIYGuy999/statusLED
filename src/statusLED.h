/*
 statusLED.cpp - Library for status LED control (blinking, flash sequences, on, off, pwm)
 Created by TheDIYGuy999 May 2016 - Dez. 2019
 Released into the public domain.
 
 Dez. 2019: added ESP32 support
 */

#ifndef statusLED_h
#define statusLED_h

#include "Arduino.h"

// Class definition (header) ========================================================================
class statusLED {
  public:
    statusLED(bool inverse);
#if defined __AVR_ATmega32U4__ || __AVR_ATmega328P__ // AVR platform
    void begin(int pin1);
#else // ESP32 platform
    void begin(int pin1, int channel, int frequency, int resolution = 8);
#endif
    
    bool flash(unsigned long onDuration, unsigned long offDuration, unsigned long pauseDuration, int pulses, int delay = 0, int bulbSimRamp = 0, int flashOffBrightness = 0);
    void on();
    void off(int bulbSimRamp = 0, int offOffBrightness = 0);
    void pwm(int brightness);

  private:
    int _pin1;
    int _frequency;
    unsigned long _onDuration;
    unsigned long _offDuration;
    unsigned long _pauseDuration;
    unsigned long _delay;
    int _pulses;
    int _pulseCnt = 0;
    int _channel = 0;
    int _brightness;
    int _flashBrightness = 0;
    int _flashOffBrightness = 0;
    int _offBrightness = 0;
    int _offOffBrightness = 0;
    int _bulbSimRamp;
    int _offBulbSimRamp;
    unsigned long _previousMillis = 0;
    unsigned long _previousFlashRampMillis = 0;
    unsigned long _previousOffRampMillis = 0;
    byte _state = 0;
    bool _inverse;
    bool _start;
    bool _up;
    bool _down;
};

#endif
