#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems


#include "stream.h"


int foo(char variable[32]) {
  if(!strcmp(variable, "forward")) {
    Serial.println("Forward");
  }
  else if(!strcmp(variable, "left")) {
    Serial.println("Left");
  }
  else if(!strcmp(variable, "right")) {
    Serial.println("Right");
  }
  else if(!strcmp(variable, "backward")) {
    Serial.println("Backward");
  }
  else if(!strcmp(variable, "stop")) {
    Serial.println("Stop");
  } else {
    return -1;
  }
  return 0;
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector 
  
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  
  initStream();
  
  // Start streaming web server
  startCameraServer(foo);
}

void loop() {
  // put your main code here, to run repeatedly:

}
