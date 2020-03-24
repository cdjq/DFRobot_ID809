# DFRobot_CCS811
This sensor is used to measure the concentration of carbon dioxide and TVOC(TotalVolatileOrganicCompounds) via IIC within measurement range 400-8000ppm(parts per million) for the former and 0-1100ppb(parts per billion) for the latter. It features small size, low power consumption, high accuracy and short pre-heat time. Besides, this sensor provides grade 1 MSL(moisture sensitivity level), which makes the sensor suitable for all kinds of moist operating conditions. Meanwhile, users can input environment parameter to calibrate the output data. <br>

Carbon dioxide concentration(ppm)  | Impact on human-beings
------------------ | ---------
more than 5000        |      toxic 
2500-5000        |      unhealthy to human body
1000-2500        |      feel sleepy 
500-1000        |      turbid air
less than 500        |      no effect

TVOC concentration (ppb)     | Impact on human-beings
------------------ | ---------
less than 50        |      no effect 
50-750        |      uncomfortable, nervous 
760-6000        |      uncomfortable, nervous, headache
more than 6000       |      headache, neurological problems 

<br>
<img src="./image/SEN0318-image.jpg">
<br>

* Arduino UNO
<br>
<img src="./image/SEN0318-CONNECT.jpg">
<br>

   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

1. Read carbon dioxide concentration (Unit:ppm) <br>
2. Read TVOC concentration (Unit:ppb) <br>
3. Change the sampling interval of data reading to change power consumption <br>
4. Set environment parameter to calibrate the readings <br>
5. Supports for interrupt measurement <br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++

  /**
   * @brief 测试模块是否正常连接
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t testConnection();
  
  /**
   * @brief 设置模块ID
   * @param ID号:1-255
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t setDeviceID(uint8_t DeviceID);
  
  /**
   * @brief 设置模块安全等级
   * @param 安全等级:1-5
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t setSecurityLevel(uint8_t SecurityLevel);
  
  /**
   * @brief 设置模块指纹重复检查(在保存指纹时，检查是否已被注册)
   * @param 1(ON) or 0(OFF)
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t setDuplicationCheck(uint8_t DuplicationCheck);
  
  /**
   * @brief 设置模块波特率
   * @param Baudrate:in typedef enum eDEVICE_BAUDRATE_t
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t setBaudrate(eDEVICE_BAUDRATE_t Baudrate);
  
  /**
   * @brief 设置模块自学功能(在对比指纹成功时，更新指纹)
   * @param 1(ON) or 0(OFF)
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t setAutoLearn(uint8_t AutoLearn);
  
  /**
   * @brief 读取模块ID
   * @return ID号:1-255
   */
  uint8_t readDeviceID();
  
  /**
   * @brief 读取模块安全等级
   * @return 安全等级:1-5
   */
  uint8_t readSecurityLevel();
  
  /**
   * @brief 读取模块指纹重复检查状态
   * @return 1(ON) or 0(OFF)
   */
  uint8_t readDuplicationCheck();
  
  /**
   * @brief 读取模块波特率
   * @return Baudrate:in typedef enum eDEVICE_BAUDRATE_t
   */
  uint8_t readBaudrate();
  
  /**
   * @brief 读取模块自学功能状态
   * @return 1(ON) or 0(OFF)
   */
  uint8_t readAutoLearn();
   
  /**
   * @brief 读取设备号
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t getDeviceInfo();
  
  /**
   * @brief 设置LED灯
   * @param mode:in typedef enum eLED_MODE_t
   * @param color:in typedef enum eLED_COLOR_t
   * @param 闪烁次数
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t LEDCtrl(eLED_MODE_t mode,eLED_COLOR_t color,uint8_t blinkCount);
  
  /**
   * @brief 检测是否有手指触碰
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t detectFinger();
  
  /**
   * @brief 获取可注册首个编号
   * @return 可注册ID号
   */
  uint8_t getEmptyID();
  
  /**
   * @brief 检查ID是否已被注册
   * @return 1(已注册) or 0(未注册)
   */
  uint8_t getStatusID(uint8_t ID);
  
  /**
   * @brief 获取注册用户数量
   * @return 注册用户数量
   */
  uint8_t getEnrollCount();
  
  /**
   * @brief 设置指纹采集次数
   * @param 采集次数:1-3
   */
   void setCollectNumber(uint8_t number);
  
  /**
   * @brief 获取指纹
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t generate();
  
  /**
   * @brief 保存指纹
   * @param 指纹ID
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t storeChar(uint8_t ID);
  
  /**
   * @brief 删除指纹
   * @param 指纹ID or DELALL(全部删除)
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t delChar(uint8_t ID);
  
  /**
   * @brief 将指纹与全部指纹匹配
   * @return 匹配成功的指纹ID or 0(defeated)
   */
  uint8_t search();

  /**
   * @brief 将指纹与指定指纹匹配
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t verify(uint8_t ID);
  
  /**
   * @brief 进入休眠状态
   * @return 1(succeed) or 0(defeated)
   */
  uint8_t enterStandbyState();
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32        |      √       |              |             | 
micro:bit        |      √       |              |             | 


## History

- Data 2019-7-19
- Version V0.1


## Credits

Written by(yufeng.luo@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





