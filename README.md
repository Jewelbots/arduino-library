# Jewelbots Library for Arduino IDE Development
===

This library is designed to make coding the Jewelbots for Arduino easier for users. Installation instructions are below. If you would like to contribute, pull requests are welcome. Documentation is here.

# Requirements
===

1. A Jewelbot device!

2. PC with one of the following OS:
    - Mac OSX 10.11.4 (current tested version), OSX 10.9.x, 10.10.x should also work.
    - Windows 10 (current tested version), Windows 7 and 8.x should also work.
    -
3. Arduino IDE version 1.6.9 (current tested version).

4. A USB Cable

# Getting Started
===

## Install Board Package Add-on

1. Get Arduino IDE from Arduino website and install it to your PC

	http://arduino.cc/en/Main/Software

2. Start the IDE and from the Menu, click "Preference...", add the following line to "Additional Boards Manager URLs"

	https://jewelbots.github.io/arduino-library/package_jewelbots_index.json  

3. Install the "Jewelbots Development Board" add-on via Boards Manager from the Menu: Tools -> Board -> Boards Manager ...

4. Select the "JWB nRF51822" board from Tools -> Board dropdown.

4. Test the "hello world" sketch below:

''''
#include <Arduino.h>
void setup() {
  // put your setup code here, to run once:

}


void loop() {
  // put your main code here, to run repeatedly:
LED led;
led.on(1, "blue", 650000);
}
````
