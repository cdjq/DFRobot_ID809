/*!
 * @file queryDeviceBPS.ino
 * @brief 查询设备波特率
 * @n 实验现象：查询设备波特率，并通过串口打印出来
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

uint32_t ID809_BPS[8] = {9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
uint8_t i = 0;

void setup(){
  /*初始化打印串口*/
  Serial.begin(9600);
  /*测试模块波特率*/
  do{
    /*初始化FPSerial*/
    Serial.print(".");
    FPSerial.begin(ID809_BPS[i]);
    /*将FPSerial作为指纹模块的通讯串口*/
    fingerprint.begin(FPSerial);
    i++;
  }while(fingerprint.isConnected() == false);
  Serial.println(" ");
}

void loop(){
  Serial.print("模块波特率为:");
  Serial.println(ID809_BPS[i-1]);
  Serial.println("-----------------------------");
  delay(1000);
}