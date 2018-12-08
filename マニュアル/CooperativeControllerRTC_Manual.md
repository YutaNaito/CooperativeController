


# 複数ロボットのための協調制御RTCユーザーマニュアル_Ver1.0


## 内容
1. 概要
    1. はじめに
    1. 開発・動作環境
    1. 入力装置
    1. 移動作業ロボット
    1. クライアント協調型フレキシブル遠隔操作システムの構成
    1. RTシステム構成
1. 複数ロボットのための協調制御RTC
1. 利用方法
    1. 用意するハードウェア
    1. 動作環境
    1. 利用手順
    1. ビルドの手順
1. 使用ソフトウェア入手方法
1. 参考文献
***

## 1. 概要
### 1.1. はじめに
近年，人が立ち入るには危険な場所において，遠隔でロボットが作業することが求められている．しかし，遠隔操作には遠隔通信で行うための特別な機材・システム構築及び各ロボットの制御プログラムの調整が必要となる．そこで，ロボットサービス用の共通通信規格であるRSNP(Robot Service Network Protocol)[1]とロボットシステムの開発に標準化されたソフトウェアプラットフォームであるRTミドルウェア[2]を用いることで比較的容易に遠隔操作システムの開発が可能である．また，遠隔で作業を行うためには，遠隔地で移動するための移動台車や実際に作業を行うためのロボットアーム，遠隔地の状況を把握するためのカメラなどが必要となり，これらを統合したシステムを一から構築するには，期間及びコストがかかる．しかし，既存のロボットアームや移動台車などの遠隔操作システムを合体させることで，遠隔地の状況に合わせてシステムを組み合わせ臨機応変に対応できると考える．先行研究では，各システムが独立しており，またそれらを合体させることで一つの遠隔操作システムとしても利用可能なクライアント協調型遠隔操作システムの構築を行った[3]．しかし，各ロボット単体では達成できるタスクに限界があるため複数のロボットの協調制御が必要になる．加えて，各ロボットに操作デバイスを用意しては操作者の負担につながる．そこで，本コンテストでは複数ロボットの遠隔操作において一つの操作装置の操作の仕方の違いから自動で制御するロボットを切り替える協調制御のためのRTコンポーネントの開発を実施する．今回は，作業ロボットとして基礎的な移動台車とロボットアームの２台に限定して協調制御を行う．
     
- 協調制御は以下の方針で行う
    - 操縦装置の入力値から操作するロボットを決定
    - 測域センサにより検出した障害物の位置により各ロボットの指令値を補正

<br>

### 1.2. 開発・動作環境
- PC(OS：64bit版Windows10)
- OpenRTM-aist1.1.2(C++，32bit版)
- Visual C++ 2013
- CMake3.5.2
- Python2.7.10
- PyYAML3.10
- doxygen1.8.1
- 修正BSDライセンスを適用． 
     
<br>

### 1.3. リンク
芝浦工業大学　知能機械システム研究室　RTミドルウェア <br>
http://www.sic.shibaura-it.ac.jp/~matsuhir/index.html

<br>
     
### 1.4. 入力装置
図1に操作者側(以降マスタ側)で使用する操縦装置を示す．操縦装置はNovint社のFalcon，3自由度のゲームコントローラーであり，X,Y,Zの3軸の位置及び速度の計測やモータのフィードバック機能による反力提示が可能である．

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/Falcon.png" align="middle">
<br><b>Fig1. ３自由度操縦装</b>
</div>
<br>
<br>

### 1.5. 移動作業ロボット
図2に本システムで使用する遠隔地側(以降スレーブ側)の移動作業ロボットを示す．移動作業ロボットは，移動台車がイクシスリサーチ社のiWs09，4軸のロボットアームがRobotis社のMikataArmおよびWEBカメラがあり，以上の４つから構成されている．移動台車前方には，北陽電機社のLRF(測域センサ)が搭載されており，障害物回避が可能である． 

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/WorkingMobility.png" align="middle">
<br><b>Fig2. 移動作業ロボット</b>
</div>
<br>
<br>

### 1.6 クライアント協調型フレキシブル遠隔操作システムの構成
図3にクライアント協調型フレキシブル遠隔操作システムの概要を示す．各ロボットの制御プログラムやRSNPのクライアントはRTコンポーネント[3]から構成されている．ピックアンドプレイスなどの作業内容によっては移動台車とロボットアームが一体となって動作させる必要があり，その際は測域センサのデータからロボット周辺の障害物の位置によって開発する協調制御RTCで操作するロボットを切り替える協調制御を行う．本コンポーネントは，TCP/IP通信などの無線LAN環境下での使用も可能である．
 
<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/SystemOverView.png" align="middle">
<br><b>Fig3. 各クライアント協調時のシステム概要</b>
</div>
<br>
<br>

また図4のように各クライアントでロボット単体での遠隔操作システムとして使用することが可能である．組み合わせてロボット一体型の遠隔操作システムとしても扱うこともできる．

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/EachClientSystemView.png" align="middle">
<br><b>Fig4. 各クライアント単体でのシステム概要</b>
</div>
<br>
<br>

### 1.7　RTシステム構成
図5と図6に既存のRTC群を使用したクライアント協調型フレキシブル遠隔操作システムの構成図を示す．システムは操作者が操作装置を扱うマスタ側と遠隔地でロボットを制御するスレーブ側に分かれている．
     
#### マスタ側
<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/SystemConfigurationMaster.png" align="middle">
<br><b>Fig5. RTシステム構成(マスタ側)</b>
</div>
<br> 
<br>

<div align="center">
<b>Table1. RTC概要(マスタ側)</b>

|RTC名称|用途|
|:--|:--|
|MobileRobotMasterClient1()|RSPNサーバーとの通信及びデータの送受信用|
|FalconController()|操作装置入力用|

</div>
<br>
<br>
マスタ側は２つのRTコンポーネントから構成されている．２つのコンポーネントは先行研究で開発されたRTCを再利用したものである．FalconControllerで操作装置のグリップ部の３自由度の位置(X[m]，Y[m]，Z[m])とボタンの入力値を読み取り，その値からロボットアームと移動台車への指令値を生成する．そして，各ロボットの指令値と操作装置の入力値をMobileRobotMasterClientからRSNPサーバーに送信する．
<br>

#### スレーブ側
<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/SystemConfigurationSlave.png" align="middle">
<br><b>Fig6. RTシステム概要(スレーブ側)</b>
</div>
<br>
<br>

<div align="center">
<b>Table2. RTC概要(スレーブ側)</b>

| RTC名称 | 用途 |
|:--|:--|
|URG()|測域センサからのデータ取得|
|UrgController()|測域センサからのデータ補正用|
|iXsRSNPClient()|RSNPサーバーとの通信及びデータの送受信用|
|RTC_iWs09()|移動台車の制御用|
|MikataArmRSNPClient()|RSNPサーバーとの通信及びデータの送受信用|
|MikataArmKinema()|ロボットアームの指令値生成用|
|MikataArmController()|ロボットアームの制御用|
|CameraRSNPClient()|RSNPサーバーとの通信及びデータの送受信用|
|Screen_capture()|画像取得用|
|CooperativeController()|協調制御用|

</div>
<br>
<br>
スレーブ側は10個のRTコンポーネントから構成されている．CooperativeController以外は先行研究で開発されたRTCを再利用したものである．主にロボットアームを制御するロボットアーム制御RTC群，移動台車を制御する移動台車制御RTC群，カメラ画像を取得するカメラRTC群からなっている．各RSNPクライアントRTコンポーネントでRSNPサーバーとの通信やマスタ側とのデータの送受信を行う．ロボットアーム制御RTC群では，MikataArmRSNPClientがRSNPサーバーからロボットアームの指令値(手先の位置X[m]，Y[m]，Z[m]，Pitch[rad])を受け取り，MikataArmKinemaを通して各関節の目標角度を算出し，MikataArmControllerでロボットアームを制御する．移動台車制御RTC群では，iXsRSNPClientがRSNPサーバーから移動台車の指令値(直進速度[m/s]，旋回速度[rad/s])を受け取り，RTC_iWs09で移動台車を制御する．カメラ制御RTC群では，Screen_captureで取得した画 像をCameraRSNPClientでRSNPサーバーに送りWebブラウザ上に表示する．

***

## 2. 複数ロボットのための協調制御RTC
    
<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/CooperativeControllerRTC.png" align="middle">
<br><b>Fig7. 協調制御RTC</b>
</div>
<br>
<br>
開発した本RTCは，操作装置の入力値からロボットアームか移動台車どちらの制御を行うかを決定し，測域センサのデータから障害物の検出した位置によってロボットアームと移動台車の指令値に補正を行う ．図8に開発した協調制御RTCの接続イメージを示す．InPortには，ロボットアーム及び移動台車への指令値，そ操作装置の入力値，測域センサのデータを送信するポートを接続する．本稿で使用するロボットアームは４自由度，操作装置は３自由度の操作装置であるが，Configurationの値より操作装置とロボットアームの自由度は変更可能である．操縦装置の自由度は最大で３自由度，ロボットアームの自由度は最大でハンド含めて７自由度である．操縦装置は本稿で使用している３自由度の装置だけでなく，２自由度であるジョイスティックなどの操縦装置でも使用可能である．OutPortには，ロボットアームと移動台車を制御するRTコンポーネントで指令値を受信するポートを接続して使用する．表3，4，5にInPort，OutPort，Configurationの概要を示す．
<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/CooperativeControllerRTCConnectionImage.png" align="middle">
<br><b>Fig8. 協調制御RTCの接続イメージ</b>
</div>
<br>
<br>

<div align="center">
<br><b>Table3. 協調制御RTC概要(InPort)</b>

|InPort名称|データ型|機能<br>データの例|
|:--|:--|:--|
|RobotArm|TimedDoubleSeq|ロボットアームの目標手先位置[m]及び目標姿勢[rad]，ハンドの目標開閉角度[rad]を取得．自由度はハンドを含めて最大で7．<br><br>RobotArm.data[0] = 手先目標位置X<br>RobotArm.data[1] = 手先目標位置Y<br>RobotArm.data[2] = 手先目標位置Z<br>RobotArm.data[3] = 手先目標姿勢Roll<br>RobotArm.data[4] = 手先目標姿勢Pitch<br>RobotArm.data[5] = 手先目標姿勢Yaw<br>RobotArm.data[6] = ハンドの開閉角度
|MobileRobot|TimedVelocity2D|移動台車への直進速度[m/s]及び旋回角速度[rad/s]を取得．TimedVelocity2D.data.vyは使用しない．<br><br>TimedVelocity2D.data.vx = 直進速度<br>TimedVelocity2D.data.va = 旋回角速度|
|LRF|RangeData|測域センサのレーザーn本の各長さ[mm]を取得<br>LRF.ranges.data[0] = レーザーの長さ<br>≀<br>LRF.ranges.data[n] = レーザーの長さ|
|ControlleValue|TimedDoubleSeq|操作装置の入力値を取得．操縦装置の自由度は最大で３自由<br><br>ControlleValue.data[0] = X軸方向の入力[m]<br>ControlleValue.data[1] = Y軸方向の入力[m]<br>ControlleValue.data[2] = Z軸方向の入力[m]|

</div>
<br>

<div align="center">
<br><b>Table4. 協調制御RTC概要(OutPort)</b>

|OutPort名称|データ型|機能|
|:--|:--|:--|
|CooperativeArm|TimedDoubleSeq|補正したロボットアームの手先の目標位置[m]及び姿勢[rad]，ハンドの開閉角度[rad]を出力<br><br>CooperativeArm.data[0] = 手先補正位置X[m]<br>CooperativeArm.data[1] = 手先補正位置Y[m]<br>CooperativeArm.data[2] = 手先補正位置Z[m]<br>CooperativeArm.data[3] = 手先補正姿勢Roll[rad]<br>CooperativeArm.data[4] = 手先補正姿勢Pitch[rad]<br>CooperativeArm.data[5] = 手先補正姿勢Yaw[rad]<br>CooperativeArm.data[6] = ハンドの補正開閉角度[rad]|
|CooperativeMobile|TimedVelocity2D|補正した移動台車への直進速度[m/s]及び旋回角度[rad]を出力<br><br>TimedVelocity2D.data.vx = 直進速度[m/s]<br>TimedVelocity2D.data.va = 旋回角速度[rad/s]|

</div>
<br>

<div align="center">
<br><b>Table5. 協調制御RTC概要(Configuration)</b>

|Configuration名称|データ型|機能|
|:--|:--|:--|
|inputDOF|int|操作装置の自由度|
|armDOF|int|ロボットアームの自由度|
|offset|double|制御ロボット切り替え時の入力値のオフセット|

</div>
<br>

ピックアンドプレイスなどの位置決め精度が求められる作業とそうでない作業では，操作者の各ロボットの操作量に違いが発生する．移動台車の場合，目標位置周辺への移動のみで高い精度を求められることは少ないが，ロボットアームの場合は実際に作業を行う場面が多く，物体の把持など高い精度が求められる．そのため実際に作業を行う際，ロボットアームと移動台車の操作では，慎重に動作させる必要があるロボットアームの方が移動台車に比べて操作の変化量が小さくなると考えられる．図9に熟練者がロボットアームと移動台車を操作した際の，操作装置のグリップ部の位置の変化つまり速度を示す．図からも変化の傾きに差があることが分かる．

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/InPutValue.png" align="middle">
<br><b>Fig9. 移動台車操作時(左)とロボットアームの操作時(右)の操作装置の値</b>
</div>
<br>
<br>

そこで，本RTCでは操作装置より受信したハンドル部の3軸の位置(X[m]，Y[m]，Z[m])の変化量，つまり速度を求め，ローパスフィルタとして移動平均を行った速度から最小二乗法で逐次傾きを求める．そして，評価関数として利用するシグモイド関数に代入して得た値から制御するロボットを決定する．以下に示すシグモイド関数はa>0に対してa→∞のときステップ関数に近づく．この性質を利用することで操作装置の入力から，操作量の変化の小さいロボットアームとそうでない移動台車を判別し制御するロボットを決定する．

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/Sigmoid.png" size="1" align="middle">
</div>
<br>

更に，移動台車に搭載されている測域センサにより前方180度範囲で障害物を検出し，検出した障害物の位置によって移動台車とロボットアームの制御に補正を行う．図10に示す②の範囲に障害物がある場合は，ロボットアームの手先位置の2軸(X，Z)のみの動作と移動台車の旋回のみを行い，①の範囲に障害物がある場合はロボットアームのみの制御に切り替わる．①と②の範囲に障害物を検出しない場合は，移動台車のみの制御を行う．以上の制御により一つの操縦装置で移動台車とロボットアームの協調制御を行い，移動台車が障害物に接触することなく操作することが可能である．なお，①と②はロボットアームの動作範囲から決定したものである．

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/ObstacleDetectArea.png" align="middle">
<br><b>Fig10. 測域センサの障害物検出範囲</b>
</div>
<br>
<br>

<div align="center">
<b>Table6. 測域センサによる補正内容]()</b>

|障害物の位置|指令値の補正内容|
|:--:|:--|
|①|移動台車の指令値を全て0に補正|
|②|移動台車の直進成分とロボットアームのY軸方向の指令値を0に補正|
|①と②以外|ロボットアームの指令値を全て0に補正|

</div>
<br>

***

## 3. 利用方法

### 3.1 用意するハードウェア
以下に本稿で使用するハードウェア一覧を示す．

<div align="center">
<b>Table7. 用意するハードウェア</b>

|名称|数量|
|:--|:--|
|操作装置|1|
|移動作業ロボット|1|
|Webカメラ|1|
|測域センサ|1|
|ノートPC|2|
|サーバーPC|1|
|モバイルルーター|1|
|バッテリー|1|

</div>

### 3.2 動作環境
以下に本稿で使用する遠隔操作システムの動作環境を示す．
     
- サーバー
     - サーバーPC:OS Windows7
     
- マスタ側クライアント
     - クライアントPC:OpenRTM-aist1.1.2がインストールされており使用できるOS
     - 実行環境:wwwブラウザ Google chrome(HTML5対応)
     
- スレーブ側クライアント
     - クライアントPC:OpenRTM-aist1.1.2がインストールされており使用できるOS

### 3.3 利用手順

#### 1. PCの起動
マスタ側とスレーブ側，RSNPサーバーに使用するPCの電源を入れる
     
#### 2. ロボットの電源
移動台車の電源を入れ，Webカメラ，測域センサ，ロボットアームの各USBを制御用PCに接続する．
     
#### 3. RSNPサーバーの起動
Eclipseを起動してRSNPサーバーのウィンドウから使用するサーバーの名前を選択し，「サーバーを起動」ボタンをクリックする．

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/ServerWindow.png" align="middle">
<br><b>Fig11. サーバーウィンドウ</b>
</div>
<br>
<br>

#### 4. クライアントの準備
マスタ側とスレーブ側のPCでStartNamingServiceを起動する．そして，Eclipseを起動してネームサーバーをlocalhostに接続する．マスタ側のPCでは以下のRTコンポーネントを起動する．
- MobileRobotMasterClient　→　MobileRootMasterClient
- FalconController　→　FalconController.exe
    
スレーブ側のPCでは開発した協調制御RTコンポーネントを含む以下のRTコンポーネントを起動する．
- URG　→　urgcomp.exe
- UrgController　→　UrgControllerComp.exe
- iXsRSNPClient　→　iXsRSNPClient.bat
- RTC_iWs09　→　RTC_iWs09.exe
- MikataArmRSNPClient　→　MikataArmRSNPClient.bat
- MikataArmKinema　→　MikataArmKinemaComp.exe
- MikataArmController　→　mikataarmcontrollercomp.exe
- CameraRSNPClient　→　CameraRSNPClient.bat
- Screen_capture　→　Screen_capture.bat
- CooperativeController　→　cooperativecontroller.exe(今回開発した協調制御RTC)

#### 5. SystemEditorを完成させる

4で起動したRTコンポーネントをSystem Diagram上にドラッグ＆ドロップし，図5と図6のように各RTコンポーネントを接続する．
     
#### 6. コンフィギュレーション値の設定
開発したCooperativeControllerのinputDOFとarmDOFは，操作装置とロボットアーム(ハンドも含む)の自由度の設定である．本システムでは，３自由度の操作装置に5自由度のロボットアーム(ハンドの開閉も含む自由度)を使用するが，接続する操作装置とロボットアームに合わせて変更する．また，各RSNPクライアントのコンフィギュレーション値はRSNPサーバーとなるPCに合わせて変更する．EndPointは以下のようになっている．[IP]にはサーバーとなるPCのIPアドレスを入力し，[Port]にはサーバー設定時に定めたPort を入力する．

                   http:// [IP][Port]/Falcon_master_slave/services

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/ConfigurationSetupWindow.png" align="middle">
<br><b>Fig12. コンフィギュレーション値設定画面</b>
</div>
<br>
<br>

#### 7. システムの起動
マスタ側とスレーブ側のRTコンポーネントをActivateすることで，クライアントがサーバーに接続され遠隔操作が可能となる．
<br>
<br>

### 3.4 ビルドの手順

#### 1. ファイルのダウンロード
下記URLより協調制御RTCのフォルダをダウンロードし，任意のディレクトリに配置す
る．
<div align="center">https://github.com/YutaNaito/CooperativeController</div>

#### 2. CMake
CMakeを開き，ダウンロードしたファルダから「CMakeLists.txt」をCMakeのウィンドウ
にドラッグ＆ドロップする．そして，「Where to build the binaries」のディレクトリに「/build」を加える．「Configure」，「Generate」の順にそれぞれ成功したらクリックして実行しいく．開発環境の選択ではVisual Studio 12 2013を指定する．
<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/CMakeImage.PNG" align="middle">
<br><b>Fig13. CMakeの手順</b>
</div>
<br>
<br>

<div align="center">
<img src="https://github.com/YutaNaito/CooperativeController/blob/master/ManualPicture/CMakeImage2.PNG" align="middle">
<br><b>Fig14. 開発環境の選択</b>
</div>
<br>
<br>

***

## 4. 使用ソフトウェア入手方法
- OpenRTM-aist1.1.2
    - http://openrtm.org/

- RSNP ライブラリ v2.3（RSi）
- rsnp-robot-api-2.3.0_r49.jar 
- rsnp-robot-fjlib-2.3.0_r49.jar
    - http://robotservices.org/index.php/aboutrsnp/library/
（上記サイトを参考に書面またはメールで申し込みが必要) 

- Apache Tomcat v7.0.55
    - http://tomcat.apache.org/ 

- Axis2 v1.4.1
    - http://axis.apache.org/axis2/java/core/

- Java SE Development Kit v1.7.0_65 (JDK) 
    - http://www.oracle.com/technetwork/opensource/index.html

- Eclipse IDE for Java EE Developers v4.3 
    - http://www.eclipse.org/downloads/packages/eclipse-ide-java-ee-developers/junosr1

- apache-mime4j-0.6.jar
    - http://james.apache.org/mime4j/ 

- commons-codec-1.3.jar
    - http://commons.apache.org/proper/commons-codec/ 

- commons-logging-1.1.1.jar
    - http://commons.apache.org/proper/commons-logging/ 

- httpclient-4.0.jar，httpcore-4.0.1.jar，httpmime-4.0.jar 
    - http://hc.apache.org/downloads.cgi

- jquery-2.1.1.min.js 
    - http://jquery.com/download/

- OpenRTM-aist-1.1.2.jar
    - http://www.openrtm.org/openrtm/ja/content/openrtm-aist-official-website


***

## 5. 参考文献
> [1]Robot Service Network Protocol2.2 仕様書 2010
 
>  [2]OpenRTM-aist Webページ「http://openrtm.org/」最終閲覧日　2018年8月25日
 
> [3]内藤佑太, 松日楽信人：”RSNPクライアント協調型フレキシブル遠隔操作システム”, 第
 36回日本ロボット学会学術講演会2018, 1H2-02, 2018.
