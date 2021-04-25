/*!
 * @file downLoadImage.ino
 * @brief 下载指纹图像到传感器中，并转化为模板用于识别
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_ID809
*/
#include <DFRobot_ID809.h>
//需要在此放指纹图像的位图数据
//位图颜色深度为256灰度位图,像素为160x160
uint8_t temp[160*160]={0};

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
  
  
  fingerprint.downLoadImage(5,temp);
  Serial.println("over");
}

uint8_t enrollCount;   //Number of registered users 

void loop(){


  delay(1000);
}