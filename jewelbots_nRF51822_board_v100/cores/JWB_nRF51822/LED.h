
#ifndef __LED_H__
#define __LED_H__



extern "C"{

#include <stdbool.h>
#include <stdint.h>
#include "LED_Colors.h"

enum LED_Pos { SE = 3, SW = 0, NW = 1, NE = 2 };

class LED {
public:
  LED();
  ~LED();
  void on(uint8_t number, char *color, uint32_t length);
  void color_lookup(char *color);
  void turnOn(LED_Pos led, ColorLabel color );
  void turnOff(LED_Pos led);
  void flash(LED_Pos led, ColorLabel color, uint8_t microseconds);

private:
  void setLight(uint8_t number, uint8_t r, uint8_t g, uint8_t b);

};

} // extern "C"

#endif
