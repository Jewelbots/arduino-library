
#ifndef __LED_H__
#define __LED_H__



extern "C"{

#include <stdbool.h>
#include <stdint.h>

class LED {
public:
  LED();
  ~LED();
  void on(uint8_t number, char *color, uint8_t length);
  void color_lookup(char *color);

};

} // extern "C"

#endif
