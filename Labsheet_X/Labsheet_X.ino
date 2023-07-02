#include "motors.h"
#include "linesensor.h"
#include <stdio.h>
#include <stdlib.h>

// # define LINE_SENSOR_UPDATE 10
# define LED_PIN 13
# define FIX_TATAL 100

int len = 5;
int len_new = 5;
const float forward_pwm = 45;
double LINE_SENSOR_UPDATE = FIX_TATAL / forward_pwm;
unsigned long ls_ts = 0;
unsigned long current_ts = 0;
bool led_status;
int code_mark[4];
int code_last = 0;
int code_now = 0;
int code_count = 0;
int code_read[5];
int code_new[5];
double code_average = 0;
double code_variance = 0;
double code_average_new = 0;
int code_threshold = 70;
int data_real_reading[300];
int data_smooth_reading[300];
int data_std[300];
int data_count = 0;


Motors_c motors;
LineSensor_c linesensors;

// put your setup code here, to run once:
void setup() {
  for(int i=0;i<len;i++){
    code_read[i] = 600;
  }
  for(int i=0;i<len_new;i++){
    code_new[i] = 600;
  }
  pinMode(LED_PIN, OUTPUT);
  motors.initialise();
  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");
  delay(3000);
}


// put your main code here, to run repeatedly:
void loop() {
  drive_forward();
  current_ts = millis();
  unsigned long elapsed_time = current_ts - ls_ts;
  if( elapsed_time > LINE_SENSOR_UPDATE ) {
    bool success = linesensors.doParallelSensorRead();
    if(success){
      for(int i=len-1;i>0;i--){
        code_read[i] = code_read[i-1];
      }
      code_read[0] = linesensors.sensor_read[2];
      data_real_reading[data_count] = linesensors.sensor_read[2];

      double code_sum = 0;
      for(int i=0;i<len;i++){
        code_sum += code_read[i];
      }
      code_average = code_sum/len;
      data_smooth_reading[data_count] = int(code_average);

      for(int i=len_new-1;i>0;i--){
        code_new[i] = code_new[i-1];
      }
      code_new[0] =code_average;

      double code_sum_new = 0;
      for(int i=0;i<len_new;i++){
        code_sum_new += code_new[i];
      }
      code_average_new = code_sum_new/len_new;

      double variance_sum = 0;
      for(int i=0;i<len;i++){
        variance_sum += pow((code_read[i] - code_average), 2);
      }
      code_variance = sqrt(variance_sum/len);
      data_std[data_count] = int(code_variance);

      if(code_variance < code_threshold){
        if(code_average < 1150){
          code_now = 0;
        }else if(code_average > 1250 && code_average < 1850){
          code_now = 1;
        }else if(code_average > 1950 && code_average < 2600){
          code_now = 2;
        }else if(code_average > 2600){
          code_now = 3;
        }
      }

      if(code_now != code_last){
        code_count += 1;
        if(code_count == 5){
          // delay(500);
          stopRobot();
          delay(1000);
          show_signal();
          collect_data();
          delay(100000000);
        }
        code_mark[code_count-1] = code_now;
        code_last = code_now;        
      }
      Serial.print(linesensors.sensor_read[2]);
      Serial.print(" ");
      Serial.print(code_variance);
      Serial.print(" ");
      Serial.print(code_average);
      Serial.print(" ");
      Serial.print(code_now);
      Serial.print(" ");
      Serial.println(code_count);
    }
    ls_ts = millis();
    data_count += 1;
  }
}

void show_signal(){
  for(int i=0;i<4;i++){
      Serial.print(code_mark[i]);
      Serial.print(" ");
  }

  int times = 0;
  if(code_mark[0] == 1 && code_mark[1] == 0 && code_mark[2] == 2 && code_mark[3] == 3){
    times = 1;
  }if(code_mark[0] == 1 && code_mark[1] == 0 && code_mark[2] == 3 && code_mark[3] == 2){
    times = 2;
  }if(code_mark[0] == 1 && code_mark[1] == 2 && code_mark[2] == 0 && code_mark[3] == 3){
    times = 3;
  }if(code_mark[0] == 1 && code_mark[1] == 3 && code_mark[2] == 0 && code_mark[3] == 2){
    times = 4;
  }if(code_mark[0] == 2 && code_mark[1] == 0 && code_mark[2] == 1 && code_mark[3] == 3){
    times = 5;
  }if(code_mark[0] == 2 && code_mark[1] == 0 && code_mark[2] == 3 && code_mark[3] == 1){
    times = 6;
  }if(code_mark[0] == 2 && code_mark[1] == 1 && code_mark[2] == 0 && code_mark[3] == 3){
    times = 7;
  }if(code_mark[0] == 2 && code_mark[1] == 3 && code_mark[2] == 0 && code_mark[3] == 1){
    times = 8;
  }if(code_mark[0] == 3 && code_mark[1] == 0 && code_mark[2] == 1 && code_mark[3] == 2){
    times = 9;
  }if(code_mark[0] == 3 && code_mark[1] == 0 && code_mark[2] == 2 && code_mark[3] == 1){
    times = 10;
  }if(code_mark[0] == 3 && code_mark[1] == 1 && code_mark[2] == 0 && code_mark[3] == 2){
    times = 11;
  }if(code_mark[0] == 3 && code_mark[1] == 2 && code_mark[2] == 0 && code_mark[3] == 1){
    times = 12;
  }
  blink(times);
}

void blink(int times){
  for(int i=0;i<times;i++){
    led_status = HIGH;
    digitalWrite(LED_PIN, led_status);
    delay(500);
    led_status = LOW;
    digitalWrite(LED_PIN, led_status);
    delay(500);
  }
}

// void calibration(){
//   intialisingBeeps();
//   state = STATE_DRIVE_FORWARDS;
//   head_angle = kinematics.THETAI;
//   motors.setMotorPower(forward_pwm, forward_pwm);
//   encoders.record_start();
// }

void drive_forward(){
  motors.setMotorPower(forward_pwm, forward_pwm);
}

void stopRobot(){
  motors.setMotorPower(0, 0);
}

void collect_data(){
  if(data_count>=1000){
    data_count = 999;
  }
  while(!Serial){
    delay(1000);
  }

  while(Serial.available() == 0) {
  }
  String teststr = Serial.readString();
  teststr.trim();
  if (teststr == "start") {
    Serial.println("data_real_reading");
    for(int i=0;i<=data_count;i++){
      Serial.println(data_real_reading[i]);
    }
    Serial.println("data_smooth_reading");
    for(int i=0;i<=data_count;i++){
      Serial.println(data_smooth_reading[i]);
    }
    Serial.println("data_std");
    for(int i=0;i<=data_count;i++){
      Serial.println(data_std[i]);
    }
    Serial.println("data_count");
    Serial.println(data_count);
  } else {
    Serial.println("Something wrong");
  }
}    

