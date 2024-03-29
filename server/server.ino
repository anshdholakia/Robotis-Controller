#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "new_controll.h"

const char *ssid = "your-SSID";
const char *password = "your-PASSWORD";

ESP8266WebServer server(80); // Server on port 80

// Initializing object
Controll_robot ctr;

void setup()
{
   Serial.begin(57600);
   while (!Serial)
      ;
   ctr.init(); // Starting robot

   // Connect to Wi-Fi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
      delay(1000);
      Serial.println("Connecting to WiFi...");
   }
   Serial.println("Connected to WiFi");

   server.on("/move_joint", HTTP_POST, handleMoveJoint);
   server.on("/close_open_gripper", HTTP_POST, handleCloseOpenGripper);
   server.on("/get_joint_angles", HTTP_GET, handleGetJointAngles);
   server.begin();
}

void loop()
{
   server.handleClient();
}

void handleGetJointAngles()
{
   double joint_angles[3];
   joint_angles[0] = ctr.dxl_GetPos(1);
   joint_angles[1] = ctr.dxl_GetPos(2);
   joint_angles[2] = ctr.dxl_GetPos(3);
   // Format joint_angles as a comma-separated string
   String response = String(joint_angles[0]) + "," + String(joint_angles[1]) + "," + String(joint_angles[2]);

   // Send the response
   server.send(200, "text/plain", response);
}

void handleCloseOpenGripper()
{
   if server.hasArg("gripper_pos") && server.arg("gripper_pos") != NULL)
      {
         int gripper_pos = server.arg("gripper_pos").toInt();
         if (gripper_pos == 1)
         {
            ctr.dxl_Release();
         }
         else if (gripper_pos == 2)
         {
            ctr.dxl_Open();
         }
      }
}

void handleMoveJoint()
{
   int motor_number;
   float motor_speed, motor_angle;
   if (server.hasArg("motor_number") && server.arg("motor_number") != NULL)
   {
      String motor_number = server.arg("motor_number");
      motor_number = motor_number.toInt();
   }
   else
   {
      server.send(400, "text/plain", "Motor number is required");
      return;
   }

   if (server.hasArg("motor_speed") && server.arg("motor_speed") != NULL)
   {
      String motor_speed = server.arg("motor_speed");
      motor_speed = motor_speed.toFloat();
      ctr.dxl_SetVel(motor_number, motor_speed);
   }

   if (server.hasArg("motor_angle") && server.arg("motor_angle") != NULL)
   {
      String motor_angle = server.arg("motor_angle");
      motor_angle = motor_angle.toFloat();
      ctr.dxl_SetPos(motor_number, motor_angle);
   }

   server.send(200, "text/plain", "OK");
}
