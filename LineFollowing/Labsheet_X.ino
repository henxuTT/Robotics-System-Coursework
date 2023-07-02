#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"
#include "pid.h"

# define LINE_SENSOR_UPDATE 50
# define MOTOR_UPDATE       2000

# define LED_PIN 13

# define STATE_INITIAL                0
# define STATE_DRIVE_FORWARDS         1
# define STATE_FOUND_LINE             2
# define STATE_TURN_AROUND            3
# define STATE_RETURN                 4
# define STATE_HEADBACK               5

const float turn_pwm_max = 300;
const float forward_pwm = 30;
int state;
int reach_line_count;
float return_angle = 0;
float head_angle;

Motors_c motors;
LineSensor_c linesensors;
ENCODERS_c encoders;
PID_c PID_heading;
Kinematics_c kinematics;

// put your setup code here, to run once:
void setup() {

  pinMode(LED_PIN, OUTPUT);

  setupEncoder0();
  setupEncoder1();
  
  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");

  state = STATE_INITIAL;
  reach_line_count = 0;

  motors.initialise();
  PID_heading.initialise(1, 0, 0);

  kinematics.initialise();
}


// put your main code here, to run repeatedly:
void loop() {
  switch(state){
    case STATE_INITIAL: 
      calibration();
      break;
    case STATE_DRIVE_FORWARDS: 
      driveForwards();
      break;
    case STATE_FOUND_LINE: 
      linefollow();
      break;
    case STATE_TURN_AROUND: 
      turn_around();
      break;
    case STATE_RETURN: 
      return_base_turn();
      break;
    case STATE_HEADBACK: 
      head_back();
      break;
    default: 
      Serial.println("System Error, Unknown state ");
      stopRobot();
      break;
   }     
   Serial.println(state);

  // encoders.record_start();
  // current_ts = millis();
  
  // //check line_sensor
  // elapsed_t = current_ts - ls_ts;
  // if( elapsed_t > LINE_SENSOR_UPDATE ) {
  //   bool success = linesensors.doParallelSensorRead();
  //   if(success){
  //     // Serial.println("READIND SUCCEEDED");
  //     // Serial.print( "elapsed_time: " );
  //     // for(int i=0 ; i<NB_LS_PINS; i++ ){
  //     //   Serial.print( linesensors.sensor_read[i] );
  //     //   Serial.print(" ");
  //     }
  //     // Serial.println("");
  //     ls_ts = millis();

  // //     // //LED Check
  // //     // bool led_status = LOW;
  // //     // for(int i=0 ; i<NB_LS_PINS; i++ ){
  // //     //   if(linesensors.sensor_read[i]>1500){
  // //     //     led_status = HIGH;
  // //     //   }
  // //     // }
  // //     // digitalWrite(LED_PIN, led_status);
    
  //   float e_line;
  //   e_line = getLineError(linesensors.sensor_read[0], linesensors.sensor_read[1], linesensors.sensor_read[2]); 
  //   Serial.print("e_line: ");
  //   Serial.println(e_line);

  //   // float turn_pwm;
  //   // turn_pwm = turn_pwm_max * e_line;
  //   // motors.setMotorPower(-turn_pwm + forward_pwm, turn_pwm + forward_pwm);
  //   float time_2 = millis();
  //   PID_heading.update(0, eline,time_2);
    
  //   }else {
  //     Serial.println("READING FAILED");
  //     ls_ts = millis();
  //   }

  // }

  // // //check motor
  // // elapsed_t = current_ts - motor_ts;
  // // if( elapsed_t > MOTOR_UPDATE ) {
  // //     // Toggle motor direction
  // //     // ...

  // //     // Write motor direction and
  // //     // pwm to motors.
  // //     // ...

  // //     // Record when this execution happened
  // //     // for future iterations of loop()
  // //     motor_ts = millis();
  // // }

  // delay(500);
  // encoders.record_end();
  // encoders.calculate_v();

  // unsigned long pid_end_time = micros();
  // float dtime = float(pid_end_time-pid_start_time)/1000;
  // pid_start_time = pid_end_time;

  // float left_term = PID_wheel_l.update(0.15, encoders.v_el, dtime);
  // float right_term = PID_wheel_r.update(0.15, encoders.v_er, dtime);

  // motors.setMotorPower(left_term + forward_pwm, right_term + forward_pwm);
  
  // kinematics.update(encoders.distance_el, encoders.distance_er);
  // Serial.print("X: ");
  // Serial.println(kinematics.XI);
  // Serial.print("Y: ");
  // Serial.println(kinematics.YI);
  // Serial.print("THETA: ");
  // Serial.println(kinematics.THETAI);
}

void updateState() {
}

void calibration(){
  intialisingBeeps();
  state = STATE_DRIVE_FORWARDS;
  head_angle = kinematics.THETAI;
  motors.setMotorPower(forward_pwm, forward_pwm);
  encoders.record_start();
}

// write this function to have your
// robot beep 5 times, across a total
// of 5 seconds.
void intialisingBeeps() {
  for (int i = 0; i < 5; i++) {
    // tone(8, 262, 500);
    // noTone(8);
    // delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
}

// Write code that will command your
// robot to drive forwards straightline_detect
// until it detects the course line.
void driveForwards() {
  bool reach_line = linesensors.line_detect();
  if(reach_line){
    reach_line_count += 1;
    if(reach_line_count == 1){
      motors.setMotorPower(forward_pwm-20, forward_pwm+20);
      delay(100);
    }
    state = STATE_FOUND_LINE;
    encoders.record_end();
    kinematics.update(encoders.distance_el, encoders.distance_er);
  }else{
    encoders.record_end();
    kinematics.update(encoders.distance_el, encoders.distance_er);
    // Serial.print("left: ");
    // Serial.println(encoders.distance_el);
    // Serial.print("right: ");
    // Serial.println(encoders.distance_er);
    float term = PID_heading.update(head_angle, kinematics.THETAI, encoders.dtime);
    motors.setMotorPower(forward_pwm - term, forward_pwm + term);
  }
}

void linefollow() {
  bool success = linesensors.doParallelSensorRead();
  if(success){
    bool out_line = linesensors.out_line_test();
    if(out_line){
      if(kinematics.THETAI > 170 || kinematics.THETAI < 10){
        if(reach_line_count != 2){
          state = STATE_TURN_AROUND;
          motors.setMotorPower(-forward_pwm, -forward_pwm);
          stopRobot();
          delay(2000);
          encoders.record_end();
          kinematics.update(encoders.distance_el, encoders.distance_er);
        }else{
          state = STATE_RETURN;
          encoders.record_end();
          kinematics.update(encoders.distance_el, encoders.distance_er);
          motors.setMotorPower(forward_pwm, forward_pwm);
          delay(1000);
          motors.setMotorPower(-forward_pwm, -forward_pwm);
          stopRobot();
          encoders.record_end();
          kinematics.update(encoders.distance_el, encoders.distance_er);
          delay(2000);
        }
      }else{
        state = STATE_DRIVE_FORWARDS;
        head_angle = kinematics.THETAI;
        encoders.record_end();
        kinematics.update(encoders.distance_el, encoders.distance_er);
      }
    }else{
      float e_line = linesensors.getLineError(); 
      // Serial.print("e_line: ");
      // Serial.println(e_line);
      float turn_pwm = turn_pwm_max * e_line;
      motors.setMotorPower(-turn_pwm + forward_pwm, turn_pwm + forward_pwm);
      encoders.record_end();
      kinematics.update(encoders.distance_el, encoders.distance_er);
    }
  }
}

void turn_around(){
  turn_angle(180);
  state = STATE_FOUND_LINE;
}

void turn_angle(float angle_to_turn){
  float goal_angle = kinematics.THETAI - angle_to_turn;
  float error_angle = 180;
  while(error_angle > 5){
    motors.setMotorPower(forward_pwm, -forward_pwm);
    error_angle = kinematics.THETAI - goal_angle;
    encoders.record_end();
    kinematics.update(encoders.distance_el, encoders.distance_er);
  }
  motors.setMotorPower(-forward_pwm, forward_pwm);
  stopRobot();
  delay(1000);
}

void return_base_turn(){
  state =  STATE_HEADBACK;
  float a = atan(kinematics.YI/kinematics.XI) * 180 / PI;
  if(a < 0 ){
    a = a + 180;    
  }
  float angle_to_turn = kinematics.THETAI + 180 -a;
  turn_angle(angle_to_turn);
  motors.setMotorPower(forward_pwm, forward_pwm);
  encoders.record_end();
  kinematics.update(encoders.distance_el, encoders.distance_er);
  delay(1000);
}

void head_back(){
  encoders.record_end();
  kinematics.update(encoders.distance_el, encoders.distance_er);
  float a = atan(kinematics.YI/kinematics.XI) * 180 / PI;
  if( a < 0 ){
    a = a + 180;
  } 
  float desired_angle = a - 180;
  float term = PID_heading.update(desired_angle, kinematics.THETAI, encoders.dtime);
  motors.setMotorPower(forward_pwm - term, forward_pwm + term);
  delay(100);

  float distance = sqrt(pow(kinematics.XI,2)+pow(kinematics.YI,2));
  if(distance < 0.05){
    motors.setMotorPower(-forward_pwm, -forward_pwm);
    stopRobot();
    delay(100000000);
  }
}
  
void stopRobot(){
  motors.setMotorPower(0, 0);
}


