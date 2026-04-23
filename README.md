# ターターコネクト
## 概要
rs485通信を用いてArduinoボードから各種ドライバ基板を制御する為のライブラリです。
## 開発
高知ロボ研
## 環境
動作確認済み環境  
 Arduino Mega 2560(Arduino IDE 2.3.4)  
 Arduino Due(Arduino IDE 2.3.4)
## ライブラリの導入方法
スケッチブックの場所を開き、"libraries"というフォルダをスケッチブックのフォルダ内に作成してください。
"libraries"にmainリポジトリをcloneしてください。  
ディレクトリ構造は以下のようになるはずです。  
* Arduino/
    * libraries/
        * tartarconnect/  

Arduino IDEを再起動してください。
"File>Examples>tartarconnect"があるはずです。
## ライブラリの使い方
"connect_test.ino"で、Arduinoからモータードライバドライバを制御する例を示しています。
このライブラリを使用するには、
~~~
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
~~~
のようにインスタンスを作成しなければいけません。(めんどくさいですよね...。)  
モータードライバドライバのUIはMotorUiに含まれているので、上の例の場合は、"motorui."のようにアクセスします。  
モータードライバドライバへのパラメータの設定に用いるUIは"lib_ui"のMotorUi"クラスをいじるだけで簡単に変更できます。  
詳しくは、"connect_test_ui2.ino"を参照してください。(connect_test.ino では、MddMotorの部分が異なりますが、動きます。ただしこれからはui2をメインに開発を行うので、ui2の使用をお勧めします)
## License
This project is licensed under the MIT License.

This project includes code licensed under the Beer-Ware License by Robin Lilja.
