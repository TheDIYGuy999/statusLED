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
- v1.0: Initial commit
- v1.1: ESP32 support added
- v1.2: ESP32 now with 16 PWM channels instead of 8
- v1.3: The .flash() function is now a boolean and returning "true", if the sequence is in start position. Allows to synchronize the indicator sound with the LED. Used in: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32
- v1.4: Experimental support for optional 16bit timer support on ESP32 added. Required for v2.0 of: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32
- v1.5: Optional delay for first flash() pass added. Allows to program offsets between different LED. Required for v2.6 of: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32
- v1.6: Optional ramp (ESP32 only) for flash() and off() added. Allows to switch indicators on and off softly. Required for v5.7 of: https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32
- v1.7: Adjustable off brightness for flash()
- v1.8: Arduino 1.5 structure, so it's not detected as "legacy" library anymore

## Usage

See [example](https://github.com/TheDIYGuy999/statusLED/blob/master/examples/statusLED/statusLED.ino).

(c) 2016 - 2021 TheDIYGuy999
