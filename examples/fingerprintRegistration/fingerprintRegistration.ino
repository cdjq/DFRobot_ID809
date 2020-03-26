/*!
 * @file fingerprintRegistration.ino
 * @brief 采集指纹并保存
 * @n 该模块可以使用硬串口或软串口控制
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
 * @url https://github.com/cdjq/DFRobot_ID809
*/

#include <DFRobot_ID809.h>

#define COLLECT_NUMBER 3  //指纹采样次数，可设置1-3

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
  /*将Serial1作为指纹模块的通讯串口*/
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

uint8_t ID,i,ret;

void loop(){
  /*获取一个未注册编号，用来保存指纹
    获取成功返回ID号，
    获取失败返回ERR_ID809
   */
  if((ID = fingerprint.getEmptyID()) == ERR_ID809){
    while(1){
      /*获取错误码信息*/
      desc = fingerprint.getErrorDescription();
      Serial.println(desc);
      delay(1000);
    }
  }
  Serial.print("未注册编号,ID=");
  Serial.println(ID);
  i = 0;   //采样计数清零
  /*指纹采样三次*/
  while(i < COLLECT_NUMBER){
    /*设置指纹灯环模式、颜色和闪烁次数，
      可设置参数如下：
      参数1:<LEDMode>
      eBreathing   eFastBlink   eKeepsOn    eNormalClose
      eFadeIn      eFadeOut     eSlowBlink   
      参数2:<LEDColor>
      eLEDGreen  eLEDRed      eLEDYellow   eLEDBlue
      eLEDCyan   eLEDMagenta  eLEDWhite
      参数3:<呼吸、闪烁次数> 0表示一直呼吸、闪烁，
      该参数仅在eBreathing、eFastBlink、eSlowBlink模式下有效
     */
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eBreathing, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
    Serial.print("正在进行第");
    Serial.print(i+1);
    Serial.println("次指纹采样");
    Serial.println("请按下手指");
    /*采集指纹图像，超过10S没按下手指则采集超时
      如果获取成功返回0，否则返回ERR_ID809
     */
    if((ret = fingerprint.collectionFingerprint(/*timeout = */10)) != ERR_ID809){
      /*设置指纹灯环为黄色快闪3次*/
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3);
      Serial.println("采集成功");
      i++;   //采样计数+1
    }else{
      Serial.println("采集失败");
      /*获取错误码信息*/
      desc = fingerprint.getErrorDescription();
      Serial.println(desc);
    }
    Serial.println("请松开手指");
    /*等待手指松开
      检测到手指返回1，否则返回0
     */
    while(fingerprint.detectFinger());
  }
  
  /*将指纹信息保存到一个未注册的编号中*/
  if(fingerprint.storeFingerprint(/*Empty ID = */ID) != ERR_ID809){
    Serial.print("保存成功，ID=");
    Serial.println(ID);
    Serial.println("-----------------------------");
    /*设置指纹灯环为绿色常亮*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    delay(1000);
    /*关闭指纹灯环*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eNormalClose, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
    delay(1000);
  }else{
    Serial.println("保存失败");
    /*获取错误码信息*/
    desc = fingerprint.getErrorDescription();
    Serial.println(desc);
  }
}