/*!
 * @file DFRobot_ID809.cpp
 * @brief 定义DFRobot_ID809类的基础结构和底层方法的实现
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Eddard](eddard.liu@dfrobot.com)
 * @version  V1.0
 * @date  2020-03-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_IIC_Serial
 */

#include <DFRobot_ID809.h>
#include <Arduino.h>
#include <string.h>
#include <stdio.h>
Stream *dbg=NULL;
uint16_t _PacketSize = 0;

DFRobot_ID809::DFRobot_ID809()
  :s(NULL){
		
}

DFRobot_ID809::~DFRobot_ID809(){
		
}

bool DFRobot_ID809::begin(Stream &s_){
    s = &s_;
    if(s == NULL){
        return false;
    }
    return true;
}

uint8_t DFRobot_ID809::testConnection(){
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_TEST_CONNECTION, NULL, 0);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::setDeviceID(uint8_t deviceID){
	uint8_t data[5] = {0};
	data[1] = deviceID;
	uint8_t ret = setParam(data);
	return ret;
}

uint8_t DFRobot_ID809::setSecurityLevel(uint8_t securityLevel){
	uint8_t data[5] = {0};
	data[0] = 1;
	data[1] = securityLevel;
	uint8_t ret = setParam(data);
	return ret;
}

uint8_t DFRobot_ID809::setDuplicationCheck(uint8_t duplicationCheck){
	uint8_t data[5] = {0};
	data[0] = 2;
	data[1] = duplicationCheck;
	uint8_t ret = setParam(data);
	return ret;
}

uint8_t DFRobot_ID809::setBaudrate(eDEVICE_BAUDRATE_t baudrate){
	uint8_t data[5] = {0};
	data[0] = 3;
	data[1] = baudrate;
	uint8_t ret = setParam(data);
	return ret;
}

uint8_t DFRobot_ID809::setAutoLearn(uint8_t autoLearn){
	uint8_t data[5] = {0};
	data[0] = 4;
	data[1] = autoLearn;
	uint8_t ret = setParam(data);
	return ret;
}

uint8_t DFRobot_ID809::getDeviceID(){
	uint8_t data[1];
	data[0] = 0;
	uint8_t ret = getParam(data);
	return ret;
}

uint8_t DFRobot_ID809::getSecurityLevel(){
	uint8_t data[1];
	data[0] = 1;
	uint8_t ret = getParam(data);
	return ret;
}

uint8_t DFRobot_ID809::getDuplicationCheck(){
	uint8_t data[1];
	data[0] = 2;
	uint8_t ret = getParam(data);
	return ret;
}

uint8_t DFRobot_ID809::getBaudrate(){
	uint8_t data[1];
	data[0] = 3;
	uint8_t ret = getParam(data);
	return ret;
}

uint8_t DFRobot_ID809::getAutoLearn(){
	uint8_t data[1];
	data[0] = 4;
	uint8_t ret = getParam(data);
	return ret;
}

String DFRobot_ID809::getDeviceInfo(){
	char *data;
	uint8_t result;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_DEVICE_INFO, NULL, 0);
	//LDBG("\r\n");//保证发送包是正确的
	sendPacket(header);
	free(header);
	result = responsePayload(buf);
	//LDBG("result=");LDBG(result);
	uint16_t dataLen = buf[0]+(buf[1]<<8)+1;
	if((data = (char *)malloc(dataLen)) == NULL){
		LDBG("no memory!!!\r\n");
		while(1);
	}
	data[dataLen] = 0;
	result = responsePayload(data);
	//LDBG("result=");LDBG(result);//2个包都是正确的
	String ret=String(data);
	free(data);
	return ret;
}


uint8_t DFRobot_ID809::setModuleSN(uint8_t* SN){
	uint8_t data[2];
	uint8_t ret;
	data[0] = 16;
	if(strlen(SN) > 16){
		LDBG("SN号超过15位");
		return 0;
	}
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_SET_MODULE_SN, data, 2);
	sendPacket(header);
	free(header);
	ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret != 0){
		pCmdPacketHeader_t header = pack(DATATYPE, CMD_SET_MODULE_SN, SN, 16);
		sendPacket(header);
		free(header);
		ret = responsePayload(buf);
		LDBG("ret=");LDBG(ret);
	}
	return ret;
}

String DFRobot_ID809::getModuleSN(){
	char *data;
	uint8_t result;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_MODULE_SN, NULL, 0);
	//LDBG("\r\n");//保证发送包是正确的
	sendPacket(header);
	free(header);
	result = responsePayload(buf);
	//LDBG("result=");LDBG(result);
	uint16_t dataLen = buf[0]+(buf[1]<<8)+1;
	if((data = (char *)malloc(dataLen)) == NULL){
		LDBG("no memory!!!\r\n");
		while(1);
	}
	data[dataLen] = 0;
	result = responsePayload(data);
	//LDBG("result=");LDBG(result);//2个包都是正确的
	String ret=String(data);
	free(data);
	return ret;
}

uint8_t DFRobot_ID809::LEDCtrl(eLED_MODE_t mode,eLED_COLOR_t color,uint8_t blinkCount){
	uint8_t data[4] = {0};
	data[0] = mode;
	data[2] = data[1] = color;
	data[3] = blinkCount;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_SLED_CTRL, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::detectFinger(){
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_FINGER_DETECT, NULL, 0);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == 0){
		ret = buf[0];
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::getEmptyID(){
	uint8_t data[4] = {0};
	data[0] = 1;
	data[2] = 80;     //该模块最多80个指纹，默认全部范围
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_EMPTY_ID, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == 0){
		ret = buf[0];
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::getStatusID(uint8_t ID){
	uint8_t data[2] = {0};
	data[0] = ID;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_STATUS, data, 2);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == 0){
		ret = buf[0];
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::getEnrollCount(){
	uint8_t data[4] = {0};
	data[0] = 1;
	data[2] = 80;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_ENROLL_COUNT, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == 0){
		ret = buf[0];
	}
	free(header);
	return ret;
}

#define  getID(A, V)  (A[0 + V/8] & (0x01 << (V & 0x07)))
String DFRobot_ID809::getEnrolledIDList()
{
	char *data;
	char IDList[80] = {0};
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_ENROLLED_ID_LIST, NULL, 0);
	//LDBG("\r\n");//保证发送包是正确的
	sendPacket(header);
	free(header);
	uint8_t result = responsePayload(buf);
	//LDBG("result=");LDBG(result);
	uint16_t dataLen = buf[0]+(buf[1]<<8)+1;
	if((data = (char *)malloc(dataLen)) == NULL){
		LDBG("no memory!!!\r\n");
		while(1);
	}
	data[dataLen] = 0;
	result = responsePayload(data);
	//LDBG("result=");LDBG(result);//2个包都是正确的
	uint8_t j = 0;
	for(uint8_t i = 0; i < (dataLen*8); i++){
		if(getID(data, i) != 0){
			IDList[j] = i;
			j++;
		}
	}
	String ret=String(IDList);
	free(data);
	return ret;
}

uint8_t DFRobot_ID809::storeFingerprint(uint8_t ID){
	uint8_t data[4] = {0};
	uint8_t ret = merge();
	_number = 0;
	data[0] = ID;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_STORE_CHAR, data, 4);
	sendPacket(header);
	ret = responsePayload(buf);
	if(ret == 0x18){
		LDBG("该指纹已注册");
		ret = buf[0];
	}
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
	
}

uint8_t DFRobot_ID809::delFingerprint(uint8_t ID){
	uint8_t data[4] = {0};
	if(ID == DELALL){
		data[0] = 1;
		data[2] = 80;
	}else{
		data[0] = data[2] = ID;
	}
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_DEL_CHAR, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::search(){
	uint8_t data[6] = {0};
	data[2] = 1;
	data[4] = 80;
	_number = 0;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_SEARCH, data, 6);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	if(ret == 0){
		ret = buf[0];
	}else{
		LDBG("ret=");LDBG(ret);
		ret = 0;
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::verify(uint8_t ID){
	uint8_t data[4] = {0};
	data[0] = ID;
	_number = 0;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_VERIFY, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	if(ret == 0){
		ret = buf[0];
	}else{
		LDBG("ret=");LDBG(ret);
		ret = 0;
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::match(uint8_t RamBufferID0, uint8_t RamBufferID1){
	uint8_t data[4] = {0};
	data[0] = RamBufferID0;
	data[2] = RamBufferID1;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_MATCH, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == 0)
	{
		ret = buf[0];
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::getBrokenQuantity(){
	uint8_t data[4] = {0};
	data[0] = 1;
	data[2] = 80;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_BROKEN_ID, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == 0)
	{
		ret = buf[0];
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::getBrokenID(){
	uint8_t data[4] = {0};
	data[0] = 1;
	data[2] = 80;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_BROKEN_ID, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == 0)
	{
		ret = buf[0];
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::loadChar(uint8_t ID, uint8_t RamBufferID){
	uint8_t data[4] = {0};
	data[0] = ID;
	data[2] = RamBufferID;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_LOAD_CHAR, data, 4);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::enterStandbyState(){
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_ENTER_STANDBY_STATE, NULL, 0);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
}
/**********************************************************************/

uint8_t DFRobot_ID809::setParam(uint8_t* data){
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_SET_PARAM, data, 5);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::getParam(uint8_t* data){
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_PARAM, data, 1);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	if(ret == ERR_SUCCESS){
		ret = buf[0];
	}
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::getImage(){
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GET_IMAGE, NULL, 0);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
}

uint8_t DFRobot_ID809::fingerprintCollection(uint16_t timeout){  //采集指纹
	uint8_t ret;
	uint16_t i = 0;
	while(!detectFinger()){
		delay(10);
		if(++i > timeout*10){
			ret = ERR_TIME_OUT;
			LDBG("采集超时");
			LDBG("ret=");LDBG(ret);
			return ret;
		}
	}
	ret = getImage();
	LDBG("ret=");LDBG(ret);
	if(_number > 2){
		ret = ERR_GATHER_OUT;
		LDBG("超过采集次数上限");
	}
	if(ret == 0){
		ret = generate(_number);
		_number++;
		LDBG("ret=");LDBG(ret);
	}
	return ret;
}

uint8_t DFRobot_ID809::generate(uint8_t RamBufferID){
	uint8_t ret;
	uint8_t data[2] = {0};
	data[0] = RamBufferID;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_GENERATE, data, 2);
	sendPacket(header);
	ret = responsePayload(buf);
	free(header);
	LDBG("ret=");LDBG(ret);
	return ret;
}

uint8_t DFRobot_ID809::merge(){
	uint8_t data[3] = {0};
	data[2] = _number;
	pCmdPacketHeader_t header = pack(CMDTYPE, CMD_MERGE, data, 3);
	sendPacket(header);
	uint8_t ret = responsePayload(buf);
	LDBG("ret=");LDBG(ret);
	free(header);
	return ret;
}

pCmdPacketHeader_t DFRobot_ID809::pack(uint8_t type, uint16_t cmd, const char *payload, uint16_t len){
	pCmdPacketHeader_t header;
	uint16_t cks=0;
	uint16_t dataLen;
	if(type == CMDTYPE){
		if((header = (pCmdPacketHeader_t)malloc(sizeof(sCmdPacketHeader_t)+16+2)) == NULL){
			//LDBG("\r\n");
			return NULL;
		}
		header->PREFIX = CMD_PREFIX_CODE;
		for(int i=0;i<16;i++){
			header->payload[i] = 0;
		}
		dataLen = 16;
	}else{
		if((header = (pCmdPacketHeader_t)malloc(sizeof(sCmdPacketHeader_t)+len+2)) == NULL){
			//LDBG("\r\n");
			return NULL;
		}
		header->PREFIX = CMD_DATA_PREFIX_CODE;
		dataLen = len;
	}
	header->SID = 0;
	header->DID = 0;
	header->CMD = cmd;
	header->LEN = len;
	if(len){
		memcpy((const char *)header->payload, payload, len);
	}
	cks = getCmdCKS(header);
	memcpy(&header->payload[dataLen],&cks,2);
	_PacketSize = sizeof(sCmdPacketHeader_t) + dataLen +2;
	return header;
}


void DFRobot_ID809::sendPacket(pCmdPacketHeader_t header){
	s->write((uint8_t *)header,_PacketSize);
}

uint8_t DFRobot_ID809::responsePayload(void* buf){
	sRcmPacketHeader_t header;
	uint16_t dataLen,dataCount,cks;
	uint8_t ch,ret;
	int16_t type;
	type = readPrefix(&header);
	if(type < 0){
		LDBG("--readPacketPrefix error---");
		return 0;
	}
	pRcmPacketHeader_t packet;
	if(type == RCMTYPE){
		packet = (pRcmPacketHeader_t)malloc(sizeof(sRcmPacketHeader_t)+14+2);
		dataLen = 14+2;
		if(packet == NULL){
			//LDBG("");
			while(1);
		}
	}else{
		packet = (pRcmPacketHeader_t)malloc(sizeof(sRcmPacketHeader_t)+header.LEN);
		dataLen = header.LEN;
		if(packet == NULL){
			//LDBG("");
			while(1);
		}
	}
	memcpy(packet, &header, 10);
    dataCount = readN(packet->payload, dataLen);
	cks = packet->payload[dataLen-2]+(packet->payload[dataLen-1]<<8);
	ret = (header.RET&0xFF);
	if(ret != 0){
		LDBG("--ERR CODE---");
	}
    if(dataLen != dataCount){
		LDBG("--recvRspPacket length error---");
		ret = ERR_RECV_LENGTH;
	}else if(getRcmCKS(packet) != cks){
		LDBG("--recvRspPacket cks error---");
		ret = ERR_RECV_CKS;
	}else{
		//LDBG("--recvRspPacket OK---");
		memcpy(buf, packet->payload, dataLen);
	}
	free(packet);
	packet = NULL;
	return ret;
}

uint16_t DFRobot_ID809::readPrefix( pRcmPacketHeader_t header ){
	uint8_t ch,ret;
	typedef enum{
		RECV_HEADER_INIT,
		RECV_HEADER_AA,
		RECV_HEADER_A5,
		RECV_HEADER_OK
	}eRecvHeaderState;
	eRecvHeaderState state = RECV_HEADER_INIT;
	while(state != RECV_HEADER_OK){
		if(readN(&ch, 1) != 1){
			ret = 1;
			break;
		}
		if((ch == 0xAA) && (state == RECV_HEADER_INIT)){
			state = RECV_HEADER_AA;
			continue;
		}else if((ch == 0xA5) && (state == RECV_HEADER_INIT)){
			state = RECV_HEADER_A5;
			continue;
		}else if((ch == 0x55) && (state == RECV_HEADER_AA)){
			state = RECV_HEADER_OK;
			ret = RCMTYPE;
			continue;
		}else if((ch == 0x5A) && (state == RECV_HEADER_A5)){
			state = RECV_HEADER_OK;
			ret = DATATYPE;
			continue;
		}else{
			state = RECV_HEADER_INIT;
			if(ch == 0xAA){
				state = RECV_HEADER_AA;
			}else if(ch == 0xA5){
				state = RECV_HEADER_A5;
			}
		}
	}
	if(ret == RCMTYPE){
		header->PREFIX = RCM_PREFIX_CODE;
	}else if(ret == DATATYPE){
		header->PREFIX = RCM_DATA_PREFIX_CODE;
	}
	readN(&header->SID, 1);
    readN(&header->DID, 1);
    readN(&header->RCM, 2);
    readN(&header->LEN, 2);
	readN(&header->RET, 2);
	return ret;
}

size_t DFRobot_ID809::readN(void* buffer, size_t len){
    size_t offset = 0,left = len;
	uint8_t *buf = (uint8_t*)buffer;
	long long curr = millis();
	while(left){
		if(s->available()){
			buf[offset++] = s->read();
		    left--;
		}
		if(millis() - curr > 5000){
			LDBG("----------!!!!!!!!!recv timeout----------");
			break;
		}
	}
	return offset;
}

uint16_t DFRobot_ID809::getCmdCKS(pCmdPacketHeader_t packet){
	uint16_t cks = 0xFF;
	cks += packet->SID;
	cks += packet->DID;
	cks += packet->CMD&0xFF;
	cks += packet->CMD>>8;
	cks += packet->LEN&0xFF;
	cks += packet->LEN>>8;
	if(packet->LEN > 0){
		uint8_t *p = packet->payload;
		for(uint8_t i = 0; i < packet->LEN; i++){
			cks += p[i];
		}
	}
	return cks&0xFFFF;
}

uint16_t DFRobot_ID809::getRcmCKS(pRcmPacketHeader_t packet){
	uint16_t cks = 0xFF;
	cks += packet->SID;
	cks += packet->DID;
	cks += packet->RCM&0xFF;
	cks += packet->RCM>>8;
	cks += packet->LEN&0xFF;
	cks += packet->LEN>>8;
	cks += packet->RET&0xFF;
	cks += packet->RET>>8;
	if(packet->LEN > 0){
		uint8_t *p = packet->payload;
		for(uint8_t i = 0; i < packet->LEN-2; i++){
			cks += p[i];
		}
	}
	return cks&0xFFFF;
}


