/*!
 * @file fingerprintRegistration.ino
 * @brief 采集指纹并保存
 * @n 本实验需要Leonardo主控和ID809指纹模块
 * @n 实验现象：自动获取空白ID，然后采集三次指纹，采集时
 * @n           设置灯环为蓝色呼吸灯，采集成功黄色灯快闪
 * @n           3次，最后将指纹保存到获取到的未注册的编
 * @n           号中，绿色灯亮1S然后熄灭
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_SHT3x
*/

#include <DFRobot_ID809.h>

#define COLLECT_NUMBER 3  //指纹采样次数，可设置1-3

/*
#ifdef ARDUINO_UNO   //???
  SoftwareSerial Serial1(2, 3); // RX, TX
#endif*/

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

uint8_t ID,i,ret;

void loop(){
  /*获取一个未注册编号，用来保存指纹*/
  ID = finger.getEmptyID();
  Serial.print("未注册编号,ID=");
  Serial.println(ID);
  i = 0;   //采样计数清零
  /*指纹采样三次*/
  while(i < COLLECT_NUMBER){
    /*设置指纹灯环模式、颜色和闪烁次数，
      可设置参数如下：
      <LEDMode>
      eBreathing   eFastBlink   eKeepsOn    eNormalClose
      eFadeIn      eFadeOut     eSlowBlink   
      <LEDColor>
      eLEDGreen  eLEDRed      eLEDYellow   eLEDBlue
      eLEDCyan   eLEDMagenta  eLEDWhite
      <闪烁次数> 0表示一直闪烁
      */
    finger.LEDCtrl(finger.eBreathing, finger.eLEDBlue, /*flashing number=*/0);
    Serial.print("正在进行第");
    Serial.print(i+1);
    Serial.println("次指纹采样");
    Serial.println("请按下手指");
    /*采集指纹图像，超过10S没按下手指则采集超时*/
    if((ret = finger.fingerprintCollection(/*timeout = */10)) == 0){
      /*设置指纹灯环为黄色快闪3次*/
      finger.LEDCtrl(finger.eFastBlink, finger.eLEDYellow, /*flashing number = */3);
      Serial.println("采集成功");
      i++;   //采样计数+1
    }else{
      //打印ret对应的错误码
      Serial.println("采集失败");
    }
    Serial.println("请松开手指");
    /*等待手指松开*/
    while(finger.detectFinger());
  }
  
  /*将指纹信息保存到一个未注册的编号中*/
  finger.storeFingerprint(ID);
  Serial.print("保存成功，ID=");
  Serial.println(ID);
  Serial.println("-----------------------------");
  /*设置指纹灯环为绿色常亮*/
  finger.LEDCtrl(finger.eKeepsOn, finger.eLEDGreen, /*flashing number = */0);
  delay(1000);
  /*关闭指纹灯环*/
  finger.LEDCtrl(finger.eNormalClose, finger.eLEDBlue, /*flashing number = */0);
  delay(1000);
}