/*!
 * @file DFRobot_ID809.h
 * @brief 定义 DFRobot_ID809 类的基础结构
 * @n 这是一个电容指纹识别模块的库,其主要
 * @n 功能是采集手指图像、对比指纹、删除指纹等
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_ID809
 */

#ifndef _DFRobot_ID809_H
#define _DFRobot_ID809_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>

#include <stdint.h>

//Open this macro to see the detailed running process of the program 

//#define ENABLE_DBG
#ifdef ENABLE_DBG
#define LDBG(...) if(dbg) {dbg->print("["); dbg->print(__FUNCTION__); dbg->print("(): "); dbg->print(__LINE__); dbg->print(" ] "); dbg->println(__VA_ARGS__);}
#else
#define LDBG(...)
#endif

extern Stream *dbg;

  /*
   命令包的帧结构
  */
typedef struct{
  uint16_t  PREFIX;
  uint8_t   SID;
  uint8_t   DID;
  uint16_t  CMD;
  uint16_t  LEN;
  uint8_t payload[0];
}__attribute__ ((packed)) sCmdPacketHeader_t, *pCmdPacketHeader_t;

  /*
   响应包的帧结构
  */
typedef struct{
  uint16_t  PREFIX;
  uint8_t   SID;
  uint8_t   DID;
  uint16_t  RCM;
  uint16_t  LEN;
  uint16_t  RET;
  uint8_t   payload[0];
}__attribute__ ((packed)) sRcmPacketHeader_t, *pRcmPacketHeader_t;

  


class DFRobot_ID809{
public: 

#define FINGERPRINT_CAPACITY     80      //模块指纹容量
#define MODULE_SN_SIZE           16      //模块SN长度


#define DELALL                   0xFF    //删除所有指纹

#define CMD_PREFIX_CODE          0xAA55  //命令包前缀代码
#define RCM_PREFIX_CODE          0x55AA  //响应包前缀代码
#define CMD_DATA_PREFIX_CODE     0xA55A  //命令数据包前缀代码
#define RCM_DATA_PREFIX_CODE     0x5AA5  //响应数据包前缀代码

#define CMD_TYPE                 0xF0    //命令包类型
#define RCM_TYPE                 0xF0    //响应包类型
#define DATA_TYPE                0x0F    //数据包类型

#define CMD_TEST_CONNECTION      0X0001  //连接测试
#define CMD_SET_PARAM            0X0002  //设置参数
#define CMD_GET_PARAM            0X0003  //读取参数
#define CMD_DEVICE_INFO          0X0004  //读取设备信息
#define CMD_SET_MODULE_SN        0X0008  //设置模块序列号
#define CMD_GET_MODULE_SN        0X0009  //读取模块序列号
#define CMD_ENTER_STANDBY_STATE  0X000C  //进入休眠状态
#define CMD_GET_IMAGE            0X0020  //采集指纹图像
#define CMD_FINGER_DETECT        0X0021  //检测手指
    #define CMD_UP_IMAGE_CODE        0X0022  //上传指纹图像到主机
    #define CMD_DOWN_IMAGE           0X0023  //下载指纹图像到模块
#define CMD_SLED_CTRL            0X0024  //控制采集器背光灯
#define CMD_STORE_CHAR           0X0040  //保存指纹模板数据到模块指纹库
#define CMD_LOAD_CHAR            0X0041  //读取模块中的指纹并暂存在 RAMBUFFER 中
    #define CMD_UP_CHAR              0X0042  //将暂存在 RAMBUFFER 中的指纹模板上传到主机
    #define CMD_DOWN_CHAR            0X0043  //下载指纹模板数据到模块指定的 RAMBUFFER
#define CMD_DEL_CHAR             0X0044  //删除指定编号范围内的指纹
#define CMD_GET_EMPTY_ID         0X0045  //获取指定编号范围内可注册的首个编号
#define CMD_GET_STATUS           0X0046  //检查指定的编号是否已被注册
#define CMD_GET_BROKEN_ID        0X0047  //检查指定范围内的指纹库是否有数据损坏
#define CMD_GET_ENROLL_COUNT     0X0048  //获取指定编号范围内已注册的指纹总数
#define CMD_GET_ENROLLED_ID_LIST 0X0049  //获取已注册 ID 列表
#define CMD_GENERATE             0X0060  //从暂存在 IMAGEBUFFER 中的指纹图像产生模板
#define CMD_MERGE                0X0061  //合成指纹模板数据用于入库
#define CMD_MATCH                0X0062  //指定2个RAMBUFFER 之间的模板做比对
#define CMD_SEARCH               0X0063  //指定编号范围的1:N识别
#define CMD_VERIFY               0X0064  //指定 RAMBUFFER 与指纹库中指定编号的模板比对

#define ERR_SUCCESS              0x00    //指令处理成功
#define ERR_ID809                0xFF    //出现错误


public:
  
  typedef enum{
    eBreathing = 1,  //呼吸
    eFastBlink,      //快闪
    eKeepsOn,        //常亮
    eNormalClose,    //常闭
    eFadeIn,         //渐开
    eFadeOut,        //渐关
    eSlowBlink       //慢闪
  }eLEDMode_t;
  
  typedef enum{
    eLEDGreen = 1,   //绿色
    eLEDRed,         //红色
    eLEDYellow,      //黄色
    eLEDBlue,        //蓝色
    eLEDCyan,        //青色
    eLEDMagenta,     //品红色
    eLEDWhite        //白色
  }eLEDColor_t;
   
  typedef enum{
    e9600bps = 1,
    e19200bps,
    e38400bps,
    e57600bps,
    e115200bps,
    e230400bps,
    e460800bps,
    e921600bps
  }eDeviceBaudrate_t;
  
  typedef enum{
    eErrorSuccess            = 0x00,    //指令处理成功
    eErrorFail               = 0x01,    //指令处理失败
    eErrorVerify             = 0x10,    //与指定编号中 Template 的 1:1 比对失败
    eErrorIdentify           = 0x11,    //已进行 1:N 比对， 但相同 Template 不存在
    eErrorTmplEmpty          = 0x12,    //在指定编号中不存在已注册的 Template 
    eErrorTmplNotEmpty       = 0x13,    //在指定编号中已存在 Template 
    eErrorAllTmplEmpty       = 0x14,    //不存在已注册的 Template 
    eErrorEmptyIDNoexist     = 0x15,    //不存在可注册的 Template ID 
    eErrorBrokenIDNoexist    = 0x16,    //不存在已损坏的 Template 
    eErrorInvalidTmplData    = 0x17,    //指定的 Template Data 无效
    eErrorDuplicationID      = 0x18,    //该指纹已注册
    eErrorBadQuality         = 0x19,    //指纹图像质量不好
    eErrorMergeFail          = 0x1A,    //Template 合成失败
    eErrorNotAuthorized      = 0x1B,    //没有进行通讯密码确认
    eErrorMemory             = 0x1C,    //外部 Flash 烧写出错
    eErrorInvalidTmplNo      = 0x1D,    //指定 Template 编号无效
    eErrorInvalidParam       = 0x22,    //使用了不正确的参数
    eErrorTimeOut            = 0x23,    //采集超时
    eErrorGenCount           = 0x25,    //指纹合成个数无效
    eErrorInvalidBufferID    = 0x26,    //Buffer ID 值不正确
    eErrorFPNotDetected      = 0x28,    //采集器上没有指纹输入
    eErrorFPCancel           = 0x41,    //指令被取消
    eErrorRecvLength         = 0x42,    //接收数据长度错误
    eErrorRecvCks            = 0x43,    //校验码错误
    eErrorGatherOut          = 0x45,    //模板采集次数超过上限
    eErrorRecvTimeout        = 0x46     //通讯超时
  }eError_t;
  
  typedef struct{
    /**< Gesture enumeration variable X */
    eError_t error;
    /**< Description about the gesture enumeration variable X */
    const char * description;
  }sErrorDescription_t;

public:
  DFRobot_ID809();
  ~DFRobot_ID809();
  
  /**
   * @brief 初始化通讯串口
   * @param 硬串口 or 软串口
   * @return true or false
   */
  bool begin(Stream &s_);
  
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
   * @param Baudrate:in typedef enum eDeviceBaudrate_t
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
  uint8_t setModuleSN(const char* SN);
  /**
   * @brief 读取序列号
   * @return 序列号
   */
  String getModuleSN();
  
  /**
   * @brief 设置LED灯
   * @param mode:in typedef enum eLEDMode_t
   * @param color:in typedef enum eLEDColor_t
   * @param blink Count: 0表示一直呼吸、闪烁，该参数仅在eBreathing、eFastBlink、eSlowBlink模式下有效
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
   * @return 可注册ID号 or ERR_ID809
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
   uint8_t getEnrolledIDList(uint8_t* list);
  
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
  
private:
  Stream *s;
  uint8_t buf[20];     //用来存储响应包的数据
  pCmdPacketHeader_t  sendHeader;
  pRcmPacketHeader_t  recHeader;
  
  static const sErrorDescription_t /*PROGMEM*/ errorDescriptionsTable[26];   //错误信息列表
  
  uint8_t _number = 0;       //指纹采集次数
  eError_t _error;           //错误码
  uint16_t _PacketSize = 0;  //需要发送的数据包长度
};

#endif


