#include "Arduino.h"
#include <open_manipulator_libs.h>

class Controll_robot {
private:
  OpenManipulator opm;
  double control_time = 0.010;  //default control frequency (100Hz)
  double present_time = 0.0;
  double previous_time = 0.0;
  bool platform_state = true;
  double multipliers[3] = { -750/13, 750/13, 3000/53 };
  double constants[3] = { 2355/26, 2355/26, 0.0 };
  double velocity[3] = {5.0, 5.0, 5.0};
  double gripper_pos = 0.0;
  uint8_t motion_cnt[1] = { 0 };
  std::vector<double*> points;
  int flag = 0;

  double angleToValue(double angle, int id) {
    return (angle - constants[id - 1]) / multipliers[id - 1];
  }
  double valueToAngle(double value, int id) {
    return multipliers[id - 1] * value + constants[id - 1];
  }

  void moveJS(double j2, double j3, double j4, int id) {
    static std::vector <double> target_angle;
    double time;
    target_angle.clear();
    target_angle.push_back(0.0);
    target_angle.push_back(j2);
    target_angle.push_back(j3);
    target_angle.push_back(j4);
    time = fabs(valueToAngle(points.at(motion_cnt[0])[id-1], id) - valueToAngle(points.at(motion_cnt[0]-1)[id-1], id))/velocity[id-1];
    Serial.println(time);
    opm.makeJointTrajectory(target_angle, time);
    opm.makeToolTrajectory("gripper", gripper_pos);
  }


  // Output warning before starting motion
  void warning() {
    Serial.print("Locking Open Manipulator in 3 ...");
    delay_ms(1000);
    Serial.print("2...");
    delay_ms(1000);
    Serial.println("1");
    delay_ms(1000);
    std::vector<JointValue> dummy = opm.receiveAllJointActuatorValue();
    opm.receiveAllToolActuatorValue();
    opm.enableAllActuator();
    double* position = (double*)malloc(sizeof(double)*3);
    for (int i = 1; i < 4; i++) {
      position[i-1]=dummy.at(i).position;
    }
    points.push_back(position);
  }

  void process() {
    present_time = millis() / 1000.0;
    // Trajectory following movement occurs here
    if (present_time - previous_time >= control_time) {
      opm.processOpenManipulator(millis() / 1000.0);
      previous_time = millis() / 1000.0;
    }
  }


public:
  void init() {
    motion_cnt[0] = 1;
    opm.setOpenManipulatorCustomJointId(11, 12, 13, 14, 15);
    opm.initOpenManipulator(platform_state);
    Serial.println("OpenManipulator Init Begin");
    Serial.println("OpenManipulator Init Completed");
    pinMode(BDPIN_PUSH_SW_1, INPUT);
    pinMode(BDPIN_PUSH_SW_2, INPUT);
    pinMode(BDPIN_GPIO_1, OUTPUT);
    digitalWrite(BDPIN_GPIO_1, LOW);
    warning();
  }


  void dxl_SetPos(int id, double angle, int start = 0) {
    if (id < 1 || id > 3) {
      Serial.println("Usage: dxl_SetPos(0-180, 1-3, 0/1)");
      return;
    }
    process();
    if (start == 1) {
      flag++;
    }
    // to save memory, we can make a queue and then pop elements from it motion_cnt[0] times and set motion_cnt[0] to 0
    if(flag<=1){
      double* position = (double*)malloc(sizeof(double)*4);
      for (int i = 0; i < 3; i++) {
        process();
        if (i + 1 == id) {
          position[i]=angleToValue(angle, id);
        } else {
          position[i] = points.back()[i];
        }
      }
      position[3] = id;
      points.push_back(position);
    }
    if(opm.getMovingState()){
      return;
    }
    else {
      moveJS(points.at(motion_cnt[0])[0], points.at(motion_cnt[0])[1], points.at(motion_cnt[0])[2], points.at(motion_cnt[0])[3]);
      motion_cnt[0]++;
      return;
      
    }
  }

  void dxl_SetVel(int id, double velo){
    if(velo<1 || velo>10 || id<1 || id>3){
      Serial.println("Usage: dxl_SetVel(1-3,1-10)");
      return;
    }
    velocity[id-1]=(double)velo;
  }

  

  void dxl_TorqueEnable(int id) {
    if (id == 254) {
      opm.enableAllActuator();
    }
  } 

  void dxl_TorqueDisable(int id) {
    if (id == 254) {
      opm.disableAllActuator();
    }
  }

  double dxl_GetPos(int id) {
    if (id < 1 || id > 3) {
      Serial.println("Usage: dxl_GetPos(1-3)");
      return -1;
    }
    return valueToAngle(opm.receiveAllJointActuatorValue()[id].position,id);
  }
};
