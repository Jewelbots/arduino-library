#include "JWB_API.h"

void setup() {
  // put your setup code here, to run once:
  breathe_all(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  Animation animation;
  animation.jewelbots_logo();
}

void button_press() {
  // put code here to run whenever the Magic Button is pressed:
  Animation animation;
  animation.breathe_single_color(BLUE);
}
