/*!
 * @file getTemplate.ino
 * @brief Get fingerprint module information 
 * @n Experiment Phenomenon：serial print module ID, security level, baud rate, etc. 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_ID809
*/

#include <DFRobot_ID809.h>

//DFRobot_ID809_IIC fingerprint;
DFRobot_ID809_UART fingerprint(115200);
//String desc;
uint8_t data[1024]={0};
void setup(){
  /*Init print serial port */
  Serial.begin(9600);
  /*Take FPSerial as communication port of the module*/
  fingerprint.begin();
  /*Wait for Serial to open*/
  /*Test whether device can communicate properly with mainboard 
    Return true or false
    */
  while(fingerprint.isConnected() == false){
    Serial.println("Communication with device failed, please check connection");
    /*Get error code information */
    //desc = fingerprint.getErrorDescription();
    //Serial.println(desc);
    delay(1000);
  }
  
  Serial.println("start");
  fingerprint.getTemplate(1,data+1);
  for(uint16_t i = 1 ;i<1024;i++){
    Serial.print(data[i],HEX);
    Serial.print(" ");
   if(i%8 == 0  &&  i!= 0){
    Serial.println();
   }


  }
}

uint8_t enrollCount;   //Number of registered users 

void loop(){


  delay(1000);
}
