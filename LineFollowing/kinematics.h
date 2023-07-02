// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _KINEMATICS_H
#define _KINEMATICS_H

#include <math.h>
#define WHEEL_R  0.016
#define CAR_W    0.0485
#define PI       3.1415926

// Class to track robot position.
class Kinematics_c {
  public:
  
    // Constructor, must exist.
    Kinematics_c() {

    } 
    float XI;
    float YI;
    float THETAI;
    float XR;
    float YR;
    float THETAR;

    void initialise(){
      XI=0;
      YI=0;
      THETAI=90;
      XR=0;
      YR=0;
      THETAR=0;
    }

    void update(float distance_left, float distance_right){
      float Vxr = (distance_left + distance_right) / 2;
      float Vthetar = (distance_left - distance_right) / (2*CAR_W) * 180 / PI * 1.14;
      XI = XI + Vxr*cos(THETAI/180*PI);
      YI = YI + Vxr*sin(THETAI/180*PI);
      THETAI = THETAI - Vthetar;
      Serial.print("X: ");
      Serial.println(XI);
      Serial.print("Y: ");
      Serial.println(YI);
      Serial.print("left: ");
      Serial.println(distance_left);
      Serial.print("right: ");
      Serial.println(distance_right);
      Serial.print("THETA: ");
      Serial.println(THETAI);
    }


    // Use this function to update
    // your kinematics

};



#endif
