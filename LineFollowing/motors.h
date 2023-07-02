// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _MOTORS_H
#define _MOTORS_H

#define left false
#define right true

# define L_PWM_PIN 10
# define L_DIR_PIN 16
# define R_PWM_PIN 9
# define R_DIR_PIN 15

# define FWD LOW
# define REV HIGH


// Class to operate the motor(s).
class Motors_c {
  public:
    // Constructor, must exist.
    Motors_c() {} 

    // Use this function to 
    // initialise the pins and 
    // state of your motor(s).
    void initialise() {
      pinMode(L_PWM_PIN, OUTPUT);
      pinMode(L_DIR_PIN, OUTPUT);
      pinMode(R_PWM_PIN, OUTPUT);
      pinMode(R_DIR_PIN, OUTPUT);
    }

    // Write a function to operate
    // your motor(s)
    void setMotorPower( float left_pwm, float right_pwm ) {
      float max_pwm = 100;
      float min_pwm = -100;

      if(left_pwm>max_pwm){
        Serial.println("WARNING: left_pwm exceeding limit");
        setMotorPower(max_pwm, right_pwm);
      }else if(right_pwm>max_pwm){
        Serial.println("WARNING: right_pwm exceeding limit");
        setMotorPower(left_pwm, max_pwm);
      }else if(left_pwm<min_pwm){
        Serial.println("WARNING: left_pwm exceeding reverse limit");
        setMotorPower(min_pwm, right_pwm);
      }else if(right_pwm<min_pwm){
        Serial.println("WARNING: right_pwm exceeding reverse limit");
        setMotorPower(left_pwm, min_pwm);
      }else{
        if(left_pwm < 0){
          digitalWrite(L_DIR_PIN, REV);
          analogWrite(L_PWM_PIN, -left_pwm);
        }else{
          digitalWrite(L_DIR_PIN, FWD);
          analogWrite(L_PWM_PIN, left_pwm);
        }

        if(right_pwm < 0){
          digitalWrite(R_DIR_PIN, REV);
          analogWrite(R_PWM_PIN, -right_pwm);
        }else{
          digitalWrite(R_DIR_PIN, FWD);
          analogWrite(R_PWM_PIN, right_pwm);
        }
      }
    }

    // void test(){
    //   float start_pwm = 0;
    //   while(start_pwm<110){
    //     setMotorPower(start_pwm,start_pwm);
    //     start_pwm += 1;
    //     delay(100);
    //   }
    //   while(start_pwm>-110){
    //     setMotorPower(start_pwm,start_pwm);
    //     start_pwm -= 1;
    //     delay(100);
    //   }

    //   setMotorPower(0,0);
    //   Serial.println("test finish");
    // }
  };



#endif
