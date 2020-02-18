# statusLED Arduino library

This is an Arduino library for easy LED control: dimming, flash sequences etc.

## Features:
- Switching the LED on and off
- Adjusting the brightness, using PWM (PWM capable pin required)
- Blinking the LED
- Flashing sequences (e.g. for visualization of numbers)
- Ideal for light control in RC vehicles
- Inverse mode, if your LED is wired between output pin and vcc
- Works on ATmega328P, ATmega32U4, ESP32

## Changes:
- December 2019: ESP32 support added
- December 2019: ESP32 now with 16 PWM channels instead of 8
- January 2020: the .flash() function is now a boolean and returning "true", if the sequence is in start position. Allows to synchronize the indicator sound with the LED. Used in: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32
- January 2020: experimental support for optional 16bit timer support on ESP32 added. Required for v2.0 of: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32
- February 2020: optional delay for first flash() pass added. Allows to program offsets between different LED. Required for v2.6 of: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32

## Usage

See [example](https://github.com/TheDIYGuy999/statusLED/blob/master/examples/statusLED/statusLED.ino).

(c) 2016 - 2020 TheDIYGuy999
