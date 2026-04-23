#ifndef _LIB_UI_H_
#define _LIB_UI_H_
#include "lib_strage.h"
#include "lib_motor_ui_enum.h"

#define PRINT_M1_SENSOR 0
#define	PRINT_M2_SENSOR 1
#define	PRINT_M3_SENSOR 2
#define	PRINT_M4_SENSOR 3
#define	PRINT_MOVE_DATA 4

typedef struct{
	int id;
	int flag;
	int data[MOTOR_BORD_DATA_LEN];
	int pre_data[MOTOR_BORD_DATA_LEN];
}LibMotorBord;

typedef enum{
	LIB_NO_TYPE,
	LIB_MDD
}LibBordTypeEnum;
class Ui{//sendtomotorやsendchangetomotorはsend(void)に最終的に統一したい...
public:
	Ui(SendStrage *_pSend,ReceiveStrage *_pReceive);
	void init(void);//ライブラリ全体の初期化
	LibReturnStateEnum initBordData(const int _id ,const LibBordTypeEnum _type);//ボード単位の初期化
	LibReturnStateEnum addBord(const int _id,const LibBordTypeEnum _type);//ボードの追加
	LibBordTypeEnum getBordType(const int _id);//idで設定されたボードの種類を取得
	LibReturnStateEnum sendToMotor(const int _id,const int _timeout);//モーターボードに変更してないデータも全部送る
	LibReturnStateEnum sendChangeToMotor(const int _timeout);//モーターボードに変更のあったデータを送る
	int getBordCount(const LibBordTypeEnum _type);//現在登録されているボードの数を返す

	//////通常、外部から使わない領域////////
	int convertFloatToTwoByte(const float _source);
	LibReturnStateEnum setMotorBordData(const int _id,const int _target,const int _source);
	int getMotorBordData(const int _id,const int _target);
	LibReturnStateEnum setMotorData(const LibMotorNumEnum _num ,const int _id,const int _target,const int _source);
	int getMotorData(const LibMotorNumEnum _num ,const int _id,const int _target);
	LibReturnStateEnum setMotorFlag(const LibMotorNumEnum _num ,const int _id,const int _target,const bool _source);
	int getMotorFlag(const LibMotorNumEnum _num ,const int _id,const int _source);
	LibReturnStateEnum setMotorBordFlag(const int _id,const int _target,const bool _source);
	int getMotorBordFlag(const int _id,const int _target);
private:
	void setMotor(const int _bord_count);
	int getMotorId(const int _count);
	void setMotorId(const int _count,const int _id);
	int getMotorFlagFromCount(const LibMotorNumEnum _num ,const int _count,const int _source);
	int getMotorDataFromCount(const LibMotorNumEnum _num ,const int _count,const int _target);
	int getMotorBordFlagFromCount(const int _count,const bool _source);
	int getMotorPreFlag(const LibMotorNumEnum _num ,const int _id,const int _source);//_countは配列の何個目かという意味
	int getMotorPreData(const LibMotorNumEnum _num ,const int _id,const int _target);
	int getMotorBordPreFlag(const int _id,const bool _source);
	LibReturnStateEnum setFlagBit(int *target1,const int target2,const int source);
	LibBoolEnum getFlagBit(int *target1,const int target2);
	int getBordCountFromId(const int _id);
	LibBordTypeEnum getBordTypeFromId(const int _id);
	void initBordId(void);
	LibReturnStateEnum updatePreBordData(const int _count ,const LibBordTypeEnum _type);
	LibMotorBord motor_bord[MOTOR_BORD_MAX_NUM];//ヒープ領域はメモリリーク怖いから使わない。自分は絶対やらかす自信がある。
	SendStrage *pSend;
	ReceiveStrage *pReceive;
	int motor_bord_count = 0;//モーターボードを何個ボードを登録したか
	int bord_id[0xFF] = {0};//ボードIDを受け取ってボードのカウントを返す
	int bord_type[0xFF] = {0};//ボードのIDを受け取ってボードの種類を返す
};

class MddMotor{
public:
	MddMotor(const int _motor,const int _id,Ui *_pUi);
	LibReturnStateEnum setBordFlag(const LibMotorBordFlagEnum _target,const bool _source);
	LibReturnStateEnum setBordData(const LibMotorBordDataEnum _target,const float _source);
	LibReturnStateEnum setParamFlag(const LibMotorParamFlagEnum _target,const bool _source);
	LibReturnStateEnum setDataFlag(const LibMotorMoveDataFlagEnum _target,const bool _source);
	LibReturnStateEnum setParam(const LibMotorParamEnum _target,const float _source);
	LibReturnStateEnum setData(const LibMotorMoveDataEnum _target,const int32_t _source);
private:
	Ui *pUi = nullptr;
	int id = 0;
	LibMotorNumEnum motor = M1;
};

class MotorUi{
public:
	MotorUi(Ui *_pUi);
	//////////////////motor 1//////////////////
	LibReturnStateEnum m1SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source);
	LibReturnStateEnum m1SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source);
	LibReturnStateEnum m1SetParam(const int _id,const LibMotorParamEnum _target,const float _source);
	LibReturnStateEnum m1SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source);
	//////////////////motor 2//////////////////
	LibReturnStateEnum m2SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source);
	LibReturnStateEnum m2SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source);
	LibReturnStateEnum m2SetParam(const int _id,const LibMotorParamEnum _target,const float _source);
	LibReturnStateEnum m2SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source);
	//////////////////motor 3//////////////////
	LibReturnStateEnum m3SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source);
	LibReturnStateEnum m3SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source);
	LibReturnStateEnum m3SetParam(const int _id,const LibMotorParamEnum _target,const float _source);
	LibReturnStateEnum m3SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source);
	//////////////////motor 4//////////////////
	LibReturnStateEnum m4SetParamFlag(const int _id,const LibMotorParamFlagEnum _target,const bool _source);
	LibReturnStateEnum m4SetDataFlag(const int _id,const LibMotorMoveDataFlagEnum _target,const bool _source);
	LibReturnStateEnum m4SetParam(const int _id,const LibMotorParamEnum _target,const float _source);
	LibReturnStateEnum m4SetData(const int _id,const LibMotorMoveDataEnum _target,const int32_t _source);
private:
	Ui *pUi;
};
#endif
