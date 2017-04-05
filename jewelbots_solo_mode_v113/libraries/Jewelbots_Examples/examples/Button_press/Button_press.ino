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
 * This example demonstrates how to include a function that runs when the button is pressed for a short time.
 * This button functionality replaces the battery check function that displays a short, single red light flash when the button is pressed.
 * This button function is only available when there are no friend group colors to display.
 * (When you are not around any friends, or your friends have turned their Jewelbots off by holding the Magic Button for 5 seconds. )
 */

// Declare Jewelbots hardware components to use
Animation animation;

// This function is run when the button is pressed for a short duration (Jewelbot unplugged)
void button_press() {
  animation.jewelbots_logo();
}

// This function is run when the button is pressed for a long duration (Jewelbot unplugged)
void button_press_long() {
  animation.rainbows();
}

// This function is run when the button is pressed for a short duration while the Jewelbot is plugged into power
void charging_button_press() {
  animation.breathe_single_color(BLUE);
}

void setup() {
  // put your setup code here, to run once:


} // setup

void loop() {
  // put your main code here, to run repeatedly:


} // loop
