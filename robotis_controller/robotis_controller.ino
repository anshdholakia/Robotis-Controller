#define MAIN "Arduino.h"
#ifndef MAIN
#include "Arduino.h"
#else
#endif
#include "../new_controll.h"

Controll_robot ctr;
void setup() {
  Serial.begin(57600);
  while (!Serial)
    ;
  ctr.init();
}

void loop() {
  ctr.dxl_SetVel(1, 10);
  ctr.dxl_SetPos(1, 140, 1);
  ctr.dxl_SetPos(2, 30);
  ctr.dxl_SetVel(3, 1);
  ctr.dxl_SetPos(3, 50);
  ctr.dxl_SetPos(1, 90);
}
