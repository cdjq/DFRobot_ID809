/*!
 * @file fingerprintDeletion.ino
 * @brief 删除指定指纹
 * @n 实验现象：检查ID是否被注册，如果已注册则删除，
                如果未注册则提示ID未注册
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_ID809
*/
#include <DFRobot_ID809.h>

#define FINGERPRINTID 1   //待删除指纹的编号

/*如果使用UNO或NANO，则使用软串口*/
#if ((defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO))
    #include <SoftwareSerial.h>
    SoftwareSerial Serial1(2, 3);  //RX, TX
    #define FPSerial Serial1
#else
    #define FPSerial Serial1
#endif

DFRobot_ID809 fingerprint;
String desc;

void setup(){
  /*初始化打印串口*/
  Serial.begin(9600);
  /*初始化FPSerial*/
  FPSerial.begin(115200);
  /*将FPSerial作为指纹模块的通讯串口*/
  fingerprint.begin(FPSerial);
  /*等待Serial打开*/
  while(!Serial);
  /*测试设备与主控是否能正常通讯,
    返回true or false
    */
  while(fingerprint.isConnected() == false){
    Serial.println("与设备通讯失败，请检查接线");
    /*获取错误码信息*/
    desc = fingerprint.getErrorDescription();
    Serial.println(desc);
    delay(1000);
  }
}

void loop(){
  /*检查指纹编号是否被注册
    如果已注册返回1,否则返回0
   */
  if(fingerprint.getStatusID(/*Fingerprint ID = */FINGERPRINTID)){
    Serial.println("ID已注册");
    /*删除该编号的指纹*/
    fingerprint.delFingerprint(/*Fingerprint ID = */FINGERPRINTID);
    //fingerprint.delFingerprint(DELALL);  //删除所有指纹
    Serial.println("ID已删除");
  }else{
    Serial.println("ID未注册");
  }
  Serial.println("-----------------------------");
  delay(1000);
}