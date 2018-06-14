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
 * This example demonstrates how to use Serial output over the USB to print
 * statements to the Arduino Serial Monitor.
 */

// Declare Jewelbots hardware components and variables
uint32_t x = 0;
Timer Timer;
char str[50] = "End of loop()";

void setup() {
  // put your setup code here, to run once:

  // Function to tell the Jewelbot to run the loop() function
  // while plugged into USB power
  set_run_loop_charging();

  // Output a simple string letting you know the code got into the setup() section
  JWB_SERIAL("In Setup\n");
}



void loop() {
  // put your main code here, to run repeatedly:

  // Formatted output
  // Standard C printf commands are valid here (no float on the Jewelbots chip)
  // This first statement outputs the current value of x as an unsigned integer value
  JWB_SERIAL_PRINTF("Integer: x = %u\n", x);
  // This statement outputs the value of x as a hexidecimal number
  JWB_SERIAL_PRINTF("Hex: x = 0x%x\n", x);
  x++;

  Timer.pause(2000);

  // This statement demonstrates outputting a string value (characters)
  JWB_SERIAL_PRINTF("%s\n", str);
}
