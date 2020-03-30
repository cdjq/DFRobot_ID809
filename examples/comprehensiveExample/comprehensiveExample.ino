/*!
 * @file comprehensiveExample.ino
 * @brief 综合示例
 * @n 该模块可以使用硬串口或软串口控制，本实验需要将IRQ引脚接到D6
 * @n 实验现象：按下手指，蓝灯快闪3次表示进入对比指纹模式
 * @n           黄灯快闪3次，表示进入指纹注册模式
 * @n           红灯快闪3次，表示进入删除指纹模式，删除该指纹
 * @n           空闲5秒后进入休眠
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
#define WAKEUP         6  //唤醒引脚

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

unsigned long time = 0;
uint8_t wakeUpFlag = 1;  //指纹模块唤醒标志位

//蓝灯 匹配模式  绿灯 注册模式  红灯  删除模式
void loop(){
  if(digitalRead(WAKEUP)){
    uint16_t i = 0;
    /*采集指纹图像，超过5S没按下手指则采集超时
      如果获取成功返回0，否则返回ERR_ID809
     */
    if((fingerprint.collectionFingerprint(/*timeout=*/5)) != ERR_ID809){
      /*获取手指按下时间*/
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
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */3);  //蓝灯快闪3次，表示正处于对比指纹模式
      /*等待手指松开*/
      while(fingerprint.detectFinger()){
        delay(50);
        i++;
        if(i == 15){             //黄灯快闪3次，表示正处于注册指纹模式
          /*设置指纹灯环为黄色常亮*/
          fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3);
        }else if(i == 30){      //红灯快闪3次，表示正处于删除指纹模式
          /*设置指纹灯环为红色常亮*/
          fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */3);
        }
      }
    }
    if(i == 0){
      /*采集指纹失败*/
    }else if(i > 0 && i < 15){
      Serial.println("进入匹配指纹模式");
      /*对比指纹*/
      fingerprintMatching();
    }else if(i >= 15 && i < 30){
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
    fingerprint.enterStandbyState();
  }
}

//对比指纹
void fingerprintMatching(){
  /*将采集到的指纹与指纹库中的所有指纹对比，
    成功返回指纹编号(1-80)，失败返回0
   */
  uint8_t ret = fingerprint.search();
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
  delay(1000);
  /*关闭指纹灯环*/
  fingerprint.ctrlLED(/*LEDMode = */fingerprint.eNormalClose, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
  Serial.println("-----------------------------");
}

//注册指纹
void fingerprintRegistration(){
  uint8_t ID,i;
  /*将采集到的指纹与指纹库中的所有指纹对比，
    成功返回指纹编号(1-80)，失败返回0
    此处作用：清除上次采集的指纹图像。
   */
  fingerprint.search();       //可增加判断语句，检测该指纹是否注册
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
    /*设置指纹灯环为蓝色呼吸*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eBreathing, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
    Serial.print("正在进行第");
    Serial.print(i+1);
    Serial.println("次指纹采样");
    Serial.println("请按下手指");
    /*采集指纹图像，超过10S没按下手指则采集超时
      如果获取成功返回0，否则返回ERR_ID809
     */
    if((fingerprint.collectionFingerprint(/*timeout = */10)) != ERR_ID809){
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
    /*设置指纹灯环为绿色常亮*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    delay(1000);
    /*关闭指纹灯环*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eNormalClose, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
  }else{
    Serial.println("保存失败");
    /*获取错误码信息*/
    desc = fingerprint.getErrorDescription();
    Serial.println(desc);
  }
  Serial.println("-----------------------------");
}


//删除指纹
void fingerprintDeletion(){
  uint8_t ret;
  /*将采集到的指纹与指纹库中的所有指纹对比，
    成功返回指纹编号(1-80)，失败返回0
   */
  ret = fingerprint.search();
  if(ret){
    /*设置指纹灯环为绿色常亮*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
    fingerprint.delFingerprint(ret);
    Serial.print("已删除指纹,ID=");
    Serial.println(ret);
  }else{
    /*设置指纹灯环为红色常亮*/
    fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */0);
    Serial.println("匹配失败或该指纹未注册");
  }
  delay(1000);
  /*关闭指纹灯环*/
  fingerprint.ctrlLED(/*LEDMode = */fingerprint.eNormalClose, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
  Serial.println("-----------------------------");
}