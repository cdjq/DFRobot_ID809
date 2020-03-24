/*!
 * @file fingerprintRegistration.ino
 * @brief 采集指纹并保存
 * @n 本实验需要Leonardo主控和ID809指纹模块
 * @n 实验现象：按下3秒内为指纹匹配
 * @n           按下3-6秒为注册指纹
 * @n           6秒以上为删除指纹
 * @n           空闲5秒之后进入休眠
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
#define WAKEUP         6  //唤醒引脚

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

unsigned long time = 0;
uint8_t wakeUpFlag = 1;  //指纹模块唤醒标志位

void loop(){
  if(digitalRead(WAKEUP)){
    /*获取手指按下时间*/
    time = millis();
    /*采集指纹图像，超过5S没按下手指则采集超时*/
    if(finger.fingerprintCollection(/*timeout = */5) == 0){  //采集成功
      /*等待手指松开*/
      while(finger.detectFinger());
      /*获取手指松开时间*/
      time = millis() - time;
    }else{
      time = 0;
    }
    if(time == 0){
      /*指纹采集失败*/
    }else if(time > 0 && time <= 3000){
      Serial.println("进入匹配指纹模式");
      /*对比指纹*/
      fingerprintMatching();
    }else if(time > 3000 && time <=6000){
      Serial.println("进入注册指纹模式");
      /*注册指纹*/
      fingerprintRegistration();
    }else{
      Serial.println("进入删除指纹模式");
      /*删除该指纹*/
      fingerprintDeletion();
    }
    time = millis();
    wakeUpFlag = 1;
  }
  if(wakeUpFlag == 1 && (millis()-time) > 5000){
    wakeUpFlag = 0;
    /*指纹模块进入休眠*/
    finger.enterStandbyState();
  }
}

//匹配指纹
void fingerprintMatching(){
  /*将采集到的指纹与指纹库中的所有指纹对比，成功返回指纹编号，失败返回0*/
  uint8_t ret = finger.search();
  
  if(ret){
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
    finger.LEDCtrl(finger.eKeepsOn, finger.eLEDGreen, /*flashing number = */0);
    Serial.print("匹配成功,ID=");
    Serial.println(ret);
  }else{
    /*设置指纹灯环为红色常亮*/
    finger.LEDCtrl(finger.eKeepsOn, finger.eLEDRed, /*flashing number = */0);
    Serial.println("匹配失败");
  }
  delay(1000);
  /*关闭指纹灯环*/
  finger.LEDCtrl(finger.eNormalClose, finger.eLEDBlue, /*flashing number = */0);
}

//注册指纹
void fingerprintRegistration(){
  uint8_t ID,i;
  /*对比指纹，作用：清除上次采集的指纹图像*/
  finger.search();
  /*获取一个未注册编号，用来保存指纹*/
  ID = finger.getEmptyID();
  Serial.print("ID=");
  Serial.println(ID);
  i = 0;   //采样计数清零
  /*指纹采样三次*/
  while(i < COLLECT_NUMBER){
    /*设置灯环为蓝色呼吸灯*/
    finger.LEDCtrl(finger.eBreathing, finger.eLEDBlue, /*flashing number=*/0);
    Serial.print("正在进行第");
    Serial.print(i+1);
    Serial.println("次指纹采样");
    Serial.println("请按下手指");
    /*采集指纹图像，超过5S没按下手指则采集超时*/
    if(finger.fingerprintCollection(/*timeout = */5) == 0){
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
}


//删除指纹
void fingerprintDeletion(){
  uint8_t ret;
  /*将采集到的指纹与指纹库中的所有指纹对比，成功返回指纹编号，失败返回0*/
  ret = finger.search();

  if(ret){
    /*设置指纹灯环为绿色常亮*/
    finger.LEDCtrl(finger.eKeepsOn, finger.eLEDGreen, /*flashing number = */0);
    finger.delFingerprint(ret);
    Serial.print("已删除指纹,ID=");
    Serial.println(ret);
  }else{
    /*设置指纹灯环为红色常亮*/
    finger.LEDCtrl(finger.eKeepsOn, finger.eLEDRed, /*flashing number = */0);
    Serial.println("匹配失败或该指纹未注册");
  }
  delay(1000);
  /*关闭指纹灯环*/
  finger.LEDCtrl(finger.eNormalClose, finger.eLEDBlue, /*flashing number = */0);
}