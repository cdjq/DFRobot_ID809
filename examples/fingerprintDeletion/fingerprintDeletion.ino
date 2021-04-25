/*!
 * @file fingerprintDeletion.ino
 * @brief Delete a specific fingerprint 
 * @n Experiment phenomenon：press your finger on the sensor, if this fingerprint is registered, delete it and LED turns on in green. 
      If it is unregistered or fingerprint collection fails, LED light turns on in red.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_ID809
*/
#include <DFRobot_ID809.h>

//DFRobot_ID809_IIC fingerprint;
DFRobot_ID809_UART fingerprint(115200);
//String desc;

void setup(){
  /*Init print serial port */
  Serial.begin(115200);

  Serial.println("over");
}

void loop(){

}
