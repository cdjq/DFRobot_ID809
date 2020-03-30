/*!
 * @file getDeviceInformation.ino
 * @brief 获取指纹模块信息
 * @n 实验现象：串口打印出模块的ID、安全等级、波特率等信息
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

uint8_t enrollCount;   //已注册用户数

void loop(){
  /*设置模块ID号，可设置值1-255*/
  //fingerprint.setDeviceID(/*Device ID = */1);
  Serial.print("模块ID为:");
  /*读取模块ID*/
  Serial.println(fingerprint.getDeviceID());
  
  /*设置模块安全等级，可设置值1-5，默认安全等级3
    Security Level          识别率
      Level 1          认假率 FAR  0.1%
                       拒真率 FRR  0.005%
      Level 2          认假率 FAR  0.003%
                       拒真率 FRR  0 01%
      Level 3          认假率 FAR  0.001%
                       拒真率 FRR  0.1%
      Level 4          认假率 FAR  0.003%
                       拒真率 FRR  0.5%
      Level 5          认假率 FAR  0.0001%
                       拒真率 FRR  1%
   */
  //fingerprint.setSecurityLevel(/*Security Level = */3);
  Serial.print("模块安全等级为:");
  /*读取模块安全等级*/
  Serial.println(fingerprint.getSecurityLevel());
  
  /*设置模块波特率，可设置范围如下所示：
    e9600bps    e19200bps   e38400bps   e57600bps
       1           2            3          4
    e115200bps  e230400bps  e460800bps  e921600bps
       5           6            7          8
   */
  //fingerprint.setBaudrate(fingerprint.e115200bps);
  Serial.print("模块波特率为:");
  /*读取模块波特率*/
  Serial.println(fingerprint.getBaudrate());
  
  /*设置模块自学功能，1(ON) 0(OFF)*/
  //fingerprint.setAutoLearn(/*Auto Learn = */1);
  Serial.print("模块自学功能:");
  /*读取模块自学功能状态
    如果开启则打印ON,否则打印OFF*/
  Serial.println(fingerprint.getAutoLearn()?"ON":"OFF");
  
  /*设置模块序列号,序列号字符数必须小于15个*/
  //fingerprint.setModuleSN(/*Module SN = */"DFRobot");
  Serial.print("模块序列号为:");
  /*读取模块序列号*/
  Serial.println(fingerprint.getModuleSN());
  
  Serial.print("模块内部已注册指纹数量:");
  /*获取注册用户数量*/
  Serial.println(enrollCount = fingerprint.getEnrollCount());
  /*声明一个数组用来存放ID列表*/
  uint8_t list[enrollCount] = {0};
  /*获取用户列表
    传入一个数组指针用来接收ID列表
   */
  fingerprint.getEnrolledIDList(list);
  Serial.print("已注册用户ID列表:");
  for(uint8_t i = 0; i < enrollCount; i++){
    Serial.print(list[i]);
    Serial.print(",");
  }
  
  Serial.print("\n指纹损坏数量:");
  /*获取损坏的指纹数量*/
  Serial.println(fingerprint.getBrokenQuantity());
  /*获取第一个损坏的指纹ID号*/
  //fingerprint.getBrokenID();
  Serial.println("-----------------------------");

  delay(1000);
}