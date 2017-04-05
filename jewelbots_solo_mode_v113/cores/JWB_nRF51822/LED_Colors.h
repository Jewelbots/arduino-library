
#ifndef __LED_COLORS_H__
#define __LED_COLORS_H__


extern "C"{
 // __cplusplus

enum ColorLabel { OFF = 0, RED = 1, GREEN = 2, BLUE = 3, YELLOW = 4, MAGENTA = 5, CYAN = 6, WHITE = 7 };


struct Color
{
  Color( uint8_t r, uint8_t g, uint8_t b )
  {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  uint8_t r, g, b;
};

const Color COLORS[8] = {
  Color( 0x00, 0x00, 0x00 ), // Off
  Color( 0x3F, 0x00, 0x00 ), // Red
  Color( 0x00, 0x3F, 0x00 ), // Green
  Color( 0x00, 0x00, 0x3F ), // Blue
  Color( 0x3F, 0x3F, 0x00 ), // Yellow
  Color( 0x3F, 0x00, 0x3F ), // Magenta
  Color( 0x00, 0x3F, 0x3F ), // Cyan
  Color( 0x3F, 0x3F, 0x3F ) // White
};



}

#endif
