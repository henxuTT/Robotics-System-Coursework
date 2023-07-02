// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_OUT_LEFT_PIN 12
#define LS_LEFT_PIN A0
#define LS_CENTRE_PIN A2
#define LS_RIGHT_PIN A3
#define LS_OUT_RIGHT_PIN A4
#define EMIT_PIN 11
#define NB_LS_PINS 5

// Class to operate the linesensor(s).
class LineSensor_c {
  public:

    // Constructor, must exist.
    LineSensor_c(){
    } 

  int ls_pin[NB_LS_PINS] = { LS_OUT_LEFT_PIN, LS_LEFT_PIN, LS_CENTRE_PIN, LS_RIGHT_PIN, LS_OUT_RIGHT_PIN };
  unsigned long sensor_read[NB_LS_PINS];
  bool timeout_flag = false;
  unsigned long timeout = 5000;
  int reach_line_count = 0;
  
  float getLineError(){
    float sum = 0;
    for(int i=0;i<NB_LS_PINS;i++){
      sum += sensor_read[i];
    }
    float weight_left = (1.2*sensor_read[0] + sensor_read[1] + 0.5*sensor_read[2])/sum;
    float weight_right = (1.2*sensor_read[4] + sensor_read[3] + 0.5*sensor_read[2])/sum;

    float e_line = weight_left - weight_right;
    return e_line;  
  }

  bool out_line_test(){
    if(sensor_read[0]<1200 && sensor_read[1]<1200 && sensor_read[2]<1200 && sensor_read[3]<1200 && sensor_read[4]<1200){
      return true;
    }else{
      return false;
    }
  }

  bool line_detect(){
    bool success = doParallelSensorRead();
    bool reach_line_flag = false;
    if(success){
      if(sensor_read[1]>1900 || sensor_read[2]>1900 || sensor_read[3]>1900){
        reach_line_flag = true;
        reach_line_count += 1; 
      }
    }
    return reach_line_flag;
  }


  bool doParallelSensorRead(){
    unsigned long start_time;
    unsigned long end_time; 
    unsigned long elapsed_time;
    int remaining = NB_LS_PINS;
  
    //initialize parameter
    timeout_flag = false;
    for(int i=0;i<NB_LS_PINS;i++){
      sensor_read[i]=0;
    }

    pinMode(EMIT_PIN, OUTPUT);
    digitalWrite(EMIT_PIN, HIGH);

    for(int i=0;i<NB_LS_PINS;i++){
      pinMode(ls_pin[i], OUTPUT);
      digitalWrite(ls_pin[i], HIGH);
    }
    delayMicroseconds(10);
    for(int i=0;i<NB_LS_PINS;i++){
      pinMode(ls_pin[i], INPUT); 
    }

    //start measuring
    start_time = micros();
    
    while( remaining != 0 ) {
      for( int i=0; i<NB_LS_PINS; i++ ) {
        if( digitalRead( ls_pin[i] ) == LOW ) {
          if( sensor_read[i] == 0) {
            elapsed_time = micros() - start_time;
            sensor_read[i] = elapsed_time;
            remaining = remaining - 1;
          } 
        }
      }

      elapsed_time = micros() - start_time;
      if(elapsed_time>=timeout){
        timeout_flag == true;
        break;
      }
    }
    
    // Serial.println( "remaining" );
    // Serial.println( remaining );
    // Serial.println( "elapsed_time" );
    // Serial.println( elapsed_time );

    if(timeout_flag == true){
      Serial.print( "TIMEOUT" );
      Serial.print("\n");
    }else{
      // Serial.print( "elapsed_time: " );
      // for(int i=0 ; i<NB_LS_PINS; i++ ){
      //   Serial.print( sensor_read[i] );
      //   Serial.print(" ");
      // }
      // Serial.print("\n");
    }

    return !timeout_flag;
  }
};



#endif
