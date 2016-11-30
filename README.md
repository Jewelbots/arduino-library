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

1. Open the Boards Manager from the Menu: Tools -> Board "..." -> Boards Manager.
1. Search for "Jewelbots", select the "Jewelbots Arduino Library" and click "Install".
1. Wait for the library to finish installation and click "Close".
1. Go to Tools -> Board "..." dropdown and select the "JWB nRF51822" board.
1. Make sure the serial port is selected from Tools -> Port.
<<<<<<< HEAD
1. Put your Jewelbots device in coding mode by holding down the button for two seconds.
=======
1. Put your Jewelbots device in coding mode by holding down the button for two seconds. Either the charging light will go out or the device will flash purple depending on what version of the firmware you have. Coding mode lasts for two minutes.
>>>>>>> ece36cd7088d481fc0565169c98e89137bd80945
1. Test the "hello world" sketch below:

```c
#include <Arduino.h>

LED led;

void setup() {
  // Put your setup code here, to run once.
}

void loop() {
  // Put your main code here, to run repeatedly:
  led.on(1, "blue", 650000);
}
```
