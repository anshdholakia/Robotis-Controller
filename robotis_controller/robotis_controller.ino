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

double **dxl_ForwardKinematics(double angle1, double angle2, double angle3)
{
   /*
     This function returns the end effector position given the angles of the respective joints
     Input: Position of end effector
     Output: Angles of joints
          l2
        ______
     l1/      \l3
      /     _ /\
      |    / |  |
      |  _/
     _|_/l4

     The joint between base and l1 is angle1
     The joint between l1 and l2 is angle2
     The joint between l2 and l3 is angle3
     */
   double theta1 = (angle1 * 71) / 4068, theta2 = ((180 - angle2) * 71) / 4068, theta3 = ((180 - angle3) * 71) / 4068;
   double a2 = 1.0, alpha2 = 0.0, d2 = 15.0;
   double a3 = 1.0, alpha3 = 0.0, d3 = 14.0;
   double a4 = 1.0, alpha4 = 0.0, d4 = 12.5;
   double **T2 = getTransformationMatrix(a2, alpha2, d2, theta1);
   double **T3 = getTransformationMatrix(a3, alpha3, d3, theta2);
   double **T4 = getTransformationMatrix(a4, alpha4, d4, theta3);
   double **T = matrixMultiply(T2, T3);
   T = matrixMultiply(T, T4);
   return T;
}

double **getTransformationMatrix(double a, double alpha, double d, double theta)
{
   double **T = 0;
   T = new double *[4];
   for (int i = 0; i < 4; i++)
   {
      T[i] = new double[4];
   }
   T[0][0] = cos(theta);
   T[0][1] = -sin(theta) * cos(alpha);
   T[0][2] = sin(theta) * sin(alpha);
   T[0][3] = a * cos(theta);

   T[1][0] = sin(theta);
   T[1][1] = cos(theta) * cos(alpha);
   T[1][2] = -cos(theta) * sin(alpha);
   T[1][3] = a * sin(theta);

   T[2][0] = 0;
   T[2][1] = sin(alpha);
   T[2][2] = cos(alpha);
   T[2][3] = d;

   T[3][0] = 0;
   T[3][1] = 0;
   T[3][2] = 0;
   T[3][3] = 1;

   return T;
}

double **matrixMultiply(double **A, double **B)
{
   double **C = 0;
   C = new double *[4];

   for (int i = 0; i < 4; i++)
   {
      C[i] = new double[4];
   }

   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         C[i][j] = 0;
         for (int k = 0; k < 4; k++)
         {
            C[i][j] += A[i][k] * B[k][j];
         }
      }
   }

   return C;
}

void setup()
{
   Serial.begin(57600);
   while (!Serial)
      ;
   ctr.init();
   // dxl_ForwardKinematics(-17, -17, -3) -> (x, y, z) -> (41 cm, 0, 0)
   double **result = dxl_ForwardKinematics(74, 96, 68);
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         Serial.print(result[i][j]);
         Serial.print(" ");
      }
      Serial.println("");
   }
}

void loop()
{
}

void default_pos(void)
{
   ctr.dxl_SetPos(1, 90);
   ctr.dxl_SetPos(2, 90);
   ctr.dxl_SetPos(3, 90);
}

void test_one(void)
{
   default_pos();
   ctr.dxl_SetVel(1, 10);
   ctr.dxl_SetPos(1, 100);
   ctr.dxl_SetVel(1, 5);
   ctr.dxl_SetPos(1, 140);
   ctr.dxl_SetVel(1, 10);
}

void test_two(void)
{
   default_pos();
   ctr.dxl_Grab();
   ctr.dxl_Release();
   ctr.dxl_Grab();
   ctr.dxl_Release();
}

void test_three(void)
{

   ctr.dxl_TorqueDisable(254);
}
