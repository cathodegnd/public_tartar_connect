/*
 Example sketch for the TartarConnect library - developed by cathidegnd
 for Arduino Mega 2560 and Arduino Due
 target nucleo f446re(tartarConnect inside)

 */

#include "TartarConnect.h"

#define MDD_ID 1

typedef struct{
	int pin;
	bool flag;
	uint32_t t;
}Sw;

Sw sw = {12,true,0};
////////////TartarConnectライブラリを使うために必要なおまじないのようなものです////////
Rs485 rs485(&Serial1);
SendStrage send(&rs485);
ReceiveStrage receive(&rs485);
Ui ui(&send, &receive);
////////////モーターごとにMddMotorクラスをインスタンス化します/////////
MddMotor m1(1, MDD_ID, &ui);  //引数:MDDで駆動できる4つのモーターのうちどれを使うのか,MDDのID,UIクラスへのポインタ
MddMotor m2(2, MDD_ID, &ui);
MddMotor m3(3, MDD_ID, &ui);
MddMotor m4(4, MDD_ID, &ui);
///////////////////////////////////////////////////////////////////


bool pushSw(Sw *_pSw);

void setup(void) {
	Serial.begin(115200);
	pinMode(sw.pin, INPUT_PULLUP);
	ui.init();                    //UIクラスを初期化します
	ui.addBord(MDD_ID, LIB_MDD);  //ボードを追加します。引数:ボードのID,ボードの種類
}

void loop(void) {
	static uint32_t t = millis();
	static int count = 0;

	if(pushSw(&sw) == true){
		count++;
	}
	switch (count) {
	case 0:
	Serial.println("0");
	m1.setBordFlag(ACTIVATE_LED1, false);
	m1.setBordFlag(ACTIVATE_LED2, false);
	m1.setBordFlag(ACTIVATE_LED3, false);
	break;
	case 1:
	Serial.println("1");
	static uint32_t t2 = 0;
	static int count2 = 0;
	if(millis() - t2 > 2000){
		count2++;
		t2 = millis();
	}
	m1.setParamFlag(ACTIVATE_ENC, false);
	m2.setParamFlag(ACTIVATE_ENC, false);
	m3.setParamFlag(ACTIVATE_ENC, false);
	m4.setParamFlag(ACTIVATE_ENC, false);
	m1.setBordData(PRINT, PRINT_MOVE_DATA);
	m1.setBordFlag(ACTIVATE_LED1, true);
	m1.setBordFlag(ACTIVATE_LED2, false);
	m1.setBordFlag(ACTIVATE_LED3, false);
	m1.setData(ROTATE_SPEED, 0xFFFF);
	m1.setData(BRAKE_PWR, 0xFFFF);
	m2.setData(ROTATE_SPEED, 0xFFFF);
	m2.setData(BRAKE_PWR, 0xFFFF);
	m3.setData(ROTATE_SPEED, 0xFFFF);
	m3.setData(BRAKE_PWR, 0xFFFF);
	m4.setData(ROTATE_SPEED, 0xFFFF);
	m4.setData(BRAKE_PWR, 0xFFFF);
	switch (count2) {
	case 0:
	m1.setDataFlag(ROTATE_DIRECTION, true);
	m1.setDataFlag(ACTIVATE_BRAKE, false);
	m2.setDataFlag(ROTATE_DIRECTION, true);
	m2.setDataFlag(ACTIVATE_BRAKE, false);
	m3.setDataFlag(ROTATE_DIRECTION, true);
	m3.setDataFlag(ACTIVATE_BRAKE, false);
	m4.setDataFlag(ROTATE_DIRECTION, true);
	m4.setDataFlag(ACTIVATE_BRAKE, false);
	break;
	case 1:
	m1.setDataFlag(ACTIVATE_BRAKE, true);
	m2.setDataFlag(ACTIVATE_BRAKE, true);
	m3.setDataFlag(ACTIVATE_BRAKE, true);
	m4.setDataFlag(ACTIVATE_BRAKE, true);
	break;
	case 2:
	m1.setDataFlag(ROTATE_DIRECTION, false);
	m1.setDataFlag(ACTIVATE_BRAKE, false);
	m2.setDataFlag(ROTATE_DIRECTION, false);
	m2.setDataFlag(ACTIVATE_BRAKE, false);
	m3.setDataFlag(ROTATE_DIRECTION, false);
	m3.setDataFlag(ACTIVATE_BRAKE, false);
	m4.setDataFlag(ROTATE_DIRECTION, false);
	m4.setDataFlag(ACTIVATE_BRAKE, false);
	break;
	default:
	count2 = 0;
	break;
	}
	break;
	case 2:
	Serial.println("2");
	m1.setParamFlag(ACTIVATE_ENC, true);
	m1.setBordData(PRINT, PRINT_M1_SENSOR);
	m1.setBordFlag(ACTIVATE_LED1, false);
	m1.setBordFlag(ACTIVATE_LED2, true);
	m1.setBordFlag(ACTIVATE_LED3, false);
	m1.setData(ROTATE_SPEED, 0);
	m2.setData(ROTATE_SPEED, 0);
	m3.setData(ROTATE_SPEED, 0);
	m4.setData(ROTATE_SPEED, 0);
	break;
	case 3:
	Serial.println("3");
	m2.setParamFlag(ACTIVATE_ENC, true);
	m1.setBordData(PRINT, PRINT_M2_SENSOR);
	m1.setBordFlag(ACTIVATE_LED1, true);
	m1.setBordFlag(ACTIVATE_LED2, true);
	m1.setBordFlag(ACTIVATE_LED3, false);
	break;
	case 4:
	m3.setParamFlag(ACTIVATE_ENC, true);
	Serial.println("4");
	m1.setBordData(PRINT, PRINT_M3_SENSOR);
	m1.setBordFlag(ACTIVATE_LED1, false);
	m1.setBordFlag(ACTIVATE_LED2, false);
	m1.setBordFlag(ACTIVATE_LED3, true);
	break;
	case 5:
	Serial.println("5");
	m4.setParamFlag(ACTIVATE_ENC, true);
	m1.setBordData(PRINT, PRINT_M4_SENSOR);
	m1.setBordFlag(ACTIVATE_LED1, true);
	m1.setBordFlag(ACTIVATE_LED2, false);
	m1.setBordFlag(ACTIVATE_LED3, true);
	break;
	case 6:
	Serial.println("6");
	m1.setBordFlag(ACTIVATE_LED1, false);
	m1.setBordFlag(ACTIVATE_LED2, true);
	m1.setBordFlag(ACTIVATE_LED3, true);
	break;
	default:
	count = 0;
	break;
	}
	if (millis() - t > 10) {
		ui.sendToMotor(MDD_ID, 100);
		t = millis();
	}
}

bool pushSw(Sw *_pSw){
  bool read_val = digitalRead(_pSw->pin);
	uint32_t t = millis();
  if(_pSw->flag == false && read_val == false && t - _pSw->t > 500){
    _pSw->flag = true;
		return true;
  }else if(_pSw->flag == true && read_val == true){
		_pSw->t = t;
    _pSw->flag = false;
  }
	return false;
}