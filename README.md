# Jewelbots Library for Arduino IDE Development


This library is designed to make coding the Jewelbots for Arduino easier for users. Installation instructions are below. If you would like to contribute, pull requests are welcome. Documentation is here.

# Requirements


1. A Jewelbot device!
1. PC with one of the following OS:
  - Mac OSX 10.11.4 (current tested version), OSX 10.9.x, 10.10.x should also work.
  - Windows 10 (current tested version), Windows 7 and 8.x should also work.
1. Arduino IDE version 1.6.9 (current tested version).
1. A [Micro-B USB](https://upload.wikimedia.org/wikipedia/commons/d/db/MicroB_USB_Plug.jpg) cable.

# Getting Started


## Install Board Package Add-on

1. Get [Arduino IDE from Arduino website](http://arduino.cc/en/Main/Software) and install it to your PC.
1. Start the IDE and from the Menu, click "Preference...", add the following line to "Additional Boards Manager URLs"
  ```
	https://jewelbots.github.io/arduino-library/package_jewelbots_index.json  
  ```

1. Install the "Jewelbots Development Board" add-on via Boards Manager from the Menu: Tools -> Board -> Boards Manager ...
1. Select the "JWB nRF51822" board from Tools -> Board dropdown.
1. Test the "hello world" sketch below:

```c
#include <Arduino.h>
void setup() {
  // put your setup code here, to run once:

}


void loop() {
  // put your main code here, to run repeatedly:
LED led;
led.on(1, "blue", 650000);
}
```
