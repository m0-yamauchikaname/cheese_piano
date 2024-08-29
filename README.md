# 静電センサー電子ピアノ
作成日　2023年2月3日
#1．	システムの概要
今回は「一口サイズのメロディー」をテーマにした「静電センサー電子ピアノ」を作成した.作成したシステムを図1に示す.
 
![図1作成したシステム](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig1_1.png)
構成要素を以下に示す.
1.	鍵盤(9個)
2.	ブザー(1個)
3.	LED(1個)
4.	赤外線受信機(1個)
5.	ブレットボードパワーモジュール(電圧を5Vに変換し供給する)

構成要素とシステムを正面から見た図と側面から見た図を図2に示す.
  
![図2システムを正面から見た図(上)横から見た図(下)](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig2_1.png)

![図2システムを正面から見た図(上)横から見た図(下)](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig2.png)

 

システムの概要を以下に示す.
	チーズの外装フィルムでできた静電センサーに触れることで圧電ブザーから音がなり,離すと音が止まる.
	静電センサーは鍵盤の役目を担っているのでセンサーの位置に応じた音階の音が鳴る.さらに音を鳴らしている間LEDが点灯する.
	赤外線受信機を搭載しておりリモコンでボタン入力を行うことで操作することが出来る,表1にリモコンのコマンドをまとめた

![表1　リモコンのコマンド](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/table1.png)

 
 
2．	回路図とその説明
回路図を図3に示す.
 
![図3　システムの回路図表1　リモコンのコマンド](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig3.png)

外部電源から電気を供給し2pinから電気が出力される回路となっている.
3～11pinには抵抗を介して2pinから電流が入力されており,それぞれpinと抵抗の間はアルミ拍につながれている.
13pinにはブザーとLEDが接続されており、13pinから信号が出ることでLEDが光り,ブザーから決められた周波数の音が鳴る.
14pinには赤外線受信機が接続されており,赤外線を受信したとき14pinに信号が送られる.
アルミ箔を使った静電センサの仕組み
このシステムは静電センサとなる機械を使用しておらず,アルミ箔と1MΩの抵抗を使ってタッチセンサを作った(Ⅰ).
まず図４に示すようにアルミ箔に触れたとき人体はコンデンサーの役目を担うため人体方向にも電気が流れるようになる.
図5に示すように2pinから3~11pinにスタート信号が流れているため,3pinのアルミ箔に触れると2pinがHIGHになってから3pinがHIGHになるのに時間がかかる.
arduinoでは一定以上の高さでないとHIGHと見なさないため,その性質を利用して遅延時間を計測し評価することでアルミ箔に触れているかどうかを判断する.

 
![図4　人体による影響を含めた回路図](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig4.png)


 
![図5　2pinからスタート信号が流れているようす](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig5.png)


 
3．	プログラムとその説明
プログラムはメインのspark_sensor_shisakuとリモコンの設定のヘッダーファイルIRremote.h,音階の周波数をまとめたヘッダーファイルpitches.hで構成されている.全体のフローチャートを以下に示す.
 
![図6　システムのフローチャート](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig6.png)


 
次に自作した関数の説明をする.今回,赤外線通信の評価関数、静電センサーの表関数、Cの評価関数、CCの評価関数、シリアル通信で値を表示する関数を作成した.
まず定義した変数と定数の説明を表2にまとめた.
![表２　定義した変数と定数](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/tabel2.png)

 
今回ピンの数Nは9とする.
関数onkaiは音の高さを設定する関数である,フローチャートを以下に示す.また重要な部分は赤く表示した.
 
![図7　関数onkai() のフローチャート](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig7.png)

関数SensorHhyoukaは静電センサーを評価する関数である,図8にSensorHyouka()のフローチャートを示す.
 
![図8　関数Sensor_Hyouka()のフローチャート](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig8.png)

関数¬ C_Hyoukaはcを評価する関数である, C_Hyouka()のフローチャートを図9に示す.

 
![図9　関数C_Hyouka()のフローチャート](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig9.png)


 
関数¬ CC_Hyoukaはccを評価する関数である, CC_Hyouka()のフローチャートを図10に示す.
 
![図10 CC_Hyouka()のフローチャート](https://github.com/m0-yamauchikaname/cheese_piano/blob/main/fig/fig10.png)


今回赤外線通信を行うIRremoteライブラリと,音を出力するToneライブラリが同じarduinoのタイマーを使用するため同時に使用することができない.
そのためToneライブラリとは違うタイマーを使用するNweToneライブラリをつかうことで解決した(Ⅱ).
参考文献
(Ⅰ)「Arduio タッチセンサ」
https://101010.fun/iot/arduino-diy-touch-sensor.html

(Ⅱ)「NewToneライブラリについて」
https://freelance-sakamoto.com/arduino_newtone/
 


