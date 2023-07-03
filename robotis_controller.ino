#define MAIN "Arduino.h"
#ifndef MAIN
#include "Arduino.h"
#else
#endif
#include "new_controll.h"

// testing functions
void test_one(void);
void test_two(void);
void test_three(void);
void default_pos(void);

Controll_robot ctr;

void setup() {
  Serial.begin(57600);
  while (!Serial)
    ;
  ctr.init();
}

void loop() {
  ctr.start_loop();
  //test_one();
  test_two();
  test_three();
}

void default_pos(void){
  ctr.dxl_SetPos(1, 90);
  ctr.dxl_SetPos(2, 90);
  ctr.dxl_SetPos(3, 90);
}

void test_one(void){
  default_pos();
  ctr.dxl_SetVel(1, 10);
  ctr.dxl_SetPos(1, 100);
  ctr.dxl_SetVel(1, 5);
  ctr.dxl_SetPos(1, 140);
  ctr.dxl_SetVel(1, 10);
}


void test_two(void){
  default_pos();
  ctr.dxl_Grab();
  ctr.dxl_Release();
  ctr.dxl_Grab();
  ctr.dxl_Release();
}

void test_three(void){

  ctr.dxl_TorqueDisable(254);
  
}
