
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
    void turn_on_single(LED_Pos led, ColorLabel color );
    void turn_on_all(ColorLabel color );
    void turn_off_single(LED_Pos led);
    void turn_off_all();
    void flash_single(LED_Pos led, ColorLabel color, uint32_t milliseconds);
    void flash_all(ColorLabel color, uint32_t milliseconds);
    void setLight(uint8_t number, uint8_t r, uint8_t g, uint8_t b);

};

} // extern "C"

#endif
