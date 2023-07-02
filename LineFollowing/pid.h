// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _PID_H
#define _PID_H

// Class to contain generic PID algorithm.
class PID_c {
  public:
    // Constructor, must exist.
    PID_c() {

    } 

    float p_term;
    float i_term;
    float d_term;
    float Kp;
    float Ki;
    float Kd;
    float et;
    void initialise(float KP=10, float KI=0.001, float KD = 0.1){
      p_term = 0;
      i_term = 0;
      d_term = 0;
      Kp = KP;
      Ki = KI;
      Kd = KD;
      et = 0;
    }

    float update( float demand, float measurement, float dtime ) {
      float et_now = demand - measurement;
      p_term = Kp * et_now;
      i_term += Ki * et_now * dtime;
      d_term = Kd * (et_now - et) / dtime;
      et = et_now;
      float term = p_term + i_term + d_term;

      Serial.print("p_term: ");
      Serial.println(p_term);
      Serial.print("i_term: ");
      Serial.println(i_term);
      Serial.print("d_term: ");
      Serial.println(d_term);
      Serial.print("term: ");
      Serial.println(term);
      return term;
    }  
    

};



#endif
