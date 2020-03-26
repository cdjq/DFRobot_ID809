/*!
 * @file fingerprintMatching.ino
 * @brief 采集指纹并与指纹库中指纹做对比
 * @n 实验现象：采集指纹图像并与指纹库中的指纹最对比，
                对比成功亮绿色灯并打印ID号，对比失败返回0
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](Eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_ID809
*/
#include <DFRobot_ID809.h>

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
  /*初始化Serial1*/
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

uint8_t ret;

void loop(){
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
  Serial.println("请按下手指");
  /*采集指纹图像，超过10S没按下手指则采集超时
    如果获取成功返回0，否则返回ERR_ID809
   */
  if((ret = fingerprint.collectionFingerprint(/*timeout=*/10)) != ERR_ID809){
    /*设置指纹灯环为黄色快闪3次*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3);
    Serial.println("采集成功");
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
  
  /*将采集到的指纹与指纹库中的所有指纹对比，
    成功返回指纹编号(1-80)，失败返回0
   */
  ret = fingerprint.search();
  /*将采集到的指纹与指定编号指纹对比
    成功返回指纹编号(1-80)，失败返回0
   */
  //ret = fingerprint.verify(/*Fingerprint ID = */1);  
  if(ret != 0){
    /*设置指纹灯环为绿色常亮*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    Serial.print("匹配成功,ID=");
    Serial.println(ret);
  }else{
    /*设置指纹灯环为红色常亮*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */0);
    Serial.println("匹配失败");
  }
  Serial.println("-----------------------------");
  delay(1000);
}