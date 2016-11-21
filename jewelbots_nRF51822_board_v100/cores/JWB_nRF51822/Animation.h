
#ifndef __LED_H__
#define __LED_H__



extern "C"{

#include <stdbool.h>
#include <stdint.h>

class Animate {
  public:
    extern void breathe_all(uint8_t color);
    void jewelbots_logo(void);
    void rainbows(void);
 };
} // extern "C"

#endif
