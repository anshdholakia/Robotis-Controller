#include <iostream>
using namespace std;
#include <math.h>

void forwardKinematics(double theta1, double theta2, double theta3) {
  // Convert input angles from degrees to radians
  double theta1_rad = ((90+theta1) * 3.14) / 180;
  double theta2_rad = ((180-theta2) * 3.14) / 180;
  double theta3_rad = ((180-theta3) * 3.14) / 180;

  // Define link lengths
  double l1 = 15.0;
  double l2 = 13.0;
  double l3 = 12.5;

  // Calculate coordinates
  double x = l1 * cos(theta1_rad) + l2 * cos(theta1_rad + theta2_rad) + l3 * cos(theta1_rad + theta2_rad + theta3_rad);
  double y = l1 * sin(-theta1_rad) + l2 * sin(-theta1_rad - theta2_rad) + l3 * sin(-theta1_rad - theta2_rad - theta3_rad);

  // Output results
  double* result = new double[2];
  result[0] = x;
  result[1] = y;
  cout << "X: " << x << endl;
  cout << "Y: " << y << endl;
}

int main()
{
  forwardKinematics(-17, -17, -3);
  return 0;
}
