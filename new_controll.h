#include "Arduino.h"
#include <open_manipulator_libs.h>
#include <queue>
using namespace std;

class Controll_robot {
private:
  OpenManipulator opm;
  double control_time = 0.010;  //default control frequency (100Hz)
  bool platform_state = true;
  double present_time = 0.0;
  double previous_time = 0.0;
  double multipliers[3] = { -750 / 13, 750 / 13, 3000 / 53 };
  double constants[3] = { 2355 / 26, 2355 / 26, 0.0 };
  double velocity[3] = { 5.0, 5.0, 5.0 };
  double gripper_pos;
  queue<double*> points;
  int flag = 0;

  double angleToValue(double angle, int id) {
    return (angle - constants[id - 1]) / multipliers[id - 1];
  }
  double valueToAngle(double value, int id) {
    return multipliers[id - 1] * value + constants[id - 1];
  }

  void moveJS(double j2, double j3, double j4, int id, double gripper_position, double speed, double* previous) {
    if(id==256){
      opm.disableAllActuator();
      return;
    }
    if(id==257){
      opm.enableAllActuator();
      return;
    }
    static std::vector<double> target_angle;
    double time;
    target_angle.clear();
    target_angle.push_back(0.0);
    target_angle.push_back(j2);
    target_angle.push_back(j3);
    target_angle.push_back(j4);
    if (id != 254) {
      time = fabs(valueToAngle(points.front()[id - 1], id) - valueToAngle(previous[id - 1], id)) / speed;
    } else {
      time = 1.50;
    }
    if (time == 0.00) {
      return;
    }

    opm.makeJointTrajectory(target_angle, time);
    opm.makeToolTrajectory("gripper", gripper_position);
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
    gripper_pos = opm.receiveAllToolActuatorValue().at(0).position;
    opm.enableAllActuator();
    double* position = (double*)malloc(sizeof(double) * 3);
    for (int i = 1; i < 4; i++) {
      position[i - 1] = dummy.at(i).position;
    }
    points.push(position);
  }

  void process() {
    present_time = millis() / 1000.0;
    // Trajectory following movement occurs here
    if (present_time - previous_time >= control_time) {
      opm.processOpenManipulator(millis() / 1000.0);
      previous_time = millis() / 1000.0;
    }
  }

  void addSequence(int id, double angle) {
    if ((id < 1 || id > 3) && id != 254 && id!=256 && id!=257) {
      Serial.println("Usage: dxl_SetPos(0-180, 1-3)");
      return;
    }
    process();
    // to save memory, we can make a queue and then pop elements from it
    if (flag <= 1) {
      double* position = (double*)malloc(sizeof(double) * 6);
      for (int i = 0; i < 3; i++) {
        if (i + 1 == id && id != 254 && id!=256 && id!=257) {
          position[i] = angleToValue(angle, id);
        } else {
          position[i] = points.back()[i];
        }
      }
      position[3] = id;
      position[4] = gripper_pos;
      if (id == 254 || id == 256 || id==257) {
        position[5] = 0.0;
      } else {
        position[5] = velocity[id - 1];
      }
      points.push(position);
    }
    if (opm.getMovingState()) {
      return;
    } else {
      double* previous = points.front();
      points.pop();
      moveJS(points.front()[0], points.front()[1], points.front()[2], points.front()[3], points.front()[4], points.front()[5], previous);
      free(previous);
      return;
    }
  }


public:
  void init() {
    opm.setOpenManipulatorCustomJointId(11, 12, 13, 14, 15);
    opm.initOpenManipulator(platform_state);
    Serial.println("OpenManipulator Init Begin");
    Serial.println("OpenManipulator Init Completed");
    warning();
  }


  void dxl_SetPos(int id, double angle) {
    if (id < 1 || id > 3) {
      Serial.println("Usage: dxl_SetPos(1-3, 0-180)");
      return;
    }
    process();
    addSequence(id, angle);
  }

  void start_loop() {
    flag++;
  }

  void dxl_Grab() {
    if (flag <= 1) {
      gripper_pos = -0.01;
    }
    addSequence(254, 0);
  }

  void dxl_Release() {
    if (flag <= 1) {
      gripper_pos = 0.01;
    }
    addSequence(254, 0);
  }

  void dxl_SetVel(int id, double velo) {
    if (velo < 1 || velo > 10 || id < 1 || id > 3) {
      Serial.println("Usage: dxl_SetVel(1-3,1-10)");
      return;
    }
    velocity[id - 1] = (double)velo;
  }



  void dxl_TorqueEnable(int id) {
    if (id == 254) {
      addSequence(257, 0);
    }
  }

  void dxl_TorqueDisable(int id) {
    if (id == 254) {
      addSequence(256, 0);
    }
  }

  double dxl_GetPos(int id) {
    if (id < 1 || id > 3) {
      Serial.println("Usage: dxl_GetPos(1-3)");
      return -1;
    }
    return valueToAngle(opm.receiveAllJointActuatorValue()[id].position, id);
  }
};