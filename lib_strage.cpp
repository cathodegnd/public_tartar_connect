#include "lib_strage.h"



////////////////////////////////////////send strage////////////////////////////////////////////////////
void SendStrage::initSendStrage(void){
	initStrage();
	initSendData();
	pSerial->initRs485();
}
SendStrage::SendStrage(Rs485 *_pSerial){
	pSerial = _pSerial;
	initStrage();
}

LibReturnStateEnum SendStrage::addDataCount(const int _count){
	if(data_count + _count >= SEND_DATA_LEN){
		initDataCount();
		return LIB_ERROR;
	}
	data_count += _count;
	return LIB_OK;
}

void SendStrage::setDataCount(const int _count){
	data_count = _count;
}
void SendStrage::initDataCount(void){
	data_count = DEFAULT_DATA_COUNT;
}
int SendStrage::getDataCount(void){
	return data_count;
}

LibReturnStateEnum SendStrage::addSendData(const int *_pData,const int _size){
  int _send_size = getDataCount();
  LibReturnStateEnum _flag = addDataCount(_size);
  if(_flag == LIB_ERROR)return LIB_ERROR;
  for(int i = 0; i < _size; i++){
    send_data[_send_size+i] = _pData[i];
  }
  return LIB_OK;
}

void SendStrage::initSendData(void){
	for(int i = 0; i < SEND_DATA_LEN; i++){
		send_data[i] = 0;
	}
	initDataCount();
}

LibReturnStateEnum SendStrage::setSendData(const int _target,const int _source){
	send_data[_target] = _source;
}

LibReturnStateEnum SendStrage::addLongPacket(void){
	static int bord_count = 0;
	if(getDataCount() == DEFAULT_DATA_COUNT){
		addDataCount(1);
		bord_count = 0;
	}
	bord_count++;
	setSendData(DEFAULT_DATA_COUNT,bord_count);
	int id = getId();
	int adress = getAdress();
	int length = getLength();
	setSendData(0,id);//todo:変更に対応しやすくする
	setSendData(1,short_flag);
	setSendData(2,adress);
	setSendData(3,length);
	int data = 0;
	LibReturnStateEnum add_flag = LIB_OK;
	for(int i = adress; i < adress + length && add_flag == LIB_OK; i++){

		data = getMotorSerial((MotorSerialEnum)i);
		add_flag = addSendData(&data,1);
	}
	initMotorSerial();
	setId(DEFAULT_ID);
	return add_flag;
}

LibReturnStateEnum SendStrage::setShortPacket(void){
	initSendData();
	int id = getId();
	int adress = getAdress();
	int length = getLength();
	setSendData(0,id);//todo:変更に対応しやすくする
	setSendData(1,short_flag);
	setSendData(2,adress);
	setSendData(3,length);
	int data = 0;
	LibReturnStateEnum add_flag = LIB_OK;
	for(int i = adress; i < adress + length && add_flag == LIB_OK; i++){

		data = getMotorSerial((MotorSerialEnum)i);
		add_flag = addSendData(&data,1);
	}
	initMotorSerial();
	setId(DEFAULT_ID);
	return add_flag;
}

LibReturnStateEnum SendStrage::sendStrageData(const uint32_t _timeout){
//		for(int i = 0; i < getDataCount() + 1; i++){
//			Serial.print(" ");
//			Serial.print(send_data[i]);
//
//		}
//		Serial.println();/////////////////////////////////////////////////////////////////////////////////////////////////////
	pSerial->setSendDataWithCobsAndChecksum(send_data,getDataCount());
	pSerial->sendData(_timeout);
	initSendData();
}

void SendStrage::initShortPacketFlag(void){
	short_flag = 0;
}

void SendStrage::initLongPacketFlag(void){
	long_flag = 0;
}

LibBoolEnum SendStrage::getShortPacketFlag(const SerialShortPacketFlagEnum _flag){
	return (LibBoolEnum)getByteBit(short_flag,(int)_flag);
}

LibBoolEnum SendStrage::getLongPacketFlag(const SerialLongPacketFlagEnum _flag){
	return (LibBoolEnum)getByteBit(long_flag,(int)_flag);
}

LibReturnStateEnum SendStrage::setShortPacketFlag(const SerialShortPacketFlagEnum _target,const LibBoolEnum _source){
	return setByteBit(&short_flag,(int)_target,(int)_source);
}

LibReturnStateEnum SendStrage::setLongPacketFlag(const SerialLongPacketFlagEnum _target,const LibBoolEnum _source){
	return setByteBit(&long_flag,(int)_target,(int)_source);
}

////////////////////////////////////////receive strage////////////////////////////////////////////////////
ReceiveStrage::ReceiveStrage(Rs485 *_pSerial){
	pSerial = _pSerial;
}

void ReceiveStrage::initReceiveStrage(void){
	pSerial->initRs485();
}

void ReceiveStrage::initReturnPacketFlag(void){
	return_flag = 0;
}

LibBoolEnum ReceiveStrage::getReturnPacketFlag(const SerialReturnPacketFlagEnum _flag){
	return (LibBoolEnum)getByteBit(return_flag,(int)_flag);
}

LibReturnStateEnum ReceiveStrage::setReturnPacketFlag(const SerialReturnPacketFlagEnum _target,const LibBoolEnum _source){
	return setByteBit(&return_flag,(int)_target,(int)_source);
}
////////////////////////////////////////strage////////////////////////////////////////////////////

void Strage::initStrage(void){
	initMotorSerial();
	setId(DEFAULT_ID);
	setAdress(0);
	setLength(0);
}


////////////other////////////
LibReturnStateEnum Strage::setByteBit(int *target1,const int target2,const int source){
	if(target2 > 7 || source > 1 || source < 0){
		return LIB_ERROR;
	}
	if(source == 0){
		*target1 &= ~(0b1 << target2);
	}else{
		*target1 |= 0b1 << target2;
	}
	return LIB_OK;
}

int Strage::getByteBit(const int target1,const int target2){
	if(target2 > 7){
		return INT_ERROR;
	}
	return (target1 >> target2) & 0b1;
}

LibReturnStateEnum Strage::setLongData(int *s,const int target1,const int target2,const int source){
	if(target1 > target2 || target1 < 0 || target2 < 0){
		return LIB_ERROR;
	}
	for(int i = target1; i <= target2; i++){
		s[i] = source;
	}
	return LIB_OK;
}

int Strage::getData(int *s,const int length,const int target){
	if(target >= length || target < 0){
		return INT_ERROR;
	}
	return s[target];
}

LibReturnStateEnum Strage::setData(int *s,const int length,const int target,const int source){

	if(target >= length || target < 0){
		return LIB_ERROR;
	}
	if(source > 255 || 0 > source){
		s[target] = 0;
		return LIB_ERROR;
	}
	s[target] = source;
	return LIB_OK;
}

////////////getter/setter////////////
////////////id////////////
LibReturnStateEnum Strage::setId(const int _id){
	if(_id > 255 || _id < 0){
		id = 0;
		return LIB_ERROR;
	}
	id = _id;
	return LIB_OK;
}

int Strage::getId(void){
	return id;
}
////////////adress////////////
LibReturnStateEnum Strage::setAdress(const int _adress){
	if(_adress > 255 || _adress < 0){
		adress = 0;
		return LIB_ERROR;
	}
	adress = _adress;
	return LIB_OK;
}

int Strage::getAdress(void){
	return adress;
}
////////////length////////////
LibReturnStateEnum Strage::setLength(const int _length){
	if(_length > 255 || _length < 0){
		length = 0;
		return LIB_ERROR;
	}
	length = _length;
	return LIB_OK;
}

int Strage::getLength(void){
	return length;
}

////////////ram////////////

void Strage::initMotorSerial(void){
	setLongData(pMotor_serial, 0, MOTOR_SEND_SERIAL_LENGTH - 1, 0);
}

int Strage::getMotorSerial(const MotorSerialEnum target){
	return getData(pMotor_serial, MOTOR_SERIAL_LENGTH, (int)target);
}

LibReturnStateEnum Strage::setMotorSerial(const MotorSerialEnum target,const int source){
	setData(pMotor_serial, MOTOR_SERIAL_LENGTH, (int)target, source);
	return LIB_OK;
}

LibBoolEnum Strage::getMotorEtcFlag(const MotorSerialEtcEnum source){
	int bit_count = (int)source;
	switch(bit_count / 8){
	case 0:
		if(((getMotorSerial(M1_ETC) >> bit_count) & 0b1) == 1){
			return LIB_TRUE;
		}
		return LIB_FALSE;
		break;
	case 1:
		if(((getMotorSerial(M2_ETC) >> (bit_count - 8)) & 0b1) == 1){
			return LIB_TRUE;
		}
		return LIB_FALSE;
		break;
	case 2:
		if(((getMotorSerial(M3_ETC) >> (bit_count - 16)) & 0b1) == 1){
			return LIB_TRUE;
		}
		return LIB_FALSE;
		break;
	case 3:
		if(((getMotorSerial(M4_ETC) >> (bit_count - 24)) & 0b1) == 1){
			return LIB_TRUE;
		}
		return LIB_FALSE;
		break;
	default:
		break;
	}
	return LIB_FALSE;
}

LibBoolEnum Strage::getMotorSensorActivateFlag(const MotorSerialSetSensorEnum source){
	if(((getMotorSerial(SET_SENSOR) >> (int)source) & 0b1) == 1){
		return LIB_TRUE;
	}
	return LIB_FALSE;
}
