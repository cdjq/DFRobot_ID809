/*!
 *@file contrastTemplate.ino
 *@brief 将ID 1指纹模板提取出来，采集指纹与模板做对比
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-01
 *@get from https://www.dfrobot.com
 *@https://github.com/DFRobot/DFRobot_ID809
*/
#include <DFRobot_ID809.h>

//模板数据
uint8_t temp[1008]={0};

/*Use software serial when using UNO or NANO*/
#if ((defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO))
    #include <SoftwareSerial.h>
    SoftwareSerial Serial1(2, 3);  //RX, TX
    #define FPSerial Serial1
#else
    #define FPSerial Serial1
#endif

DFRobot_ID809 fingerprint;
//String desc;

void setup(){
  /*Init print serial port */
  Serial.begin(9600);
  /*Init FPSerial*/
  FPSerial.begin(115200);
  /*Take FPSerial as communication port of fingerprint module */
  fingerprint.begin(FPSerial);
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
  //获取ID 1的指纹模板
  fingerprint.getTemplate(/*id = */1,temp);
  //将模板数据下载到ID 1中
  //fingerprint.downLoadTemplate(/*id = */1,temp);
}

void loop(){
  //设置采集灯环
  fingerprint.ctrlLED(/*LEDMode = */fingerprint.eBreathing, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
  Serial.println("请按下手指");
  /*采集指纹图像，关闭采集超时功能
    如果获取成功返回0，否则返回ERR_ID809
     采集一次指纹模板数据,然后储存到rambuffer0
   */
  if((fingerprint.collectionFingerprint(/*timeout=*/0,0)) != ERR_ID809){
    /*设置指纹灯环为黄色快闪3次*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3);
    Serial.println("采集成功");
    Serial.println("请松开手指");
    /*等待手指松开
      检测到手指返回1，否则返回0
     */
    while(fingerprint.detectFinger());
    //将模板数据与采集的指纹做对比
    if(!fingerprint.contrastTemplate(/*TEMP = */temp)){
      /*设置指纹灯环为绿色常亮*/
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
      Serial.println("对比成功,模板与手指匹配");
    }else{
      /*设置指纹灯环为红色常亮*/
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */0);
      Serial.println("对比失败,模板手指不匹配");
    }
  }else{
    Serial.println("采集失败");
  }
  Serial.println("-----------------------------");
  delay(1000);
}
