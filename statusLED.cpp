/*
 statusLED.cpp - Library for status LED control (blinking, flash sequences, on, off, pwm)
 Created by TheDIYGuy999 May 2016 - Dez. 2019
 Released into the public domain.
 
 Dez. 2019: added ESP32 support
 */

#include "Arduino.h"
#include "statusLED.h"


// Member definition (code) ========================================================================
statusLED::statusLED(bool inverse) { // Constructor (called, when new ojects of that class are created)
    _state = 0;
    _previousMillis = 0;
    _inverse = inverse;
}

// Begin function ************************************************************

#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__ // Atmega Platform
void statusLED::begin(int pin1) {
    _pin1 = pin1;
    pinMode(_pin1, OUTPUT);
    if (_inverse) digitalWrite(_pin1, HIGH);
    else digitalWrite(_pin1, LOW);
}

#else // ESP32 platform (using channel 0 will affect millis()! )
void statusLED::begin(int pin1, int channel, int frequency) {
    _pin1 = pin1;
    _channel = channel;
    _frequency = frequency;
    sigmaDeltaSetup(_channel, _frequency);
    sigmaDeltaAttachPin(_pin1, _channel);
    if (_inverse) sigmaDeltaWrite(_channel, 255);
    else sigmaDeltaWrite(_channel, 0);
}
#endif


// Flash function ************************************************************
void statusLED::flash(unsigned long onDuration, unsigned long offDuration, unsigned long pauseDuration, int pulses) {
    _onDuration = onDuration;
    _offDuration = offDuration;
    _pauseDuration = pauseDuration;
    _pulses = pulses;
    
    unsigned long currentMillis = millis();
    
    switch (_state) {
        case 0: //---- Step 0 (do nothing)
            _state = 1;
            break;
            
        case 1: //---- Step 1 (LED on)
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
            if (_inverse) digitalWrite(_pin1, LOW);
            else digitalWrite(_pin1, HIGH);
#else
            if (_inverse) sigmaDeltaWrite(_channel, 0);
            else sigmaDeltaWrite(_channel, 255);
#endif
            _pulseCnt ++; // Increase loop counter
            _previousMillis = currentMillis;
            _state = 2;
            break;
            
        case 2: //---- Step 2 (ON duration)
            if (currentMillis - _previousMillis >= _onDuration) {
                _state = 3;
            }
            break;
            
        case 3: //---- Step 3 (LED off)
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
            if (_inverse) digitalWrite(_pin1, HIGH);
            else digitalWrite(_pin1, LOW);
#else
            if (_inverse) sigmaDeltaWrite(_channel, 255);
            else sigmaDeltaWrite(_channel, 0);
#endif
            _previousMillis = currentMillis;
            _state = 4;
            break;
            
        case 4: //---- Step 4 (OFF duration)
            if (currentMillis - _previousMillis >= _offDuration) {
                _state = 5;
            }
            break;
            
        case 5: //---- Step 5 (Flash sequence finished?)
            if (_pulseCnt >= _pulses) {
                _pulseCnt = 0;
                _previousMillis = currentMillis;
                _state = 6; // Sequence finished
            } else {
                _state = 0;
            }
            break;
            
        case 6: //---- Step 4 (Pause duration)
            if (currentMillis - _previousMillis >= _pauseDuration) {
                _state = 0;
            }
            break;
    }
}

// On function ************************************************************
void statusLED::on() {
    _state = 0;
    _pulseCnt = 0;
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
    if (_inverse) digitalWrite(_pin1, LOW);
    else digitalWrite(_pin1, HIGH);
#else
    if (_inverse) sigmaDeltaWrite(_channel, 0);
    else sigmaDeltaWrite(_channel, 255);
#endif
}

// Off function ************************************************************
void statusLED::off() {
    _state = 0;
    _pulseCnt = 0;
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
    if (_inverse) digitalWrite(_pin1, HIGH);
    else digitalWrite(_pin1, LOW);
#else
    if (_inverse) sigmaDeltaWrite(_channel, 255);
    else sigmaDeltaWrite(_channel, 0);
#endif
}

// PWM function ************************************************************
void statusLED::pwm(int brightness) {
    _state = 0;
    _pulseCnt = 0;
    //_channel = 1;
    _brightness = brightness;
    
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
    if (_inverse) analogWrite(_pin1, 255 - _brightness);
    else analogWrite(_pin1, _brightness);
    
#else // ESP32 platform (analogWrite not supported)
    if (_inverse) sigmaDeltaWrite(_channel, 255 - _brightness);
    else sigmaDeltaWrite(_channel, _brightness);
    
#endif
}
