#include "lib_ui.h"

struct LibMotorAdjust{
	int adjust_power_l;
	int adjust_power_h;
	int lower_limit_l;
	int lower_limit_h;
};

struct LibMotorEnc{
	int p_l;
	int p_h;
	int i_l;
	int i_h;
	int d_l;
	int d_h;
};

struct LibMotorCur{
	int p_l;
	int p_h;
	int i_l;
	int i_h;
	int d_l;
	int d_h;
};


struct MotorConvertData{
	int rotate_speed_l;
	int rotate_speed_h;
	int rotate_power;
	int brake_power_l;
	int brake_power_h;
	union{
		struct{
			int activate_brake:1;//flag
			int rotate_with_brake:1;//flag
			int rotate_direction:1;//flag
		};
		int etc;
	};
};

struct MotorConvertNowData{
	int now_speed_l;
	int now_speed_h;
	int now_power;
};

typedef union{
	struct{
	int shield_num;
	int firmware_version;
	int reserved1;
	int reserved2;
	union{
	struct{
		int m1_enc_direction:1;
		int m1_cur_direction:1;
		int m2_enc_direction:1;
		int m2_cur_direction:1;
		int m3_enc_direction:1;
		int m3_cur_direction:1;
		int m4_enc_direction:1;
		int m4_cur_direction:1;
	};
	int sensor_direction;
	};
	union{
	struct{
		int m1_activate_enc:1;
		int m1_activate_cur:1;
		int m2_activate_enc:1;
		int m2_activate_cur:1;
		int m3_activate_enc:1;
		int m3_activate_cur:1;
		int m4_activate_enc:1;
		int m4_activate_cur:1;
	};
	int setsensor;
	};
	struct LibMotorAdjust m1adjust;
	struct LibMotorAdjust m2adjust;
	struct LibMotorAdjust m3adjust;
	struct LibMotorAdjust m4adjust;
	struct LibMotorEnc m1enc;
	struct LibMotorEnc m2enc;
	struct LibMotorEnc m3enc;
	struct LibMotorEnc m4enc;
	struct LibMotorCur m1cur;
	struct LibMotorCur m2cur;
	struct LibMotorCur m3cur;
	struct LibMotorCur m4cur;
	int lpfk_l;
	int lpfk_h;
	int lpft_l;
	int lpft_h;
	struct MotorConvertData m1;
	struct MotorConvertData m2;
	struct MotorConvertData m3;
	struct MotorConvertData m4;
	union{
		struct{
			int led1:1;
			int led2:1;
			int led3:1;
			int serial:3;
		};
		int ui;
	};
	struct MotorConvertNowData m1now;
	struct MotorConvertNowData m2now;
	struct MotorConvertNowData m3now;
	struct MotorConvertNowData m4now;
	};
	int val[MOTOR_SERIAL_LENGTH];
}LibMotorConvertUnion;

typedef union{
	uint32_t n;
	float f;
}LibFloatInt;

typedef union{
	int32_t n32;
	int nint;
}Lib32int;

/////////////sensor ui///////////

/////////////mdd ui/////////////
MddMotor::MddMotor(const int _motor,const int _id,Ui *_pUi){
	switch(_motor){
	default:
	case 1:
		motor = M1;
		break;
	case 2:
		motor = M2;
		break;
	case 3:
		motor = M3;
		break;
	case 4:
		motor = M4;
		break;
	}
	id = _id;
	pUi = _pUi;
}

LibReturnStateEnum MddMotor::setBordFlag(const LibMotorBordFlagEnum _target,const bool _source){
	return pUi->setMotorBordFlag(id,(int)_target,_source);
}
LibReturnStateEnum MddMotor::setBordData(const LibMotorBordDataEnum _target,const float _source){
	if(_target == PRINT){
		return pUi->setMotorBordData(id,(int)_target,(int)_source);
	}
	if(_source < 0){
		return LIB_ERROR;
	}
	int val = pUi->convertFloatToTwoByte(_source);
	return pUi->setMotorBordData(id,(int)_target,val);
}

LibReturnStateEnum MddMotor::setParamFlag(const LibMotorParamFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(motor,id,(int)_target,_source);
}
LibReturnStateEnum MddMotor::setDataFlag(const LibMotorMoveDataFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(motor,id,(int)_target,_source);
}
LibReturnStateEnum MddMotor::setParam(const LibMotorParamEnum _target,const float _source){
	if(_source < 0){
		return LIB_ERROR;
	}
	if(_target == LOWER_LIMIT){
		return pUi->setMotorData(motor,id,(int)LOWER_LIMIT,(int)_source);
	}
	int val = pUi->convertFloatToTwoByte(_source);
	return pUi->setMotorData(motor,id,(int)_target,val);
}
LibReturnStateEnum MddMotor::setData(const LibMotorMoveDataEnum _target,const int32_t _source){
	if((_target == ROTATE_SPEED || _target == BRAKE_PWR) && _source > 0xFFFF || _source < 0){
		return LIB_ERROR;
	}
	Lib32int val;
	val.n32 = _source;
	return pUi->setMotorData(motor,id,(int)_target,val.nint);
}
/////////////motor ui///////////
MotorUi::MotorUi(Ui *_pUi){
	pUi = _pUi;

}

//////////////////motor 1//////////////////

LibReturnStateEnum MotorUi::m1SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M1,_id,(int)_target,_source);
}

LibReturnStateEnum MotorUi::m1SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M1,_id,(int)_target,_source);
}


LibReturnStateEnum MotorUi::m1SetParam(const int _id,const LibMotorParamEnum _target,const float _source){
	if(_source < 0){
		return LIB_ERROR;
	}
	if(_target == LOWER_LIMIT){
		return pUi->setMotorData(M1,_id,(int)LOWER_LIMIT,(int)_source);
	}
	int val = pUi->convertFloatToTwoByte(_source);
	return pUi->setMotorData(M1,_id,(int)_target,val);
}

LibReturnStateEnum MotorUi::m1SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source){
	if((_target == ROTATE_SPEED || _target == BRAKE_PWR) && _source > 0xFFFF || _source < 0){
		return LIB_ERROR;
	}
	Lib32int val;
	val.n32 = _source;
	return pUi->setMotorData(M1,_id,(int)_target,val.nint);
}

//////////////////motor 2//////////////////

LibReturnStateEnum MotorUi::m2SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M2,_id,(int)_target,_source);
}

LibReturnStateEnum MotorUi::m2SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M2,_id,(int)_target,_source);
}


LibReturnStateEnum MotorUi::m2SetParam(const int _id,const LibMotorParamEnum _target,const float _source){
	if(_source < 0){
		return LIB_ERROR;
	}
	if(_target == LOWER_LIMIT){
		return pUi->setMotorData(M2,_id,(int)LOWER_LIMIT,(int)_source);
	}
	int val = pUi->convertFloatToTwoByte(_source);
	return pUi->setMotorData(M2,_id,(int)_target,val);
}

LibReturnStateEnum MotorUi::m2SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source){
	if((_target == ROTATE_SPEED || _target == BRAKE_PWR) && _source > 0xFFFF || _source < 0){
		return LIB_ERROR;
	}
	Lib32int val;
	val.n32 = _source;
	return pUi->setMotorData(M2,_id,(int)_target,val.nint);
}

//////////////////motor 3//////////////////

LibReturnStateEnum MotorUi::m3SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M3,_id,(int)_target,_source);
}

LibReturnStateEnum MotorUi::m3SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M3,_id,(int)_target,_source);
}


LibReturnStateEnum MotorUi::m3SetParam(const int _id,const LibMotorParamEnum _target,const float _source){
	if(_source < 0){
		return LIB_ERROR;
	}
	if(_target == LOWER_LIMIT){
		return pUi->setMotorData(M3,_id,(int)LOWER_LIMIT,(int)_source);
	}
	int val = pUi->convertFloatToTwoByte(_source);
	return pUi->setMotorData(M3,_id,(int)_target,val);
}

LibReturnStateEnum MotorUi::m3SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source){
	if((_target == ROTATE_SPEED || _target == BRAKE_PWR) && _source > 0xFFFF || _source < 0){
		return LIB_ERROR;
	}
	Lib32int val;
	val.n32 = _source;
	return pUi->setMotorData(M3,_id,(int)_target,val.nint);
}

//////////////////motor 4//////////////////

LibReturnStateEnum MotorUi::m4SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M4,_id,(int)_target,_source);
}

LibReturnStateEnum MotorUi::m4SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source){
	return pUi->setMotorFlag(M4,_id,(int)_target,_source);
}


LibReturnStateEnum MotorUi::m4SetParam(const int _id,const LibMotorParamEnum _target,const float _source){
	if(_source < 0){
		return LIB_ERROR;
	}
	if(_target == LOWER_LIMIT){
		return pUi->setMotorData(M4,_id,(int)LOWER_LIMIT,(int)_source);
	}
	int val = pUi->convertFloatToTwoByte(_source);
	return pUi->setMotorData(M4,_id,(int)_target,val);
}

LibReturnStateEnum MotorUi::m4SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source){
	if((_target == ROTATE_SPEED || _target == BRAKE_PWR) && _source > 0xFFFF || _source < 0){
		return LIB_ERROR;
	}
	Lib32int val;
	val.n32 = _source;
	return pUi->setMotorData(M4,_id,(int)_target,val.nint);
}





/////////////ui/////////////////////////////////////////////////////////////////////////////////////////////////
Ui::Ui(SendStrage *_pSend,ReceiveStrage *_pReceive){
	pSend = _pSend;
	pReceive = _pReceive;
}


void Ui::init(void){
	pSend->initSendStrage();
	pReceive->initReceiveStrage();
	initBordId();
}

int Ui::convertFloatToTwoByte(const float _source){
	LibFloatInt val;
	val.f = _source;
	if(val.n << 1 == 0){
		return 0;
	}
	int sign = (val.n >> 16) & 0x8000;
	int exponent = (((val.n >> 23) - 127 + 15)& 0x1F) << 10;
	int fraction = (val.n >> (23 - 10)) & 0x3FF;
	return sign | exponent | fraction;
}

void Ui::setMotorId(const int _count,const int _id){
	motor_bord[_count].id = _id;
}

int Ui::getMotorId(const int _count){
	return motor_bord[_count].id;
}

LibReturnStateEnum Ui::addBord(const int _id,const LibBordTypeEnum _type){
	if(_id > 0xFE || _id < 0 || motor_bord_count >= MOTOR_BORD_MAX_NUM){
		return LIB_ERROR;
	}
	switch(_type){
	default:
	case LIB_NO_TYPE:
		return LIB_ERROR;
		break;
	case LIB_MDD:
		bord_id[_id] = motor_bord_count;
		setMotorId(motor_bord_count,_id);
		motor_bord_count++;
		break;
	}
	bord_type[_id] = (int)_type;
	initBordData(_id,_type);
	return LIB_OK;
}

LibReturnStateEnum Ui::sendToMotor(const int _id,const int _timeout){
	int bord_count = getBordCountFromId(_id);
	if(bord_count == INT_ERROR){
		return LIB_ERROR;
	}
	pSend->setShortPacketFlag(SHORT_INIT_RAM,LIB_FALSE);
	pSend->setShortPacketFlag(SHORT_INIT_ROM,LIB_FALSE);
	pSend->setShortPacketFlag(SHORT_READ_DATA,LIB_FALSE);
	setMotor(bord_count);
	pSend->setAdress(0);
	pSend->setLength(MOTOR_SEND_SERIAL_LENGTH);
	pSend->setShortPacket();
	pSend->sendStrageData(_timeout);
	updatePreBordData(bord_count,LIB_MDD);
	return LIB_OK;
}

LibReturnStateEnum Ui::sendChangeToMotor(const int _timeout){//TODO
/*	int pChange_ram[MOTOR_BORD_MAX_NUM] = {0};
	int pChange_rom[MOTOR_BORD_MAX_NUM] = {0};

	int change_ram_count = 0;
	int change_rom_count = 0;
	int ram_front = 0;
	int ram_back = SERIAL_RAM_LENGTH - 1;
	int rom_front = 0;
	int rom_back = SERIAL_ROM_LENGTH - 1;
	for(int i = 0; i < getBordCount(LIB_MDD);i++){
		int ram_front1 = 0;
		int ram_back1 = 0;
		int rom_front1 = 0;
		int rom_back1 = 0;
		LibReturnStateEnum ram_state = getMotorRamChengePos(&ram_front1,&ram_back1,i);
		LibReturnStateEnum rom_state = getMotorRomChengePos(&rom_front1,&rom_back1,i);
		if(ram_state == LIB_OK){
			pChange_ram[change_ram_count] = i;
			change_ram_count++;
			if(ram_front1 > ram_front){
				ram_front = ram_front1;
			}
			if(ram_back1 < ram_back){
				ram_back = ram_back1;
			}
		}
		if(rom_state == LIB_OK){
			pChange_rom[change_rom_count] = i;
			change_rom_count++;
			if(rom_front1 > rom_front){
				rom_front = rom_front1;
			}
			if(rom_back1 < rom_back){
				rom_back = rom_back1;
			}
		}
	}
	if(change_ram_count == 1){
		int bord_count = pChange_ram[0];
		pSend->setShortPacketFlag(SHORT_INIT_RAM,LIB_FALSE);
		pSend->setShortPacketFlag(SHORT_INIT_ROM,LIB_FALSE);
		pSend->setShortPacketFlag(SHORT_READ_DATA,LIB_FALSE);
		pSend->setShortPacketFlag(SHORT_TO_RAM,LIB_TRUE);
		setMotorRam(ram_front,ram_back - ram_front + 1,bord_count);
		pSend->setShortPacket();
	}else{
		for(int i = 0; i < change_ram_count; i++){
			int bord_count = pChange_ram[i];
			pSend->setLongPacketFlag(LONG_INIT_RAM,LIB_FALSE);
			pSend->setLongPacketFlag(LONG_INIT_ROM,LIB_FALSE);
			pSend->setLongPacketFlag(LONG_TO_RAM,LIB_TRUE);
			setMotorRam(ram_front,ram_back - ram_front + 1,bord_count);
			pSend->addLongPacket();
		}
	}
	pSend->sendStrageData(_timeout);
	if(change_rom_count == 1){
		int bord_count = pChange_rom[0];
		pSend->setShortPacketFlag(SHORT_INIT_RAM,LIB_FALSE);
		pSend->setShortPacketFlag(SHORT_INIT_ROM,LIB_FALSE);
		pSend->setShortPacketFlag(SHORT_READ_DATA,LIB_FALSE);
		pSend->setShortPacketFlag(SHORT_TO_RAM,LIB_FALSE);
		setMotorRom(rom_front,rom_back - rom_front + 1,bord_count);
		pSend->setShortPacket();
	}else{
		for(int i = 0; i < change_ram_count; i++){
			int bord_count = pChange_rom[i];
			pSend->setLongPacketFlag(LONG_INIT_RAM,LIB_FALSE);
			pSend->setLongPacketFlag(LONG_INIT_ROM,LIB_FALSE);
			pSend->setLongPacketFlag(LONG_TO_RAM,LIB_FALSE);
			setMotorRom(rom_front,rom_back - rom_front + 1,bord_count);
			pSend->addLongPacket();
		}
	}
	pSend->sendStrageData(_timeout);
	for(int i = 0; i < getBordCount(LIB_MDD); i++){
		updatePreBordData(i,LIB_MDD);
	}*/

}

void Ui::setMotor(int _bord_count){//todo 関数に分割して代入部分を簡単にする。
	int id = getMotorId(_bord_count);
	LibMotorConvertUnion convert;
	for(int i = 0; i < MOTOR_SERIAL_LENGTH; i++){
		convert.val[i] = 0;
	}
	convert.shield_num = SHIELD_VERSION;
	convert.firmware_version = TARTAR_VERSION;
	////////////////motor1//////////////
	convert.m1_enc_direction = getMotorFlag(M1,id,(int)ENC_DIRECTION) & 0b1;
	convert.m1_activate_enc = getMotorFlag(M1,id,(int)ACTIVATE_ENC) & 0b1;
	convert.m1_activate_cur = getMotorFlag(M1,id,(int)ACTIVATE_CUR) & 0b1;
	convert.m1adjust.adjust_power_l = (getMotorData(M1,id,(int)ADJUST_PWR)) & 0xFF;
	convert.m1adjust.adjust_power_h = (getMotorData(M1,id,(int)ADJUST_PWR)>>8) & 0xFF;
	convert.m1adjust.lower_limit_l = (getMotorData(M1,id,(int)LOWER_LIMIT)) & 0xFF;
	convert.m1adjust.lower_limit_h = (getMotorData(M1,id,(int)LOWER_LIMIT)>>8) & 0xFF;
	convert.m1enc.p_l = (getMotorData(M1,id,(int)ENC_P)) & 0xFF;
	convert.m1enc.p_h = (getMotorData(M1,id,(int)ENC_P)>>8) & 0xFF;
	convert.m1enc.i_l = (getMotorData(M1,id,(int)ENC_I)) & 0xFF;
	convert.m1enc.i_h = (getMotorData(M1,id,(int)ENC_I)>>8) & 0xFF;
	convert.m1enc.d_l = (getMotorData(M1,id,(int)ENC_D)) & 0xFF;
	convert.m1enc.d_h = (getMotorData(M1,id,(int)ENC_D)>>8) & 0xFF;
	convert.m1cur.p_l = (getMotorData(M1,id,(int)CUR_P)) & 0xFF;
	convert.m1cur.p_h = (getMotorData(M1,id,(int)CUR_P)>>8) & 0xFF;
	convert.m1cur.i_l = (getMotorData(M1,id,(int)CUR_I)) & 0xFF;
	convert.m1cur.i_h = (getMotorData(M1,id,(int)CUR_I)>>8) & 0xFF;
	convert.m1cur.d_l = (getMotorData(M1,id,(int)CUR_D)) & 0xFF;
	convert.m1cur.d_h = (getMotorData(M1,id,(int)CUR_D)>>8) & 0xFF;
	////////////////motor1//////////////
	convert.m2_enc_direction = getMotorFlag(M2,id,(int)ENC_DIRECTION) & 0b1;
	convert.m2_activate_enc = getMotorFlag(M2,id,(int)ACTIVATE_ENC) & 0b1;
	convert.m2_activate_cur = getMotorFlag(M2,id,(int)ACTIVATE_CUR) & 0b1;
	convert.m2adjust.adjust_power_l = (getMotorData(M2,id,(int)ADJUST_PWR)) & 0xFF;
	convert.m2adjust.adjust_power_h = (getMotorData(M2,id,(int)ADJUST_PWR)>>8) & 0xFF;
	convert.m2adjust.lower_limit_l = (getMotorData(M2,id,(int)LOWER_LIMIT)) & 0xFF;
	convert.m2adjust.lower_limit_h = (getMotorData(M2,id,(int)LOWER_LIMIT)>>8) & 0xFF;
	convert.m2enc.p_l = (getMotorData(M2,id,(int)ENC_P)) & 0xFF;
	convert.m2enc.p_h = (getMotorData(M2,id,(int)ENC_P)>>8) & 0xFF;
	convert.m2enc.i_l = (getMotorData(M2,id,(int)ENC_I)) & 0xFF;
	convert.m2enc.i_h = (getMotorData(M2,id,(int)ENC_I)>>8) & 0xFF;
	convert.m2enc.d_l = (getMotorData(M2,id,(int)ENC_D)) & 0xFF;
	convert.m2enc.d_h = (getMotorData(M2,id,(int)ENC_D)>>8) & 0xFF;
	convert.m2cur.p_l = (getMotorData(M2,id,(int)CUR_P)) & 0xFF;
	convert.m2cur.p_h = (getMotorData(M2,id,(int)CUR_P)>>8) & 0xFF;
	convert.m2cur.i_l = (getMotorData(M2,id,(int)CUR_I)) & 0xFF;
	convert.m2cur.i_h = (getMotorData(M2,id,(int)CUR_I)>>8) & 0xFF;
	convert.m2cur.d_l = (getMotorData(M2,id,(int)CUR_D)) & 0xFF;
	convert.m2cur.d_h = (getMotorData(M2,id,(int)CUR_D)>>8) & 0xFF;
	////////////////motor1//////////////
	convert.m3_enc_direction = getMotorFlag(M3,id,(int)ENC_DIRECTION) & 0b1;
	convert.m3_activate_enc = getMotorFlag(M3,id,(int)ACTIVATE_ENC) & 0b1;
	convert.m3_activate_cur = getMotorFlag(M3,id,(int)ACTIVATE_CUR) & 0b1;
	convert.m3adjust.adjust_power_l = (getMotorData(M3,id,(int)ADJUST_PWR)) & 0xFF;
	convert.m3adjust.adjust_power_h = (getMotorData(M3,id,(int)ADJUST_PWR)>>8) & 0xFF;
	convert.m3adjust.lower_limit_l = (getMotorData(M3,id,(int)LOWER_LIMIT)) & 0xFF;
	convert.m3adjust.lower_limit_h = (getMotorData(M3,id,(int)LOWER_LIMIT)>>8) & 0xFF;
	convert.m3enc.p_l = (getMotorData(M3,id,(int)ENC_P)) & 0xFF;
	convert.m3enc.p_h = (getMotorData(M3,id,(int)ENC_P)>>8) & 0xFF;
	convert.m3enc.i_l = (getMotorData(M3,id,(int)ENC_I)) & 0xFF;
	convert.m3enc.i_h = (getMotorData(M3,id,(int)ENC_I)>>8) & 0xFF;
	convert.m3enc.d_l = (getMotorData(M3,id,(int)ENC_D)) & 0xFF;
	convert.m3enc.d_h = (getMotorData(M3,id,(int)ENC_D)>>8) & 0xFF;
	convert.m3cur.p_l = (getMotorData(M3,id,(int)CUR_P)) & 0xFF;
	convert.m3cur.p_h = (getMotorData(M3,id,(int)CUR_P)>>8) & 0xFF;
	convert.m3cur.i_l = (getMotorData(M3,id,(int)CUR_I)) & 0xFF;
	convert.m3cur.i_h = (getMotorData(M3,id,(int)CUR_I)>>8) & 0xFF;
	convert.m3cur.d_l = (getMotorData(M3,id,(int)CUR_D)) & 0xFF;
	convert.m3cur.d_h = (getMotorData(M3,id,(int)CUR_D)>>8) & 0xFF;
	////////////////motor1//////////////
	convert.m4_enc_direction = getMotorFlag(M4,id,(int)ENC_DIRECTION) & 0b1;
	convert.m4_activate_enc = getMotorFlag(M4,id,(int)ACTIVATE_ENC) & 0b1;
	convert.m4_activate_cur = getMotorFlag(M4,id,(int)ACTIVATE_CUR) & 0b1;
	convert.m4adjust.adjust_power_l = (getMotorData(M4,id,(int)ADJUST_PWR)) & 0xFF;
	convert.m4adjust.adjust_power_h = (getMotorData(M4,id,(int)ADJUST_PWR)>>8) & 0xFF;
	convert.m4adjust.lower_limit_l = (getMotorData(M4,id,(int)LOWER_LIMIT)) & 0xFF;
	convert.m4adjust.lower_limit_h = (getMotorData(M4,id,(int)LOWER_LIMIT)>>8) & 0xFF;
	convert.m4enc.p_l = (getMotorData(M4,id,(int)ENC_P)) & 0xFF;
	convert.m4enc.p_h = (getMotorData(M4,id,(int)ENC_P)>>8) & 0xFF;
	convert.m4enc.i_l = (getMotorData(M4,id,(int)ENC_I)) & 0xFF;
	convert.m4enc.i_h = (getMotorData(M4,id,(int)ENC_I)>>8) & 0xFF;
	convert.m4enc.d_l = (getMotorData(M4,id,(int)ENC_D)) & 0xFF;
	convert.m4enc.d_h = (getMotorData(M4,id,(int)ENC_D)>>8) & 0xFF;
	convert.m4cur.p_l = (getMotorData(M4,id,(int)CUR_P)) & 0xFF;
	convert.m4cur.p_h = (getMotorData(M4,id,(int)CUR_P)>>8) & 0xFF;
	convert.m4cur.i_l = (getMotorData(M4,id,(int)CUR_I)) & 0xFF;
	convert.m4cur.i_h = (getMotorData(M4,id,(int)CUR_I)>>8) & 0xFF;
	convert.m4cur.d_l = (getMotorData(M4,id,(int)CUR_D)) & 0xFF;
	convert.m4cur.d_h = (getMotorData(M4,id,(int)CUR_D)>>8) & 0xFF;
	/////////////////other///////////////////////
	convert.lpfk_l = (getMotorBordData(id,(int)LPF_K)) & 0xFF;
	convert.lpfk_h = (getMotorBordData(id,(int)LPF_K)>>8) & 0xFF;
	//Serial.println(((convert.lpfk_h)<<8)|(convert.lpfk_l));
	convert.lpft_l = (getMotorBordData(id,(int)LPF_T)) & 0xFF;
	convert.lpft_h = (getMotorBordData(id,(int)LPF_T)>>8) & 0xFF;
	///////////////////////////////////////////////

	/////////////////motor1//////////////
	convert.m1.rotate_speed_l = (getMotorData(M1,id,(int)ROTATE_SPEED)) & 0xFF;
	convert.m1.rotate_speed_h = (getMotorData(M1,id,(int)ROTATE_SPEED) >> 8) & 0xFF;
	convert.m1.rotate_power = (getMotorData(M1,id,(int)ROTATE_PWR)) & 0xFF;
	convert.m1.brake_power_l = (getMotorData(M1,id,(int)BRAKE_PWR)) & 0xFF;
	convert.m1.brake_power_h = (getMotorData(M1,id,(int)BRAKE_PWR) >> 8) & 0xFF;
	convert.m1.activate_brake = (getMotorFlag(M1,id,(int)ACTIVATE_BRAKE)) & 0b1;
	convert.m1.rotate_with_brake = (getMotorFlag(M1,id,(int)ROTATE_WITH_BRAKE)) & 0b1;
	convert.m1.rotate_direction = (getMotorFlag(M1,id,(int)ROTATE_DIRECTION)) & 0b1;
	/////////////////motor2//////////////
	convert.m2.rotate_speed_l = (getMotorData(M2,id,(int)ROTATE_SPEED)) & 0xFF;
	convert.m2.rotate_speed_h = (getMotorData(M2,id,(int)ROTATE_SPEED) >> 8) & 0xFF;
	convert.m2.rotate_power = (getMotorData(M2,id,(int)ROTATE_PWR)) & 0xFF;
	convert.m2.brake_power_l = (getMotorData(M2,id,(int)BRAKE_PWR)) & 0xFF;
	convert.m2.brake_power_h = (getMotorData(M2,id,(int)BRAKE_PWR) >> 8) & 0xFF;
	convert.m2.activate_brake = (getMotorFlag(M2,id,(int)ACTIVATE_BRAKE)) & 0b1;
	convert.m2.rotate_with_brake = (getMotorFlag(M2,id,(int)ROTATE_WITH_BRAKE)) & 0b1;
	convert.m2.rotate_direction = (getMotorFlag(M2,id,(int)ROTATE_DIRECTION)) & 0b1;
	/////////////////motor3//////////////
	convert.m3.rotate_speed_l = (getMotorData(M3,id,(int)ROTATE_SPEED)) & 0xFF;
	convert.m3.rotate_speed_h = (getMotorData(M3,id,(int)ROTATE_SPEED) >> 8) & 0xFF;
	convert.m3.rotate_power = (getMotorData(M3,id,(int)ROTATE_PWR)) & 0xFF;
	convert.m3.brake_power_l = (getMotorData(M3,id,(int)BRAKE_PWR)) & 0xFF;
	convert.m3.brake_power_h = (getMotorData(M3,id,(int)BRAKE_PWR) >> 8) & 0xFF;
	convert.m3.activate_brake = (getMotorFlag(M3,id,(int)ACTIVATE_BRAKE)) & 0b1;
	convert.m3.rotate_with_brake = (getMotorFlag(M3,id,(int)ROTATE_WITH_BRAKE)) & 0b1;
	convert.m3.rotate_direction = (getMotorFlag(M3,id,(int)ROTATE_DIRECTION)) & 0b1;
	/////////////////motor4//////////////
	convert.m4.rotate_speed_l = (getMotorData(M4,id,(int)ROTATE_SPEED)) & 0xFF;
	convert.m4.rotate_speed_h = (getMotorData(M4,id,(int)ROTATE_SPEED) >> 8) & 0xFF;
	convert.m4.rotate_power = (getMotorData(M4,id,(int)ROTATE_PWR)) & 0xFF;
	convert.m4.brake_power_l = (getMotorData(M4,id,(int)BRAKE_PWR)) & 0xFF;
	convert.m4.brake_power_h = (getMotorData(M4,id,(int)BRAKE_PWR) >> 8) & 0xFF;
	convert.m4.activate_brake = (getMotorFlag(M4,id,(int)ACTIVATE_BRAKE)) & 0b1;
	convert.m4.rotate_with_brake = (getMotorFlag(M4,id,(int)ROTATE_WITH_BRAKE)) & 0b1;
	convert.m4.rotate_direction = (getMotorFlag(M4,id,(int)ROTATE_DIRECTION)) & 0b1;
	////////////////for all motor////////////////
	convert.led1 = getMotorBordFlag(id,(int)ACTIVATE_LED1);
	convert.led2 = getMotorBordFlag(id,(int)ACTIVATE_LED2);
	convert.led3 = getMotorBordFlag(id,(int)ACTIVATE_LED3);
	convert.serial = getMotorBordData(id,(int)PRINT);

	pSend->setId(id);
	///////////////////motor1/////////////////////////
	pSend->setMotorSerial(M1_ADJUST_PWR_L,convert.m1adjust.adjust_power_l);
	pSend->setMotorSerial(M1_ADJUST_PWR_H,convert.m1adjust.adjust_power_h);
	pSend->setMotorSerial(M1_LOWER_LIMIT_L,convert.m1adjust.lower_limit_l);
	pSend->setMotorSerial(M1_LOWER_LIMIT_H,convert.m1adjust.lower_limit_h);
	pSend->setMotorSerial(M1_ENC_P_L,convert.m1enc.p_l);
	pSend->setMotorSerial(M1_ENC_P_H,convert.m1enc.p_h);
	pSend->setMotorSerial(M1_ENC_I_L,convert.m1enc.i_l);
	pSend->setMotorSerial(M1_ENC_I_H,convert.m1enc.i_h);
	pSend->setMotorSerial(M1_ENC_D_L,convert.m1enc.d_l);
	pSend->setMotorSerial(M1_ENC_D_H,convert.m1enc.d_h);
	pSend->setMotorSerial(M1_CUR_P_L,convert.m1cur.p_l);
	pSend->setMotorSerial(M1_CUR_P_H,convert.m1cur.p_h);
	pSend->setMotorSerial(M1_CUR_I_L,convert.m1cur.i_l);
	pSend->setMotorSerial(M1_CUR_I_H,convert.m1cur.i_h);
	pSend->setMotorSerial(M1_CUR_D_L,convert.m1cur.d_l);
	pSend->setMotorSerial(M1_CUR_D_H,convert.m1cur.d_h);
	///////////////////motor2/////////////////////////
	pSend->setMotorSerial(M2_ADJUST_PWR_L,convert.m2adjust.adjust_power_l);
	pSend->setMotorSerial(M2_ADJUST_PWR_H,convert.m2adjust.adjust_power_h);
	pSend->setMotorSerial(M2_LOWER_LIMIT_L,convert.m2adjust.lower_limit_l);
	pSend->setMotorSerial(M2_LOWER_LIMIT_H,convert.m2adjust.lower_limit_h);
	pSend->setMotorSerial(M2_ENC_P_L,convert.m2enc.p_l);
	pSend->setMotorSerial(M2_ENC_P_H,convert.m2enc.p_h);
	pSend->setMotorSerial(M2_ENC_I_L,convert.m2enc.i_l);
	pSend->setMotorSerial(M2_ENC_I_H,convert.m2enc.i_h);
	pSend->setMotorSerial(M2_ENC_D_L,convert.m2enc.d_l);
	pSend->setMotorSerial(M2_ENC_D_H,convert.m2enc.d_h);
	pSend->setMotorSerial(M2_CUR_P_L,convert.m2cur.p_l);
	pSend->setMotorSerial(M2_CUR_P_H,convert.m2cur.p_h);
	pSend->setMotorSerial(M2_CUR_I_L,convert.m2cur.i_l);
	pSend->setMotorSerial(M2_CUR_I_H,convert.m2cur.i_h);
	pSend->setMotorSerial(M2_CUR_D_L,convert.m2cur.d_l);
	pSend->setMotorSerial(M2_CUR_D_H,convert.m2cur.d_h);
	///////////////////motor3/////////////////////////
	pSend->setMotorSerial(M3_ADJUST_PWR_L,convert.m3adjust.adjust_power_l);
	pSend->setMotorSerial(M3_ADJUST_PWR_H,convert.m3adjust.adjust_power_h);
	pSend->setMotorSerial(M3_LOWER_LIMIT_L,convert.m3adjust.lower_limit_l);
	pSend->setMotorSerial(M3_LOWER_LIMIT_H,convert.m3adjust.lower_limit_h);
	pSend->setMotorSerial(M3_ENC_P_L,convert.m3enc.p_l);
	pSend->setMotorSerial(M3_ENC_P_H,convert.m3enc.p_h);
	pSend->setMotorSerial(M3_ENC_I_L,convert.m3enc.i_l);
	pSend->setMotorSerial(M3_ENC_I_H,convert.m3enc.i_h);
	pSend->setMotorSerial(M3_ENC_D_L,convert.m3enc.d_l);
	pSend->setMotorSerial(M3_ENC_D_H,convert.m3enc.d_h);
	pSend->setMotorSerial(M3_CUR_P_L,convert.m3cur.p_l);
	pSend->setMotorSerial(M3_CUR_P_H,convert.m3cur.p_h);
	pSend->setMotorSerial(M3_CUR_I_L,convert.m3cur.i_l);
	pSend->setMotorSerial(M3_CUR_I_H,convert.m3cur.i_h);
	pSend->setMotorSerial(M3_CUR_D_L,convert.m3cur.d_l);
	pSend->setMotorSerial(M3_CUR_D_H,convert.m3cur.d_h);
	///////////////////motor4/////////////////////////
	pSend->setMotorSerial(M4_ADJUST_PWR_L,convert.m4adjust.adjust_power_l);
	pSend->setMotorSerial(M4_ADJUST_PWR_H,convert.m4adjust.adjust_power_h);
	pSend->setMotorSerial(M4_LOWER_LIMIT_L,convert.m4adjust.lower_limit_l);
	pSend->setMotorSerial(M4_LOWER_LIMIT_H,convert.m4adjust.lower_limit_h);
	pSend->setMotorSerial(M4_ENC_P_L,convert.m4enc.p_l);
	pSend->setMotorSerial(M4_ENC_P_H,convert.m4enc.p_h);
	pSend->setMotorSerial(M4_ENC_I_L,convert.m4enc.i_l);
	pSend->setMotorSerial(M4_ENC_I_H,convert.m4enc.i_h);
	pSend->setMotorSerial(M4_ENC_D_L,convert.m4enc.d_l);
	pSend->setMotorSerial(M4_ENC_D_H,convert.m4enc.d_h);
	pSend->setMotorSerial(M4_CUR_P_L,convert.m4cur.p_l);
	pSend->setMotorSerial(M4_CUR_P_H,convert.m4cur.p_h);
	pSend->setMotorSerial(M4_CUR_I_L,convert.m4cur.i_l);
	pSend->setMotorSerial(M4_CUR_I_H,convert.m4cur.i_h);
	pSend->setMotorSerial(M4_CUR_D_L,convert.m4cur.d_l);
	pSend->setMotorSerial(M4_CUR_D_H,convert.m4cur.d_h);

	///////////////////other/////////////////////////
	pSend->setMotorSerial(SENSOR_DIRECRION,convert.sensor_direction);
	pSend->setMotorSerial(SET_SENSOR,convert.setsensor);
	pSend->setMotorSerial(LIB_LPF_K_L,convert.lpfk_l);
	pSend->setMotorSerial(LIB_LPF_K_H,convert.lpfk_h);
	pSend->setMotorSerial(LIB_LPF_T_L,convert.lpft_l);
	pSend->setMotorSerial(LIB_LPF_T_H,convert.lpft_h);
	//pSend->setMotorSerial(LIB_LPF_T_L,10);
	//pSend->setMotorSerial(LIB_LPF_T_H,10);


	///////////////////motor1/////////////////////////
	pSend->setMotorSerial(M1_ROTATE_SPEED_L,convert.m1.rotate_speed_l);
	pSend->setMotorSerial(M1_ROTATE_SPEED_H,convert.m1.rotate_speed_h);
	pSend->setMotorSerial(M1_ROTATE_PWR,convert.m1.rotate_power);
	pSend->setMotorSerial(M1_BRAKE_PWR_L,convert.m1.brake_power_l);
	pSend->setMotorSerial(M1_BRAKE_PWR_H,convert.m1.brake_power_h);
	pSend->setMotorSerial(M1_ETC,convert.m1.etc);
	///////////////////motor1/////////////////////////
	pSend->setMotorSerial(M2_ROTATE_SPEED_L,convert.m2.rotate_speed_l);
	pSend->setMotorSerial(M2_ROTATE_SPEED_H,convert.m2.rotate_speed_h);
	pSend->setMotorSerial(M2_ROTATE_PWR,convert.m2.rotate_power);
	pSend->setMotorSerial(M2_BRAKE_PWR_L,convert.m2.brake_power_l);
	pSend->setMotorSerial(M2_BRAKE_PWR_H,convert.m2.brake_power_h);
	pSend->setMotorSerial(M2_ETC,convert.m2.etc);
	///////////////////motor1/////////////////////////
	pSend->setMotorSerial(M3_ROTATE_SPEED_L,convert.m3.rotate_speed_l);
	pSend->setMotorSerial(M3_ROTATE_SPEED_H,convert.m3.rotate_speed_h);
	pSend->setMotorSerial(M3_ROTATE_PWR,convert.m3.rotate_power);
	pSend->setMotorSerial(M3_BRAKE_PWR_L,convert.m3.brake_power_l);
	pSend->setMotorSerial(M3_BRAKE_PWR_H,convert.m3.brake_power_h);
	pSend->setMotorSerial(M3_ETC,convert.m3.etc);
	///////////////////motor1/////////////////////////
	pSend->setMotorSerial(M4_ROTATE_SPEED_L,convert.m4.rotate_speed_l);
	pSend->setMotorSerial(M4_ROTATE_SPEED_H,convert.m4.rotate_speed_h);
	pSend->setMotorSerial(M4_ROTATE_PWR,convert.m4.rotate_power);
	pSend->setMotorSerial(M4_BRAKE_PWR_L,convert.m4.brake_power_l);
	pSend->setMotorSerial(M4_BRAKE_PWR_H,convert.m4.brake_power_h);
	pSend->setMotorSerial(M4_ETC,convert.m4.etc);
	//////////////////for all////////////////////////
	pSend->setMotorSerial(UI,convert.ui);

}

int Ui::getBordCount(const LibBordTypeEnum _type){
	switch(_type){
	default:
	case LIB_NO_TYPE:
		return 0;
		break;
	case LIB_MDD:
		return motor_bord_count;
		break;
	}
}

LibReturnStateEnum Ui::setMotorBordData(const int _id,const int _target,const int _source){
	int count = getBordCountFromId(_id);
	if(count == INT_ERROR || _target < 48 || getBordType(_id) != LIB_MDD){
		return LIB_ERROR;
	}
	int target = (int)_target;
	motor_bord[count].data[target] = _source;
	return LIB_OK;
}

int Ui::getMotorBordData(const int _id,const int _target){
	int count = getBordCountFromId(_id);
	if(count == INT_ERROR || _target < 48 || getBordType(_id) != LIB_MDD){
		return LIB_ERROR;
	}
	int target = (int)_target;
	return motor_bord[count].data[target];
}

LibReturnStateEnum Ui::setMotorData(const LibMotorNumEnum _num ,const int _id,const int _target,const int _source){
	int count = getBordCountFromId(_id);
	if(count == INT_ERROR || _target > 11 || _target < 0 || getBordType(_id) != LIB_MDD){
		return LIB_ERROR;
	}
	int target = (int)_target + (int)_num * 12;
	motor_bord[count].data[target] = _source;
	return LIB_OK;
}

int Ui::getMotorData(const LibMotorNumEnum _num ,const int _id,const int _target){
	if(_target > 11 || _target < 0 || getBordType(_id) != LIB_MDD){
		return 0;
	}
	int count = getBordCountFromId(_id);
	int target = (int)_target + (int)_num * 12;
	return motor_bord[count].data[target];
}

LibReturnStateEnum Ui::setMotorBordFlag(const int _id,const int _target,const bool _source){
	int count = getBordCountFromId(_id);
	if(getBordType(_id) != LIB_MDD || count == INT_ERROR){
		return LIB_ERROR;
	}
	if(_source == true){
		setFlagBit(&(motor_bord[count].data[MOTOR_BORD_DATA_LEN - 1]),_target,1);
	}else{
		setFlagBit(&(motor_bord[count].data[MOTOR_BORD_DATA_LEN - 1]),_target,0);
	}
	return LIB_OK;
}

int Ui::getMotorBordFlag(const int _id,const int _target){
	int count = getBordCountFromId(_id);
	if(getBordType(_id) != LIB_MDD || count == INT_ERROR){
		return INT_ERROR;
	}
	if(getFlagBit(&(motor_bord[count].data[MOTOR_BORD_DATA_LEN - 1]),_target) == LIB_TRUE){
		return 1;
	}
	return 0;
}

LibReturnStateEnum Ui::setMotorFlag(const LibMotorNumEnum _num ,const int _id,const int _target,const bool _source){
	int count = getBordCountFromId(_id);
	if(getBordType(_id) != LIB_MDD || count == INT_ERROR){
		return LIB_ERROR;
	}
	int target = (int)_num * 12;
	if(_source == true){
		setFlagBit(&(motor_bord[count].data[target]),_target,1);
	}else{
		setFlagBit(&(motor_bord[count].data[target]),_target,0);
	}
	return LIB_OK;
}

int Ui::getMotorFlag(const LibMotorNumEnum _num ,const int _id,const int _source){
	int count = getBordCountFromId(_id);
	if(getBordType(_id) != LIB_MDD || count == INT_ERROR){
		return 0;
	}
	int target = (int)_num * 12;
	if(getFlagBit(&(motor_bord[count].data[target]),_source) == LIB_TRUE){
		return 1;
	}
	return 0;
}

int Ui::getMotorPreFlag(const LibMotorNumEnum _num ,const int _id,const int _source){//_countは配列の何個目かという意味
	int count = getBordCountFromId(_id);
	if(getBordType(_id) != LIB_MDD || count == INT_ERROR){
		return 0;
	}
	int target = (int)_num * 12;
	if(getFlagBit(&(motor_bord[count].pre_data[target]),_source) == LIB_TRUE){
		return 1;
	}
	return 0;
}

int Ui::getMotorPreData(const LibMotorNumEnum _num ,int _id,const int _target){
	int count = getBordCountFromId(_id);
	if(_target > 11 || _target < 0 || getBordType(_id) != LIB_MDD || count == INT_ERROR){
		return 0;
	}
	int target = (int)_target + (int)_num * 12;
	return motor_bord[count].pre_data[target];
}

int Ui::getMotorBordPreFlag(const int _id,const bool _source){
	int count = getBordCountFromId(_id);
	if(getBordType(_id) != LIB_MDD || count == INT_ERROR){
		return LIB_ERROR;
	}
	if(getFlagBit(&(motor_bord[count].pre_data[MOTOR_BORD_DATA_LEN - 1]),_source) == LIB_TRUE){
		return 1;
	}
	return 0;
}
/////////////other////////////


LibBordTypeEnum Ui::getBordType(const int _id){
	return (LibBordTypeEnum)bord_type[_id];
}


LibReturnStateEnum Ui::setFlagBit(int *target1,const int target2,const int source){
	if(target2 > 15 || target2 < 0 || source > 1 || source < 0){
		return LIB_ERROR;
	}
	if(source == 0){
		*target1 &= ~(0b1 << target2);
	}else{
		*target1 |= 0b1 << target2;
	}
	return LIB_OK;
}

LibBoolEnum Ui::getFlagBit(int *target1,const int target2){
	if(target2 > 15 || target2 < 0){
		return LIB_FALSE;
	}
	if((*target1 >> target2) & 0b1 == 1){
		return LIB_TRUE;
	}
	return LIB_FALSE;
}

int Ui::getBordCountFromId(const int _id){
	int count = bord_id[_id];
	return bord_id[_id];
}

LibBordTypeEnum Ui::getBordTypeFromId(const int _id){
	return (LibBordTypeEnum)bord_type[_id];
}

void Ui::initBordId(void){
	for(int i = 0; i < 0xFF; i++){
		bord_id[i] = 0xFF;
		bord_type[i] = (int)LIB_NO_TYPE;
	}
	motor_bord_count = 0;
}

LibReturnStateEnum Ui::initBordData(const int _id ,const LibBordTypeEnum _type){
	int count = getBordCountFromId(_id);
	if(count == INT_ERROR){
		return LIB_ERROR;
	}
	switch(_type){
	case LIB_NO_TYPE:
		break;
	case LIB_MDD:
		for(int i = 0; i < 4; i++){//TODO:初期化書かんといかん

			setMotorData((LibMotorNumEnum)i,_id,(int)ADJUST_PWR,convertFloatToTwoByte(DEFAULT_ADJUST_PWR));
			setMotorData((LibMotorNumEnum)i,_id,(int)LOWER_LIMIT,((int)DEFAULT_LOWER_LIMIT));
			setMotorData((LibMotorNumEnum)i,_id,(int)ENC_P,convertFloatToTwoByte(DEFAULT_ENC_P));
			setMotorData((LibMotorNumEnum)i,_id,(int)ENC_I,convertFloatToTwoByte(DEFAULT_ENC_I));
			setMotorData((LibMotorNumEnum)i,_id,(int)ENC_D,convertFloatToTwoByte(DEFAULT_ENC_D));
			setMotorData((LibMotorNumEnum)i,_id,(int)CUR_P,convertFloatToTwoByte(DEFAULT_CUR_D));
			setMotorData((LibMotorNumEnum)i,_id,(int)CUR_I,convertFloatToTwoByte(DEFAULT_CUR_D));
			setMotorData((LibMotorNumEnum)i,_id,(int)CUR_D,convertFloatToTwoByte(DEFAULT_CUR_D));
			setMotorData((LibMotorNumEnum)i,_id,(int)ROTATE_SPEED,(int)(DEFAULT_ROTATE_SPEED_H  << 8 | DEFAULT_ROTATE_SPEED_L));
			setMotorData((LibMotorNumEnum)i,_id,(int)ROTATE_PWR,(int)(DEFAULT_ROTATE_PWR));
			setMotorData((LibMotorNumEnum)i,_id,(int)BRAKE_PWR,(int)(DEFAULT_BRAKE_PWR));
			setMotorFlag((LibMotorNumEnum)i,_id,(int)ACTIVATE_BRAKE,(int)(DEFAULT_ACTIVATE_BRAKE));
			setMotorFlag((LibMotorNumEnum)i,_id,(int)ROTATE_WITH_BRAKE,(int)(DEFAULT_ROTATE_WITH_BRAKE));
			setMotorFlag((LibMotorNumEnum)i,_id,(int)ROTATE_DIRECTION,(int)(DEFAULT_ROTATE_DIRECTION));
			setMotorFlag((LibMotorNumEnum)i,_id,(int)ACTIVATE_CUR,(int)(DEFAULT_ACTIVATE_CUR));
			setMotorFlag((LibMotorNumEnum)i,_id,(int)ACTIVATE_ENC,(int)(DEFAULT_ACTIVATE_ENC));
			setMotorFlag((LibMotorNumEnum)i,_id,(int)ENC_DIRECTION,(int)(DEFAULT_ENC_DIRECTION));
		}
		setMotorBordData(_id,(int)LPF_K,DEFAULT_LPF_K);
		setMotorBordData(_id,(int)LPF_T,DEFAULT_LPF_T);
		setMotorBordFlag(_id,(int)ACTIVATE_LED1,true);
		setMotorBordFlag(_id,(int)ACTIVATE_LED2,true);
		setMotorBordFlag(_id,(int)ACTIVATE_LED3,true);
		setMotorBordData(_id,(int)PRINT,4);
		//motor_bord[count].id = 0;
		motor_bord[count].flag = DEFAULT_SHORT_FLAG;
		break;
	default:
		break;
	}
	return LIB_OK;
}

LibReturnStateEnum Ui::updatePreBordData(const int _count ,const LibBordTypeEnum _type){
	switch(_type){
	case LIB_NO_TYPE:
		break;
	case LIB_MDD:
		for(int i = 0; i < MOTOR_BORD_DATA_LEN; i++){
			motor_bord[_count].pre_data[i] = motor_bord[_count].data[i];
		}
		break;
	default:
		break;
	}
}
