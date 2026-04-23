#ifndef _LIB_STRAGE_H_
#define _LIB_STRAGE_H_

#define DEBUG_STRAGE

#include "lib_rs485.h"
#include "lib_rs485_enum.h"
class Strage{
public:
	void initStrage(void);

	LibReturnStateEnum setId(const int _id);
	int getId(void);
	LibReturnStateEnum setAdress(const int _adress);
	int getAdress(void);
	LibReturnStateEnum setLength(const int _length);
	int getLength(void);

	//void initSerialRam(void);
	void initMotorSerial(void);
	//int getSerialRam(const SerialRamEnum target);
	int getMotorSerial(const MotorSerialEnum target);
	//LibReturnStateEnum setSerialRam(const SerialRamEnum target,const int source);
	LibReturnStateEnum setMotorSerial(const MotorSerialEnum target,const int source);
	//LibBoolEnum getMotorEtcFlag(const SerialRamEtcEnum source);
	LibBoolEnum getMotorEtcFlag(const MotorSerialEtcEnum source);

//	void initSerialRom(void);
//	int getSerialRom(const SerialRomEnum target);
//	LibReturnStateEnum setSerialRom(const SerialRomEnum target,const int source);
	LibBoolEnum getMotorSensorActivateFlag(const MotorSerialSetSensorEnum source);
protected:
	LibReturnStateEnum setByteBit(int *target1,const int target2,const int source);
	int getByteBit(const int target1,const int target2);
	LibReturnStateEnum setLongData(int *s,const int target1,const int target2,const int source);
	int getData(int *s,const int length,const int target);
	LibReturnStateEnum setData(int *s,const int length,const int target,const int source);
	int pMotor_serial[MOTOR_SERIAL_LENGTH] = {0};

private:
	int id = DEFAULT_ID;
	int adress = 0;
	int length = 0;
};



class SendStrage : public Strage{
public:
	SendStrage(Rs485 *_pSerial);
	void initSendStrage(void);
	LibReturnStateEnum addDataCount(const int _count);
	void setDataCount(const int _count);
	void initDataCount(void);
	int getDataCount(void);
	LibReturnStateEnum addSendData(const int *_pData,const int _size);
	void initSendData(void);
	LibReturnStateEnum setSendData(const int _target,const int _source);
	LibReturnStateEnum addLongPacket(void);
	LibReturnStateEnum setShortPacket(void);
	LibReturnStateEnum sendStrageData(const uint32_t _timeout);
	void initShortPacketFlag(void);
	void initLongPacketFlag(void);
	LibBoolEnum getShortPacketFlag(const SerialShortPacketFlagEnum _flag);
	LibBoolEnum getLongPacketFlag(const SerialLongPacketFlagEnum _flag);
	LibReturnStateEnum setShortPacketFlag(const SerialShortPacketFlagEnum _target,const LibBoolEnum _source);
	LibReturnStateEnum setLongPacketFlag(const SerialLongPacketFlagEnum _target,const LibBoolEnum _source);
private:
	int send_data[SEND_DATA_LEN] = {0};
	int data_count = DEFAULT_DATA_COUNT;//start from data
	Rs485 *pSerial;
	int short_flag = 0;
	int long_flag = 0;
};

class ReceiveStrage : public Strage{
public:
	ReceiveStrage(Rs485 *_pSerial);
	void initReceiveStrage(void);
	void initReturnPacketFlag(void);
	LibBoolEnum getReturnPacketFlag(const SerialReturnPacketFlagEnum _flag);
	LibReturnStateEnum setReturnPacketFlag(const SerialReturnPacketFlagEnum _target,const LibBoolEnum _source);

private:
	Rs485 *pSerial;
	int return_flag = 0;
};
#endif
