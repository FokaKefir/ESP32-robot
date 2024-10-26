#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems

#include <DCMotor.h>

// #define MOTOR_1_PIN_1   14
// #define MOTOR_1_PIN_2   15
// #define MOTOR_1_PIN_EN  4
// #define MOTOR_2_PIN_1   13
// #define MOTOR_2_PIN_2   12
// #define MOTOR_2_PIN_EN  2

#include "stream.h"


// DCMotor motor1 = DCMotor(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_EN);
//DCMotor motor2 = DCMotor(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_EN);

int action_callback(char variable[32]) {
  Serial.println(variable);
  // if(!strcmp(variable, "forward")) {
  //   Serial.println("Forward");
  // }
  // else if(!strcmp(variable, "left")) {
  //   Serial.println("Left");
  // }
  // else if(!strcmp(variable, "right")) {
  //   Serial.println("Right");
  // }
  // else if(!strcmp(variable, "backward")) {
  //   Serial.println("Backward");
  // }
  // else if(!strcmp(variable, "stop")) {
  //   Serial.println("Stop");
  // } else {
  //   return -1;
  // }
  return 0;
}


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector 
  
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  initStream();
  
  // Start streaming web server
  startCameraServer(action_callback);
}

void loop() {

}
