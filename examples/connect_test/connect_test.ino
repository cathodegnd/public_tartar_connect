/*
 Example sketch for the TartarConnect library - developed by cathodegnd
 for Arduino Mega 2560
 target nucleo f446re(tartarmdd inside)
*/




#include "TartarConnect.h"
Rs485 rs485(&Serial3);
SendStrage send(&rs485);
ReceiveStrage receive(&rs485);
Ui ui(&send,&receive);
MotorUi motorui(&ui);

#define ID 0x01

void setup() {
  // put your setup code here, to run once:
  ui.init();
  ui.addBord(ID,LIB_MDD);
}

void loop() {
  // put your main code here, to run repeatedly:
static uint32_t t = millis();
if(millis()-t > 10){

	motorui.m1SetData(ID,ROTATE_SPEED,0xFFFF);//モーターのDuty比を変更
	motorui.m1SetData(ID,BRAKE_PWR,255);//ブレーキの力を変更
	motorui.m1SetParam(ID,ADJUST_PWR,1.0);//モーターのDuty比にこの値がかけられます(32ビットfloatではなく16ビットfloatです)
	motorui.m1SetParam(ID,LOWER_LIMIT,255*17);//モーターのDuty比にこの値が加算されます
	motorui.m1SetParam(ID,ENC_P,300.0);//エンコーダのPID制御のP成分です。
	motorui.m1SetParam(ID,ENC_I,10.0);//エンコーダのPID制御のI成分です。
	motorui.m1SetParam(ID,ENC_D,0.0);//エンコーダのPID制御のD成分です。
	motorui.m1SetDataFlag(ID,ACTIVATE_BRAKE,false);//ブレーキをONにするか(trueでon)
	motorui.m1SetDataFlag(ID,ROTATE_WITH_BRAKE,true);//ONにすると、モータードライバに負荷がかかりますが、制御性はよくなります
	motorui.m1SetDataFlag(ID,ROTATE_DIRECTION,true);//モーターの回転方向を設定します
	motorui.m1SetParamFlag(ID,ACTIVATE_ENC,false);//エンコーダーを用いたPID制御をするかを設定します(trueでon)
  //上ではm1~のように設定してモーター1を制御しているが、m2,m3,m4~のように設定すれば、モーター2,3,4も制御することができる。
	ui.sendToMotor(ID,100);

	t = millis();
}
}