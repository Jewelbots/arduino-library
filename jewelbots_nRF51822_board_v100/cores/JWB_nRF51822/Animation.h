
#ifndef __LED_H__
#define __LED_H__



extern "C"{

#include <stdbool.h>
#include <stdint.h>

class Animation {
  public:
    Animation();
    ~Animation();
    extern void breathe_all(uint8_t color);
    void jewelbots_logo(void);
    void rainbows(void);
 };
} // extern "C"

#endif
