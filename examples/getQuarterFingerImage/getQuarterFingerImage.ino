/*!
 *@file getQuarterFingerImage.ino
 *@brief 将采集到的指纹图像储存与SD卡中,并显示在屏幕上,可选择获取1/4图像或者全图像
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@licence     The MIT License (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2021-6-01
 *@get from https://www.dfrobot.com
 *@https://github.com/DFRobot/DFRobot_ID809
*/
#include <DFRobot_ID809.h>
#include "bmpHeader.h"
#include <SD.h>
#include "DFRobot_GDL.h"
//Custom communication pins
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D7
#define TFT_CS  D8
#define TFT_RST D9
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif





/*Use software serial when using UNO or NANO*/
#if ((defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO))
    #include <SoftwareSerial.h>
    SoftwareSerial Serial1(2, 3);  //RX, TX
    #define FPSerial Serial1
#else
    #define FPSerial Serial1
#endif
DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
DFRobot_ID809 fingerprint;
//String desc;

#define QUARTER
#ifdef  QUARTER
uint8_t data[6400];   //四分之一
#else
uint8_t data[25600];  //全图像
#endif

void setup(){
  /*Init print serial port */
  Serial.begin(9600);
  /*Init FPSerial*/
  FPSerial.begin(115200);
  /*Take FPSerial as communication port of fingerprint module */
  fingerprint.begin(FPSerial);
  screen.begin();
  screen.fillScreen(0xf800);
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
}
File myFile;
void loop(){
  //设置采集灯环
  fingerprint.ctrlLED(/*LEDMode = */fingerprint.eBreathing, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
  Serial.println("请按下手指");
    /*等待手指按下
      检测到手指返回1，否则返回0
     */
  while(!fingerprint.detectFinger());
  #ifdef  QUARTER
   //采集图像,这是1/4大小图像
  fingerprint.getQuarterFingerImage(data);
  #else
  //采集全图像
  fingerprint.getFingerImage(data);
  #endif
  if (!SD.begin(/*csPin = */3, /*type = */TYPE_NONBOARD_SD_MOUDLE)) {
    SerialUSB.println("initialization failed!");
    while(1);
  }
  myFile = SD.open("finger.bmp", FILE_WRITE);
  #ifdef  QUARTER
  //将图片保存为bmp格式的文件到SD卡
  myFile.write((const char *)bmpHeader, sizeof(bmpHeader));
  for(uint8_t i = 0;i < 13 ;i++){
    if(i == 12){
      myFile.write((const char *)data+i*512, 256);
      break;
      }
    myFile.write((const char *)data+i*512, 512);
    }
    myFile.close(); 
   //将图片显示在屏幕上
   for(uint16_t i = 0; i < 6400 ;i++){
     uint8_t a = i % 80;
     uint8_t b = i / 80;
     screen.drawPixel(80+a,120+b,(data[i]/4)<<5);
  
   }
  #else
  bmpHeader[18] = 0xa0;
  bmpHeader[22] = 0xa0;
  myFile.write((const char *)bmpHeader, sizeof(bmpHeader));
  for(uint8_t i = 0;i < 50 ;i++){
    myFile.write((const char *)data+i*512, 512);
    }
  myFile.close(); 
  //将图片显示在屏幕上
  for(uint16_t i = 0; i < 25600 ;i++){
     uint8_t a = i % 160;
     uint8_t b = i / 160;
     screen.drawPixel(40+a,80+b,(data[i]/4)<<5);
   }
  #endif
   
  

  while(1);
}
