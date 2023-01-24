#include <vector>
#include <open_manipulator_libs.h>
#include <math.h>

class Controll_robot
{
  private:
      OpenManipulator opm;
      bool platform_state = true;
      double present_time=0.0, previous_time=0.0, control_time=0.010;
      double multipliers[3] = {-220/3.72,220/3.62,220/4.02};
      double constants[3] = {(220*1.63)/3.72,86.40,11.24};
      double velocity[3] = {5.0, 5.0, 5.0};
      double angleToValue(double angle, int id){
        return (angle-constants[id-1])/multipliers[id-1];
      }
      double valueToAngle(double value, int id){
        return multipliers[id-1]*value + constants[id-1];
      }
      std::vector<double> position;
      double gripper;
      
      // Output warning before starting motion
      void warning()
      {
        Serial.print("Starting in 3 ...");
        delay(500);
        Serial.print(".. 2 ..");
        delay(500);
        Serial.println(".. 1");
        opm.disableAllActuator();
        delay_ms(1000);
      }


      void moveJS(std::vector<double> pos, double time){
        opm.enableAllActuator();
        present_time = millis()/1000.0;
        // Trajectory following movement occurs here
        if(present_time-previous_time >= control_time)
        {
          opm.processOpenManipulator(millis()/1000.0);
          previous_time = millis()/1000.0;
        }
        opm.makeJointTrajectory(pos, time);
        opm.makeToolTrajectory("gripper", gripper);
      }

      double presentGripperPosition(){
        return opm.receiveToolActuatorValue("gripper").position;
      }

      std::vector<double> presentActuatorPosition(){
        std::vector<JointValue> dummy = opm.receiveAllJointActuatorValue();
        std::vector<double> returnval;
        returnval.push_back(0.0);
        for(int i = 1; i<4;i++){
          returnval.push_back(dummy.at(i).position);
        }
        return returnval;
      }

      void init(){
        opm.setOpenManipulatorCustomJointId(11, 12, 13, 14, 15); // ID 11, 12, 13, 14, 15 is default
        opm.initOpenManipulator(platform_state);
        warning();        
      }

      void setValues(){
        position.clear();
        position.push_back(0.0);
        for(int i = 1; i<presentActuatorPosition().size(); i++){
          position.push_back(angleToValue(presentActuatorPosition().at(i), i));
        }
        gripper=0.01;
      }

      double getTime(int id, double angle){
        return (double) abs(valueToAngle(presentActuatorPosition().at(id), id) - angle)/velocity[id-1];
      }


  public:

      void start(){
        init();
        setValues();
      }

      void dxl_SetVel(int id, int vel){ // velocity from 1 to 10
        if(id<1 || id>3){
          return;
        }
        if(vel<1 || vel>10){
          Serial.println("Enter velocity between 1 and 10\n");
          vel==1;
        }
        Serial.printf("Setting velocity of actuator %d to %d deg/s\n", id, vel);
        velocity[id-1] = vel;
      }




      // make horizontal in the first one
      void dxl_SetPos(int id, double angle){
        if (opm.getMovingState()) // returns if robot still in moving state
        {
          return;
        }
        if(id<1||id>3){
          return;
        }
        std::vector<double> present_position;
        present_position = presentActuatorPosition();
        Serial.printf("Setting position of actuator %d to ", id); 
        Serial.print(angle);
        Serial.print(" deg in time ");
        Serial.println(getTime(id, angle));
        position.at(id) = angleToValue(angle, id);
        moveJS(position, getTime(id, angle));
      }

      double dxl_GetPos(int id){
          if(id<1 || id>3){
              return -1;
          }
          return valueToAngle(presentActuatorPosition().at(id), id);
      }
};
