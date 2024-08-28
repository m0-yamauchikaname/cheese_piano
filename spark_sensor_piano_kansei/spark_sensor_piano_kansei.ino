S#include "IR.h"

#include "canon.h"
#include "IRremote.h"
#include <NewTone.h>

//シリアル通信の準備
IRrecv irrecv(RECEIVER); 
decode_results results; 
int music=0;
//各定数の定義
int out=3;//電気を発信するピン
//const int BUZZER_PIN=13;//ブザーとLEDに接続するピン
const int light_PIN=2;
int pin_num[]={4,5,6,7,8,9,10,11,12};//鍵盤(静電センサー)とつながっているピン

 int N=9;//音の数
int c[]={0,0,0,0,0,0,0,0,0,0,0};//カウンター、電気発信ピンがHIGHになってから鍵盤ピンがHIGHになるまでの時間を計測
int cc[] = {0,0,0,0,0,0,0,0,0,0,0};//c[]のカウンター、c[]が一定の値を超えた時間を計測
int mode=0;
int th[][9]={//閾値、cc[]の値をこの閾値で評価する、鍵盤ごとに設定でき、チャンネルを変えることで全体を変えることができる
  {0,0,0,0,0,0,0,0,0},//0
  {1,1,1,1,0,1,0,0,0},//1
  {2,2,2,2,0,2,0,0,0},//2
  {2,2,2,2,2,1,1,1,1},//3
  {2,2,2,2,3,1,1,1,1},//4
  {2,2,2,2,4,2,2,2,2},//5
  {3,3,3,3,3,3,3,3,3},//6
  {4,4,4,4,4,4,4,4,4}//7
  };
  int ccth[9]={20,20,20,20,20,20,20,20,50};
int channel=5;//閾値のチャンネル、リモコンで操作可能、電源の種類によって閾値が変わるのでここで調節
int start[]={20,10};
int stc=0;
int k=0;//無音鍵盤カウンター、無音状態の鍵盤の数を計測
int t=0;//鍵盤HIGHカウンター


 
int light_time=0;
int light_on=0;
int light_time_th=30;
const int duration = 2000 ;//音が出る長さ、短くすることで音と音の間隔を短くできる、長いと手を放してもしばらく鳴ってしまう




void setup() {
 
  // シリアル通信設定
 // Serial.begin(115200);//シリアル通信を速度115200で開始
  pinMode(out,OUTPUT); // ディジタル出力、電気発信ピンを出力モードに設定する
  pinMode(light_PIN,OUTPUT);
  for(int i=0;i<N;i++){ pinMode(i,INPUT); }//鍵盤のピンを入力モードに設定する
  irrecv.enableIRIn();
  onkai();//音階の初期設定
}

void loop() {
  light_time=light_time+light_on;
  if(light_time>light_time_th){
    light_time=0;
    light_on=0;
    digitalWrite(light_PIN,LOW);
  }
  // カウンタ変数の初期化
  for(int i=0;i<N;i++){c[i]=0;}//カウンターの初期化
   k=0;//無音鍵盤カウンターの初期化
   t=0;
  remote();//赤外線操作の受信・それに応じた数値の操作
  
  
  digitalWrite(out,HIGH);// 8番ピンをHIGHに変化させる
  
SensorHyouka();
C_Hyouka();
CC_Hyouka();
//serialprint();

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
//Serial.println(me[i]);
break;
}
 }
 
for(int i=0;i<N;i++){if(cc[i]>ccth[i]){k++;}}

 if(k==N){noNewTone(BUZZER_PIN);}
 }



 




 //リモコン操作に関する関数
void remote(){
  int tmpValue;
  if (irrecv.decode(&results)){ // have we received an IR signal?
  for (int i = 0; i < 23; i++){
      if ((keyValue[i] == results.value) && (i<KEY_NUM)){
        tmpValue = results.value;
        //Serial.println(keyBuf[i]);
        if(results.value==keyValue[0]){ resetFunc();}
        else if(results.value==keyValue[1]){modechange();} 
        else if(results.value==keyValue[4]){Music_Play();} 
        else if(results.value==keyValue[5]){o++;}
        else if(results.value==keyValue[3]){o--;}
        else if(results.value==keyValue[2]){key++;}
        else if(results.value==keyValue[7]){key--;}
        else if(results.value==keyValue[6]){channel--;}
        else if(results.value==keyValue[8]){channel++;}
        else if(results.value==keyValue[9]){stc++;}
        else if(results.value==keyValue[9]){stc--;}
        else if(results.value==keyValue[11]){music=0;} 
        else if(results.value==keyValue[12]){music=1;} 
        else if(results.value==keyValue[13]){channel=2;} 
        else if(results.value==keyValue[14]){channel=3;} 
        else if(results.value==keyValue[15]){channel=4;} 
        else if(results.value==keyValue[16]){channel=5;} 
        else if(results.value==keyValue[17]){channel=6;} 
        else if(results.value==keyValue[18]){channel=7;} 
        else if(results.value==keyValue[19]){music=8;} 
        else if(results.value==keyValue[20]){music=9;} 
         digitalWrite(light_PIN,HIGH);
         light_on=1;
       onkai();//音階の数値の変更   
      }
      else if(REPEAT==i){results.value = tmpValue;}
    }
    irrecv.resume(); // receive the next value
  }
 }
 
 
 // 値を転送(デバッグ用)
/* void serialprint(){
  
  for(int i=0;i<N;i++){//鍵盤のccの数値の表示
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
       Serial.print(":");
      Serial.print(light_time);
        Serial.print(":");
      Serial.print(mode);
             Serial.print(":");
      Serial.print(music);

   Serial.print("\n");
   
   }
*/
void Music_Play(){
 // if(mode==1){
    if(music==0){canon();}
        if(music==1){cantry();}
        if(music==8){canonfull();}
        if(music==9){summer();}
  //}
}
void modechange(){
if(mode==0){mode=1;}
  else if(mode==1){mode=0;}
  }
