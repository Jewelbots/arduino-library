/*
 * Copyright (c) 2017 Jewelbots
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/*
 * This example demonstrates some features of the LED, Buzzer, and Timer libraries.
 * These libraries work the same way as in the older Arduino coding "board."
 *
 * Here the code runs a few different functions during the setup of the Jewelbot.
 */

// Declare Jewelbots hardware components to use
LED led;
Buzzer buzz;
Timer timer;

void setup() {
// put your setup code here, to run once:

  // Turn on one light green
  // Start a medium length buzz
  // Pause for 1 second (1000 us)
  // Turn off the green light
  led.turn_on_single(NW, GREEN);
  buzz.medium_buzz();
  timer.pause(1000);
  led.turn_off_single(NW);

  // Turn on all lights magenta
  // Start an extra long buzz
  // Pause for 3 seconds (3000 us)
  // Turn off all lights
  led.turn_on_all(MAGENTA);
  buzz.extra_long_buzz();
  timer.pause(3000);
  led.turn_off_all();

} // setup

void loop() {
// put your main code here, to run repeatedly:


} // loop
