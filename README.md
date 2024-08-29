静電センサー電子ピアノ
作成日　2023年2月3日
1．	システムの概要
今回は「一口サイズのメロディー」をテーマにした「静電センサー電子ピアノ」を作成した.作成したシステムを図1に示す.
 
図1　作成したシステム
構成要素を以下に示す.
1.	鍵盤(9個)
2.	ブザー(1個)
3.	LED(1個)
4.	赤外線受信機(1個)
5.	ブレットボードパワーモジュール(電圧を5Vに変換し供給する)

構成要素とシステムを正面から見た図と側面から見た図を図2に示す.
  
図2システムを正面から見た図(上)横から見た図(下)
 

システムの概要を以下に示す.
	チーズの外装フィルムでできた静電センサーに触れることで圧電ブザーから音がなり,離すと音が止まる.
	静電センサーは鍵盤の役目を担っているのでセンサーの位置に応じた音階の音が鳴る.さらに音を鳴らしている間LEDが点灯する.
	赤外線受信機を搭載しておりリモコンでボタン入力を行うことで操作することが出来る,表1にリモコンのコマンドをまとめた

表1　リモコンのコマンド
 
 
2．	回路図とその説明
回路図を図3に示す.
 
図3　システムの回路図
	外部電源から電気を供給し2pinから電気が出力される回路となっている.
	3~11pinには抵抗を介して2pinから電流が入力されており,それぞれpinと抵抗の間はアルミ拍につながれている.
	13pinにはブザーとLEDが接続されており、13pinから信号が出ることでLEDが光り,ブザーから決められた周波数の音が鳴る.
	14pinには赤外線受信機が接続されており,赤外線を受信したとき14pinに信号が送られる.
	アルミ箔を使った静電センサの仕組み
	このシステムは静電センサとなる機械を使用しておらず,アルミ箔と1MΩの抵抗を使ってタッチセンサを作った(Ⅰ).
	まず図４に示すようにアルミ箔に触れたとき人体はコンデンサーの役目を担うため人体方向にも電気が流れるようになる.
	図5に示すように2pinから3~11pinにスタート信号が流れているため,3pinのアルミ箔に触れると2pinがHIGHになってから3pinがHIGHになるのに時間がかかる.
arduinoでは一定以上の高さでないとHIGHと見なさないため,その性質を利用して遅延時間を計測し評価することでアルミ箔に触れているかどうかを判断する.

 
図4　人体による影響を含めた回路図

 
図5　2pinからスタート信号が流れているようす

 
3．	プログラムとその説明
プログラムはメインのspark_sensor_shisakuとリモコンの設定のヘッダーファイルIRremote.h,音階の周波数をまとめたヘッダーファイルpitches.hで構成されている.全体のフローチャートを以下に示す.
 
図6　システムのフローチャート

 
次に自作した関数の説明をする.今回,赤外線通信の評価関数、静電センサーの表関数、Cの評価関数、CCの評価関数、シリアル通信で値を表示する関数を作成した.
まず定義した変数と定数の説明を表2にまとめた.
表２　定義した変数と定数
 
今回ピンの数Nは9とする.
関数onkaiは音の高さを設定する関数である,フローチャートを以下に示す.また重要な部分は赤く表示した.
 
図7　関数onkai() のフローチャート
関数SensorHhyoukaは静電センサーを評価する関数である,図8にSensorHyouka()のフローチャートを示す.
 
図8　関数Sensor_Hyouka()のフローチャート
関数¬ C_Hyoukaはcを評価する関数である, C_Hyouka()のフローチャートを図9に示す.

 
図9　関数C_Hyouka()のフローチャート

 
関数¬ CC_Hyoukaはccを評価する関数である, CC_Hyouka()のフローチャートを図10に示す.
 
図10 CC_Hyouka()のフローチャート

今回赤外線通信を行うIRremoteライブラリと,音を出力するToneライブラリが同じarduinoのタイマーを使用するため同時に使用することができない.
そのためToneライブラリとは違うタイマーを使用するNweToneライブラリをつかうことで解決した(Ⅱ).
参考文献
(Ⅰ)「Arduio タッチセンサ」
https://101010.fun/iot/arduino-diy-touch-sensor.html

(Ⅱ)「NewToneライブラリについて」
https://freelance-sakamoto.com/arduino_newtone/
 
以下にコードspark_sensor_shisakuを示す.
//spark_sensor_shisaku
#include "IR.h"
#include "pitches.h"
#include "IRremote.h"
#include <NewTone.h>

//シリアル通信の準備
IRrecv irrecv(RECEIVER); 
decode_results results; 

//各定数の定義
int out=2;//電気を発信するピン
const int BUZZER_PIN=13;//ブザーとLEDに接続するピン
int pin_num[]={3,4,5,6,7,8,9,10,11,12,13};//鍵盤(静電センサー)とつながっているピン
 int N=9;//音の数
int c[]={0,0,0,0,0,0,0,0,0,0,0};//カウンター、電気発信ピンがHIGHになってから鍵盤ピンがHIGHになるまでの時間を計測
int cc[] = {0,0,0,0,0,0,0,0,0,0,0};//c[]のカウンター、c[]が一定の値を超えた時間を計測
int th[][9]={//閾値、cc[]の値をこの閾値で評価する、鍵盤ごとに設定でき、チャンネルを変えることで全体を変えることができる
  {0,0,0,0,0,0,0,0,0},//0
  {1,1,1,1,0,1,0,0,0},//1
  {2,2,2,2,0,2,0,0,0},//2
  {2,2,2,2,1,2,1,1,1},//3
  {2,2,2,2,2,2,2,2,2},//4
  {3,3,3,3,3,3,3,3,3},
  {4,4,4,4,4,4,4,4,4}
  };
int channel=3;//閾値のチャンネル、リモコンで操作可能、電源の種類によって閾値が変わるのでここで調節
int start[]={20,10};
int stc=0;
int k=0;//無音鍵盤カウンター、無音状態の鍵盤の数を計測
int t=0;//鍵盤HIGHカウンター

//音関連の定数の定義
int me[]={0,0,0,0,0,0,0,0,0,0};//鍵盤ごとの音の高さの設定、setupでfor文を用いるのでここで定義
//int me[]={262,294,330,349,392,440,494,523,587};//基準となるme


int s[]={0,2,4,5,7,9,11,12,14};//ドからの高さ、レは2つ高く、ミは4つ高い、12上がると1オクターブ上がる

 
 int key=37;//基準となる音、37は真ん中のオクターブのド
 //基準key=37;
 
 int o=0;//オクターブの高さ
 

const int duration = 85 ;//音が出る長さ、短くすることで音と音の間隔を短くできる、長いと手を放してもしばらく鳴ってしまう




void setup() {
 
  // シリアル通信設定
  Serial.begin(115200);//シリアル通信を速度115200で開始
  pinMode(out,OUTPUT); // ディジタル出力、電気発信ピンを出力モードに設定する
  for(int i=0;i<N;i++){ pinMode(i,INPUT); }//鍵盤のピンを入力モードに設定する
  irrecv.enableIRIn();
  onkai();//音階の初期設定
}

void loop() {
  
  // カウンタ変数の初期化
  for(int i=0;i<N;i++){c[i]=0;}//カウンターの初期化
   k=0;//無音鍵盤カウンターの初期化
   t=0;
  remote();//赤外線操作の受信・それに応じた数値の操作
  
  
  digitalWrite(out,HIGH);// 8番ピンをHIGHに変化させる
  
SensorHyouka();
C_Hyouka();
CC_Hyouka();
serialprint();

  // 8番ピンをLOWにする
  digitalWrite(out,LOW);
  // 放電するまで待つ
  delayMicroseconds(500);
}










//音階(ド、レ、ミ、ファ、ソ、ラ、シ、ド、レ)の数値を決定する関数
void onkai(){
  for(int i=0;i<N;i++){
    me[i]={melkey[key+o*12+s[i]]};
    }
 }

//センサーを評価する関数
void SensorHyouka(){
  while(1){
    t=0;
   for(int i=0;i<N;i++){
  if (digitalRead(pin_num[i]) != HIGH) {c[i]++;} // カウントアップ
  else{t++;}
   }
   if(t==N){break;}//全ての鍵盤ピンがHIGHになるまで待つ
  }}

//cを評価する関数
void C_Hyouka(){
 for(int i=0;i<N;i++){
  if ( c[i] > th[channel][i]) {//th以上の時
    cc[i]=0;
    }
  else {
    if(cc[i]<99){
      cc[i]++;
      }
      }
    }
  }

//CCを評価し音の出力と停止をする関数
void CC_Hyouka(){
 for(int i=0;i<N;i++){
if(cc[i]<start[stc]){NewTone(BUZZER_PIN,me[i],duration);
Serial.println(me[i]);
}
 }
 
for(int i=0;i<N;i++){if(cc[i]>13){k++;}}

 if(k==N){noNewTone(BUZZER_PIN);}
 }



 

 //リセット関数
 void(*resetFunc) (void) = 0;


 //リモコン操作に関する関数
void remote(){
  int tmpValue;
  if (irrecv.decode(&results)){ // have we received an IR signal?
  for (int i = 0; i < 23; i++){
      if ((keyValue[i] == results.value) && (i<KEY_NUM)){
        tmpValue = results.value;
        //Serial.println(keyBuf[i]);
        if(results.value==keyValue[0]){ resetFunc();}
        else if(results.value==keyValue[5]){o++;}
        else if(results.value==keyValue[3]){o--;}
        else if(results.value==keyValue[2]){key++;}
        else if(results.value==keyValue[7]){key--;}
        else if(results.value==keyValue[6]){channel--;}
        else if(results.value==keyValue[8]){channel++;}
        else if(results.value==keyValue[9]){stc++;}
        else if(results.value==keyValue[9]){stc--;}
        else if(results.value==keyValue[11]){channel=0;} 
        else if(results.value==keyValue[12]){channel=1;} 
        else if(results.value==keyValue[13]){channel=2;} 
        else if(results.value==keyValue[14]){channel=3;} 
        else if(results.value==keyValue[15]){channel=4;} 
        else if(results.value==keyValue[16]){channel=5;} 
         
       onkai();//音階の数値の変更   
      }
      else if(REPEAT==i){results.value = tmpValue;}
    }
    irrecv.resume(); // receive the next value
  }
 }
 
 
 // 値を転送(デバッグ用)
 void serialprint(){
  
  for(int i=0;i<N;i++){//鍵盤ごとのcとccの数値の表示
  Serial.print(c[i]);
  Serial.print(":");
  Serial.print(cc[i]);
   Serial.print("\t");}

   //キーとオクターブを表示
    Serial.print(key);
    Serial.print("::");
     Serial.print(o);
     Serial.print("\t");

 //無音鍵盤数とチャンネルを表示
      Serial.print(k);
      Serial.print("::");
      Serial.print(channel);
   Serial.print("\n");
   
   }



ヘッダーファイルIR.hを以下に示す.
//IR.h
#ifndef _IR_H
#define _IR_H


#define RECEIVER 14

#define KEY_POWER (0xFFA25D)
#define KEY_FUNC_STOP (0xFFE21D)
#define KEY_VOL_ADD (0xFF629D)
#define KEY_FAST_BACK (0xFF22DD)
#define KEY_PAUSE (0xFF02FD)
#define KEY_FAST_FORWARD (0xFFC23D)
#define KEY_DOWN (0xFFE01F)
#define KEY_VOL_DE (0xFFA857)
#define KEY_UP (0xFF906F)
#define KEY_EQ (0xFF9867)
#define KEY_ST_REPT (0xFFB04F)
#define KEY_0 (0xFF6897)
#define KEY_1 (0xFF30CF)
#define KEY_2 (0xFF18E7)
#define KEY_3 (0xFF7A85)
#define KEY_4 (0xFF10EF)
#define KEY_5 (0xFF38C7)
#define KEY_6 (0xFF5AA5)
#define KEY_7 (0xFF42BD)
#define KEY_8 (0xFF4AB5)
#define KEY_9 (0xFF52AD)
#define KEY_REPEAT (0xFFFFFFFF)
#define KEY_NUM 21
#define REPEAT 22

unsigned long keyValue[]={KEY_POWER,KEY_FUNC_STOP,KEY_VOL_ADD,KEY_FAST_BACK,KEY_PAUSE,KEY_FAST_FORWARD,
                KEY_DOWN,KEY_VOL_DE,KEY_UP,KEY_EQ,KEY_ST_REPT,KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,
                KEY_6,KEY_7,KEY_8,KEY_9,KEY_REPEAT};

char keyBuf[][15]={"POWER","FUNC/STOP","VOL+","FAST BACK","PAUSE","FAST FORWARD","DOWN","VOL-",
                  "UP","EQ","ST/REPT","0","1","2","3","4","5","6","7","8","9"};
#endif


ヘッダーファイルpitches.hを以下に示す.
// pitches.h
//音階の周波数を設定
int melkey[]={
 31,
 33,35,37,39,41,44,46,49,52,55,58,62,
 65,69,73,78,82,87,93,98,104,110,117,123,
 131,139,147,156,165,175,185,196,208,220,233,247,
 262,277,294,311,330,349,370,392,415,440,466,494,//初期状態のオクターブ
 523,554,587,622,659,698,740,784,831,880,932,988,
 1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,1976,
 2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,
 4186,4435,4699,4978};




