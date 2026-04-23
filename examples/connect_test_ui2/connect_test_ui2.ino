/*
 Example sketch for the TartarConnect library - developed by cathodegnd
 for Arduino Mega 2560 and Arduino Due
 target nucleo f446re(tartarmdd inside)
 */

#include "TartarConnect.h"

#define MDD_ID 15

////////////TartarConnectライブラリを使うために必要なおまじないのようなものです////////
Rs485 rs485(&Serial3);
SendStrage send(&rs485);
ReceiveStrage receive(&rs485);
Ui ui(&send, &receive);
////////////モーターごとにMddMotorクラスをインスタンス化します/////////
MddMotor m1(1, MDD_ID, &ui);  //引数:MDDで駆動できる4つのモーターのうちどれを使うのか,MDDのID,UIクラスへのポインタ
MddMotor m2(2, MDD_ID, &ui);
MddMotor m3(3, MDD_ID, &ui);
MddMotor m4(4, MDD_ID, &ui);
///////////////////////////////////////////////////////////////////

void setup(void) {
	ui.init();                    //UIクラスを初期化します
	ui.addBord(MDD_ID, LIB_MDD);  //ボードを追加します。引数:ボードのID,ボードの種類
}

void loop(void) {
	static uint32_t t = millis();
	if (millis() - t > 10) {
		//各引数のデフォルト値はlib_param.hに定義されています。

		m1.setBordData(PRINT, PRINT_MOVE_DATA);	//ターターMDDのUSBポートから出力するデータを設定します。引数:PRINT_MOVE_DATA(各モータの状態), PRINT_M1_SENSOR(エンコーダの目標パルスと、現在のパルス), PRINT_M2_SENSOR, PRINT_M3_SENSOR, PRINT_M4_SENSOR
		////////////////m1///////////////////
		m1.setData(ROTATE_SPEED, 0x0FFF);         //モーターのDuty比を変更
		m1.setData(BRAKE_PWR, 255);               //ブレーキの力を変更
		m1.setParam(ADJUST_PWR, 1.0);             //モーターのDuty比にこの値がかけられます(32ビットfloatではなく16ビットfloatです)
		m1.setParam(LOWER_LIMIT, 255 * 17);       //モーターのDuty比にこの値が加算されます
		m1.setParam(ENC_P, 300.0);                //エンコーダのPID制御のP成分です。
		m1.setParam(ENC_I, 10.0);                 //エンコーダのPID制御のI成分です。
		m1.setParam(ENC_D, 0.0);                  //エンコーダのPID制御のD成分です。
		m1.setDataFlag(ACTIVATE_BRAKE, false);    //ブレーキをONにするか(trueでon)
		m1.setDataFlag(ROTATE_WITH_BRAKE, true);  //ONにすると、モータードライバに負荷がかかりますが、制御性はよくなります
		m1.setDataFlag(ROTATE_DIRECTION, true);   //モーターの回転方向を設定します
		m1.setParamFlag(ACTIVATE_ENC, false);     //エンコーダーを用いたPID制御をするかを設定します(trueでon)
		////////////////m2///////////////////
		m2.setData(ROTATE_SPEED, 0x0FFF);
		m2.setData(BRAKE_PWR, 255);
		m2.setParam(ADJUST_PWR, 1.0);
		m2.setParam(LOWER_LIMIT, 255 * 17);
		m2.setParam(ENC_P, 300.0);
		m2.setParam(ENC_I, 10.0);
		m2.setParam(ENC_D, 0.0);
		m2.setDataFlag(ACTIVATE_BRAKE, false);
		m2.setDataFlag(ROTATE_WITH_BRAKE, true);
		m2.setDataFlag(ROTATE_DIRECTION, true);
		m2.setParamFlag(ACTIVATE_ENC, false);
		////////////////m3///////////////////
		m3.setData(ROTATE_SPEED, 0x0FFF);
		m3.setData(BRAKE_PWR, 255);
		m3.setParam(ADJUST_PWR, 1.0);
		m3.setParam(LOWER_LIMIT, 255 * 17);
		m3.setParam(ENC_P, 300.0);
		m3.setParam(ENC_I, 10.0);
		m3.setParam(ENC_D, 0.0);
		m3.setDataFlag(ACTIVATE_BRAKE, false);
		m3.setDataFlag(ROTATE_WITH_BRAKE, true);
		m3.setDataFlag(ROTATE_DIRECTION, true);
		m3.setParamFlag(ACTIVATE_ENC, false);
		////////////////m4///////////////////
		m4.setData(ROTATE_SPEED, 0x0FFF);
		m4.setData(BRAKE_PWR, 255);
		m4.setParam(ADJUST_PWR, 1.0);
		m4.setParam(LOWER_LIMIT, 255 * 17);
		m4.setParam(ENC_P, 300.0);
		m4.setParam(ENC_I, 10.0);
		m4.setParam(ENC_D, 0.0);
		m4.setDataFlag(ACTIVATE_BRAKE, false);
		m4.setDataFlag(ROTATE_WITH_BRAKE, true);
		m4.setDataFlag(ROTATE_DIRECTION, true);
		m4.setParamFlag(ACTIVATE_ENC, false);
		/////////////////send///////////////////
		ui.sendToMotor(MDD_ID, 100);//モーターボードにデータを送ります。引数:ボードのID,タイムアウト時間
		t = millis();
	}
}
