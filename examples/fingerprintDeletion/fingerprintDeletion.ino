/*!
 * @file fingerprintDeletion.ino
 * @brief 采集指纹并保存
 * @n 实验现象：检查ID是否被注册，如果已注册则删除，
                如果未注册则提示ID未注册
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_SHT3x
*/
#include <DFRobot_ID809.h>

#define FINGERPRINTID 1   //待删除指纹的编号

DFRobot_ID809 finger;

void setup(){
  /*初始化打印串口*/
  Serial.begin(9600);
  /*初始化Serial1*/
  Serial1.begin(115200);
  /*将Serial1作为指纹模块的通讯串口*/
  finger.begin(Serial1);
  /*等待Serial打开*/
  while(!Serial);
  /*测试设备与主控是否能正常通讯*/
  while(finger.testConnection()){
    Serial.println("与设备通讯失败，请检查接线");
    delay(1000);
  }
}

void loop(){
  //检查指纹编号是否被注册
  if(finger.getStatusID(FINGERPRINTID)){
    Serial.println("ID已注册");
    //删除该编号的指纹
    finger.delFingerprint(FINGERPRINTID);
    //finger.delChar(DELALL);  //删除所有指纹
    Serial.println("ID已删除");
  }else{
    Serial.println("ID未注册");
  }
  Serial.println("-----------------------------");
  delay(1000);
}