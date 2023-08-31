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
   server.begin();
}

void loop()
{
   server.handleClient();
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
