/*
 statusLED.cpp - Library for status LED control (blinking, flash sequences, on, off, pwm)
 Created by TheDIYGuy999 May 2016 - Dez. 2019
 Released into the public domain.
 
 Dez. 2019: added ESP32 support
 Dez. 2019: replaced sigmaDelta function with ledc funvtion, so we can use more 16 instead of 8 timers
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
void statusLED::begin(int pin1, int channel, int frequency, int resolution) {
    _pin1 = pin1;
    _channel = channel;
    _frequency = frequency;
    ledcSetup(_channel, _frequency, resolution); // 8 bit resolution, if no input argument
    ledcAttachPin(_pin1, _channel);
    if (_inverse) ledcWrite(_channel, 255);
    else ledcWrite(_channel, 0);
}
#endif


// Flash function ************************************************************
bool statusLED::flash(unsigned long onDuration, unsigned long offDuration, unsigned long pauseDuration, int pulses, int delay, int bulbSimRamp, int flashOffBrightness) {
    _onDuration = onDuration;
    _offDuration = offDuration;
    _pauseDuration = pauseDuration;
    _bulbSimRamp = bulbSimRamp;
    _pulses = pulses;
    _delay = delay;
    _flashBrightness;
    _flashOffBrightness = flashOffBrightness;
    
    unsigned long currentMillis = millis();
    
// State machine
    switch (_state) {
        case 0: //---- Step 0 (do nothing)
            _previousMillis = currentMillis;
            _state = 1;
            break;
            
        case 1: //---- Step 1 (Start delay for first pass)
            if (currentMillis - _previousMillis >= _delay) {
                _state = 2;
            }
            break;
            
        case 2: //---- Step 2 (LED on)
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
            if (_inverse) digitalWrite(_pin1, LOW);
            else digitalWrite(_pin1, HIGH);
#else
            //if (_inverse) ledcWrite(_channel, 0);
            //else ledcWrite(_channel, 255);
            _up = true;
            _down = false;
#endif
            _pulseCnt ++; // Increase loop counter
            _previousMillis = currentMillis;
            _state = 3;
            _start = true;
            break;
            
        case 3: //---- Step 3 (ON duration)
            if (currentMillis - _previousMillis >= _onDuration) {
                _state = 4;
                _start = false;
            }
            break;
            
        case 4: //---- Step 4 (LED off)
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
            if (_inverse) digitalWrite(_pin1, HIGH);
            else digitalWrite(_pin1, LOW);
#else
            //if (_inverse) ledcWrite(_channel, 255);
            //else ledcWrite(_channel, 0);
            _down = true;
            _up = false;
#endif
            _previousMillis = currentMillis;
            _state = 5;
            break;
            
        case 5: //---- Step 5 (OFF duration)
            if (currentMillis - _previousMillis >= _offDuration) {
                _state = 6;
            }
            break;
            
        case 6: //---- Step 6 (Flash sequence finished?)
            if (_pulseCnt >= _pulses) {
                _pulseCnt = 0;
                _previousMillis = currentMillis;
                _state = 7; // Sequence finished
            } else {
                _state = 2;
            }
            break;
            
        case 7: //---- Step 7 (Pause duration)
            if (currentMillis - _previousMillis >= _pauseDuration) {
                _state = 2;
            }
            break;
    } // End of state machine
    
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
    // No actions
#else
    if (bulbSimRamp > 0) {
        // Ramp brightness
        if (micros() - _previousFlashRampMillis >= _bulbSimRamp) {
            _previousFlashRampMillis = micros();
            if (_up && _flashBrightness < 255) {
                _flashBrightness ++;
            }
            if (_flashBrightness == 255 || _down) _up = false;
            
            if (_down && _flashBrightness > _flashOffBrightness) {
                _flashBrightness --;
            }
            if (_flashBrightness == _flashOffBrightness) _down = false;
        }
    }
    else {
        // Change brightness immediately
        if (_up) {
            _flashBrightness = 255;
            _up = false;
        }
        if (_down) {
            _flashBrightness = _flashOffBrightness;
            _down = false;
        }
    }
    
    // Write brightness
    if (_inverse) ledcWrite(_channel, 255 - _flashBrightness);
    else ledcWrite(_channel, _flashBrightness);
#endif
    
    return _start; // Report back, if we are back @ step 0 (added 2020 01 03)
}

// On function ************************************************************
void statusLED::on() {
    _state = 0;
    _pulseCnt = 0;
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
    if (_inverse) digitalWrite(_pin1, LOW);
    else digitalWrite(_pin1, HIGH);
#else
    if (_inverse) ledcWrite(_channel, 0);
    else ledcWrite(_channel, 255);
#endif
}

// Off function ************************************************************
void statusLED::off(int bulbSimRamp, int _offOffBrightness) {
    _state = 0;
    _pulseCnt = 0;
    _offBulbSimRamp = bulbSimRamp;
    _offOffBrightness = _offOffBrightness;
#if defined __AVR_ATmega32U4__ || defined __AVR_ATmega328P__
    if (_inverse) digitalWrite(_pin1, HIGH);
    else digitalWrite(_pin1, LOW);
#else
    //if (_inverse) ledcWrite(_channel, 255);
    //else ledcWrite(_channel, 0);
    if (_offBulbSimRamp > 0) {
        // Ramp brightness
        if (micros() - _previousOffRampMillis >= _offBulbSimRamp) {
            _previousOffRampMillis = micros();
            if (_offBrightness > _offOffBrightness) _offBrightness --;
        }
    }
    else _offBrightness = _offOffBrightness; // Change brightness immediately
    
    // Write brightness
    if (_inverse) ledcWrite(_channel, 255 - _offBrightness);
    else ledcWrite(_channel, _offBrightness);
    
    
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
    if (_inverse) ledcWrite(_channel, 255 - _brightness);
    else ledcWrite(_channel, _brightness);
    
#endif
}
