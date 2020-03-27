# DFRobot_ID809
该电容指纹模块集指纹采集、处理、存储及指纹比对为一体。
以ID809高性能处理器和半导体指纹传感器为核心，内置 IDfinger6.0指纹算法，能够独立完成全部的指纹识别工作。
该模块采用标准UART通讯，配合Arduino库，非常容易实现指纹录入、图像处理、模板生成、指纹比对等所有指纹识别功能。
该模块外观精致轻薄一体化，具有环形炫酷呼吸灯，指纹识别速度快、安全性高，支持360度任意角度识别、深度自学习功能、高性能、低功耗。
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

1. 注册指纹 <br>
2. 匹配指纹 <br>
3. 删除指纹 <br>
4. 支持休眠模式 <br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @brief 测试模块是否正常连接
   * @return true or false
   */
  bool isConnected();
  
  /**
   * @brief 设置模块ID
   * @param ID号:1-255
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t setDeviceID(uint8_t deviceID);
  
  /**
   * @brief 设置模块安全等级
   * @param 安全等级:1-5
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t setSecurityLevel(uint8_t securityLevel);
  
  /**
   * @brief 设置模块指纹重复检查(在保存指纹时，检查是否已被注册)
   * @param 1(ON) or 0(OFF)
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t setDuplicationCheck(uint8_t duplicationCheck);
  
  /**
   * @brief 设置模块波特率
   * @param Baudrate:in typedef enum eDEVICE_BAUDRATE_t
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t setBaudrate(eDeviceBaudrate_t baudrate);
  
  /**
   * @brief 设置模块自学功能(在对比指纹成功时，更新指纹)
   * @param 1(ON) or 0(OFF)
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t setAutoLearn(uint8_t autoLearn);
  
  /**
   * @brief 读取模块ID
   * @return ID号:1-255 or ERR_ID809
   */
  uint8_t getDeviceID();
  
  /**
   * @brief 读取模块安全等级
   * @return 安全等级:1-5 or ERR_ID809
   */
  uint8_t getSecurityLevel();
  
  /**
   * @brief 读取模块指纹重复检查状态
   * @return 状态：1(ON)、0(OFF) or ERR_ID809
   */
  uint8_t getDuplicationCheck();
  
  /**
   * @brief 读取模块波特率
   * @return Baudrate:in typedef enum eDEVICE_BAUDRATE_t or ERR_ID809
   */
  uint8_t getBaudrate();
  
  /**
   * @brief 读取模块自学功能状态
   * @return 状态：1(ON)、0(OFF) or ERR_ID809
   */
  uint8_t getAutoLearn();
   
  /**
   * @brief 读取设备号
   * @return 设备号
   */
  String getDeviceInfo();
  
  /**
   * @brief 设置序列号
   * @param 字符串指针
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t setModuleSN(uint8_t* SN);
  /**
   * @brief 读取序列号
   * @return 序列号
   */
  String getModuleSN();
  
  /**
   * @brief 设置LED灯
   * @param mode:in typedef enum eLED_MODE_t
   * @param color:in typedef enum eLED_COLOR_t
   * @param blink Count 0表示一直呼吸、闪烁，该参数仅在eBreathing、eFastBlink、eSlowBlink模式下有效
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t ctrlLED(eLEDMode_t mode,eLEDColor_t color,uint8_t blinkCount);
  
  /**
   * @brief 检测是否有手指触碰
   * @return 1(有手指) or 0(无手指)
   */
  uint8_t detectFinger();
  
  /**
   * @brief 获取可注册首个编号
   * @return 可注册ID号 or Error Code
   */
  uint8_t getEmptyID();
  
  /**
   * @brief 检查ID是否已被注册
   * @return 0(已注册)、1(未注册) or ERR_ID809
   */
  uint8_t getStatusID(uint8_t ID);
  
  /**
   * @brief 获取注册用户数量
   * @return 注册用户数量 or ERR_ID809
   */
  uint8_t getEnrollCount();
  
  /**
   * @brief 获取已注册用户列表
   * @return 0(succeed) or ERR_ID809
   */
   uint8_t getEnrolledIDList(uint8_t* buf);
  
  /**
   * @brief 采集指纹
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t collectionFingerprint(uint16_t timeout);
  
  /**
   * @brief 保存指纹
   * @param 指纹ID
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t storeFingerprint(uint8_t ID);
  
  /**
   * @brief 删除指纹
   * @param 指纹ID or DELALL(全部删除)
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t delFingerprint(uint8_t ID);
  
  /**
   * @brief 将指纹与全部指纹匹配
   * @return 匹配成功的指纹ID、0(匹配失败) or ERR_ID809
   */
  uint8_t search();

  /**
   * @brief 将指纹与指定指纹匹配
   * @return 匹配成功的指纹ID、0(匹配失败) or ERR_ID809
   */
  uint8_t verify(uint8_t ID);

  /**
   * @brief 指定两个RamBuffer的模板进行对比
   * @param RamBuffer号
   * @param RamBuffer号
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t match(uint8_t RamBufferID0, uint8_t RamBufferID1);
  
  /**
   * @brief 得到指纹损坏数量
   * @return 损坏的指纹ID号 or ERR_ID809
   */
  uint8_t getBrokenQuantity();

  /**
   * @brief 得到第一个损坏指纹ID
   * @return 损坏的指纹ID号 or ERR_ID809
   */
  uint8_t getBrokenID();
  
  /**
   * @brief 取出指纹模板，暂存到RamBuffer中
   * @param 指纹ID号
   * @param RamBuffer号 0-2
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t loadFingerprint(uint8_t ID, uint8_t RamBufferID);
  
  /**
   * @brief 进入休眠状态
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t enterStandbyState();
  
  /**
   * @brief 获取错误信息
   * @return 错误信息的文本描述
   */
  String getErrorDescription();
  
  bool setDbgSerial(Stream &s_){dbg = &s_; return true;}
protected:
   /**
   * @brief 设置参数
   * @param 数据类型+数据
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t setParam(uint8_t* data);
  
   /**
   * @brief 读取参数
   * @param 数据类型
   * @return 数据 or ERR_ID809
   */
  uint8_t getParam(uint8_t* data);
  
  /**
   * @brief 采集指纹图像
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t getImage();
   
   /**
   * @brief 将图像生成模板
   * @param Ram Buffer 编号
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t generate(uint8_t RamBufferID);
  
 /**
   * @brief 合成指纹
   * @return 0(succeed) or ERR_ID809
   */
  uint8_t merge();
  
 /**
   * @brief 打包数据帧
   * @param 数据类型：CMD_TYPE or DATA_TYPE
   * @param 命令
   * @param 数据
   * @param 长度
   * @return 数据帧
   */
  pCmdPacketHeader_t pack(uint8_t type, uint16_t cmd, const char *payload, uint16_t len);
  
 /**
   * @brief 发送数据
   * @param 数据帧
   */
  void sendPacket(pCmdPacketHeader_t header);
  
 /**
   * @brief 读字节
   * @param 用来存储数据的指针
   * @param 需要接收数据的长度
   * @return 实际接收长度
   */
  size_t readN(void* buf_, size_t len);
  
 /**
   * @brief 读帧头
   * @param 响应包帧头结构体
   * @return 响应包类型：RCM_TYPE,DATA_TYPE or 1(读取超时)
   */
  uint16_t readPrefix( pRcmPacketHeader_t header );
  
 /**
   * @brief 读数据
   * @param 用来存储数据的指针
   * @return 0(success) or ERR_ID809
   */
  uint8_t responsePayload(void* buf);
  
 /**
   * @brief 获取命令包CKS
   * @param 命令包帧
   * @return CKS
   */
  uint16_t getCmdCKS(pCmdPacketHeader_t packet);
  
 /**
   * @brief 获取响应包CKS
   * @param 响应包帧
   * @return CKS
   */
  uint16_t getRcmCKS(pRcmPacketHeader_t packet);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32        |             |        √      |             | 
micro:bit        |             |       √       |             | 


## History

- Data 2020-3-26
- Version V0.1


## Credits

Written by(Eddard.liu@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))





