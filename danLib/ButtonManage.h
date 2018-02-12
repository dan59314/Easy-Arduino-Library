
/* -----------------------------------------------------------------------------
  Copyright: (C) Daniel Lu, RasVector Technology.

  Email : dan59314@gmail.com
 	Web : 		http://www.rasvector.url.tw/
 	YouTube :	http://www.youtube.com/dan59314/playlist

  This software may be freely copied, modified, and redistributed
  provided that this copyright notice is preserved on all copies.
  The intellectual property rights of the algorithms used reside
  with the Daniel Lu, RasVector Technology.

  You may not distribute this software, in whole or in part, as
  part of any commercial product without the express consent of
  the author.

  There is no warranty or other guarantee of fitness of this
  software for any purpose. It is provided solely "as is".

  ---------------------------------------------------------------------------------
  版權宣告  (C) Daniel Lu, RasVector Technology.

  Email : dan59314@gmail.com
 	Web : 		http://www.rasvector.url.tw/
 	YouTube :	http://www.youtube.com/dan59314/playlist

  使用或修改軟體，請註明引用出處資訊如上。未經過作者明示同意，禁止使用在商業用途。
  
  ----------------------------------------------------------------------------------
  
  
  

  Arduino 函式庫

  功能：
    用來定義 DanLib 內定義資料結構的運作

  注意
    1. 請將所有 XXXLIb 使用到的 自訂資料結構宣告全部放到 CommonTypDefine.pas

    2. 請將所有 Math2DLib 內的Data type 的管理放到 XXXManager 內，包括：
		  <1> 資料的判斷和轉換：EX: IsEqual(), IsEmpty(), RadianToDegree()…..。
		  <2> 資料的資料運作: EX: Add(), Release(), Delete(), Insert()…..。
		  <3> 資料的取得：EX: QSin(), QCos()…..。

    3. 所有資料運作皆以 真實座標系統 ( 右手系統 ) 運作，即
       逆時鐘旋轉時，角度為正， 順時鐘旋轉時角度為負。

		4. 先置換 "DanielClasser" -> 類別名(EX:M2dManager)", 接著置換 "DanielClassFile" -> 檔案名稱(EX: M2dManage)
-------------------------------------------------------------------------

	Timer0 - An 8 bit timer used by Arduino functions delay(), millis() and micros().
	Timer1 - A 16 bit timer used by the Servo() library
	Timer2 - An 8 bit timer used by the Tone() library

	Timer_output Arduino_output  Chip_pin  Pin_name
	OC0A (Timer0)     6             12        PD6
	OC0B (Timer0)     5             11        PD5
	OC1A (Timer1)     9             15        PB1
	OC1B (Timer1)     10            16        PB2
	OC2A (Timer2)     11            17        PB3
	OC2B (Timer2)     3             5         PD3

	使用 tone() 將和 pin 3, pin 11 衝突， pin10,11 通常拿來使用在 SoftSerial
	
	Servo 和 Arduino board 必須分別獨立供電，否則Servo.write() 後會導致arduino 不斷 Reset();
*/



// ButtonManage.h

#ifndef _ButtonManage_h
#define _ButtonManage_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "CommonTypeDefine.h"
#include <BlueToothSerialManage.h>


// 自定義, 共同定義移到 CommonTypeDefine.h　------------------------------------------------
//typedef const unsigned char* PByte;  // 定義　PByte, 指向 byte 的 pointer

/*
enum TDanEnum {
  de01, de02, de03, de04, de05
};

struct TDanRec {

};
*/

enum TButtonState { bsNone=0, bsDown, bsUp, bsClick, bsDoubleClick, bsHolding, bsHold, bsLongHold};


// Constants ------------------------------------------------------------------------------
const int cPressDebounce = 20;          // ms cPressDebounce period to prevent flickering when pressing or releasing the button
const int cDbClk_Interval = 300;            // max ms between clicks for a double click event
const int cWaitAfterBtnUp = cDbClk_Interval+100;
const int cHoldTime = cDbClk_Interval+200;        // ms hold period: how long to wait for press+hold event
const int cLongHoldTime = 3000;    // ms long hold period: how long to wait for press+hold event
const int cMaxContinusClickCount=5;

// Variables ----------------------------------------------------------------------------------




class TButtonManager
{
private:
  byte FMemers1;
  bool FProcessingEvent=false;
  byte FClickCount = 0;
  byte FLongDownCount = 0;
  long FLastClickTime = -1;
  long FLastLongDownTime = -1;
	bool FHasValidBtnDown = false;
	long FBtnDownTime = -1;         // time the button was pressed down
	
	// Button variables
	/*
	bool FLastBtnVal = HIGH;  // buffered value of the button's previous state
	bool FDbClkWaiting = false;  // whether we're waiting for a double click (down)
	bool FDbClkOnUp = false;     // whether to register a double click on next release, or whether to wait and click
	bool FSingleClkOK = true;    // whether it's OK to do a single click
	long FBtnUpTime = -1;           // time the button was released
	bool FIgnoreUp = false;   // whether to ignore the button release because the click+hold was triggered
	bool FWaitForUp = false;        // when held, whether to wait for the up event
	bool FIsBtnHold = false;    // whether or not the hold event happened already
	bool FIsBtnLongHold = false;// whether or not the long hold event happened already
	*/

  void(*FOnButtonEvent)(TButtonManager*, int buttonPin, TButtonState btnState, int clickCnt); /*TButtonManager　的　Class callback function*/
protected:
  void CreateMembers();
  void InitialMembers();
  void ReleaseMembers(); 
  
  bool IsValid_DebounceInterval(long dMsec);
  bool IsValid_DbClickInterval(long dMsec);
  bool IsValid_HoldInterval(long dMsec);
  
  bool GetButtonState(int buttonPin, TButtonState &bs, int &clickCnt);
  bool GetButtonState_New(int buttonPin, TButtonState &bs, int &clickCnt);
public:

  TButtonManager();
  ~TButtonManager();

  TButtonManager( void(*pButtonEvent)(TButtonManager*, int buttonPin, TButtonState btnState, int clickCnt)=NULL); /* overload constructor pButtonEvent(sender, irCode) */

  void Initial(
  	void(*pButtonEvent)(TButtonManager *, int buttonPin, TButtonState btnState, int clickCnt)=NULL);
  
  bool Check_ButtonEvent(int buttonPin);
  
  void Run(void); /* 執行 Run指令*/
};





extern TButtonManager ButtonManager;

#endif


