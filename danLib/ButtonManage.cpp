
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








#include "ButtonManage.h"

#define FixNoiseSignal

// Variables ----------------------------------------------------------------------------------



TButtonManager::TButtonManager()
{
	CreateMembers();
	InitialMembers();
}

TButtonManager::~TButtonManager()
{
	ReleaseMembers();
}

TButtonManager::TButtonManager(void(*pButtonEvent)(TButtonManager*, int buttonPin, TButtonState btnState, int clickCnt))
{
	CreateMembers();
	InitialMembers();

	FOnButtonEvent = pButtonEvent;
}


void TButtonManager::CreateMembers()
{

}

void TButtonManager::InitialMembers()
{

}

void TButtonManager::ReleaseMembers()
{

}

/*void TButtonManager::HelloWorld(TDanEnum de)
{
	switch (de)
	{
	case de01:
		break;
	case de02:
		break;
	case de03:
		break;
	case de04:
		break;
	case de05:
		break;
	}

}*/


void TButtonManager::Initial( void(*pButtonEvent)(TButtonManager *, int buttonPin, TButtonState btnState, int clickCnt))
{
	 FOnButtonEvent = pButtonEvent;	
}

bool TButtonManager::IsValid_DebounceInterval(long dMsec)
{
	return dMsec > cPressDebounce;
}


bool TButtonManager::IsValid_DbClickInterval(long dMsec)
{
	return dMsec >= cDbClk_Interval;
}


bool TButtonManager::IsValid_HoldInterval(long dMsec)
{
	return dMsec >= cHoldTime;
}


bool TButtonManager::GetButtonState_New(int buttonPin, TButtonState &bs, int &clickCnt)
{ 
	 bs = bsNone;	 
	 clickCnt =0;
   
	 bool curBtnVal = digitalRead(buttonPin);   // value read from buttoncurBtnVal = digitalRead(buttonPin);
	 bool	blCurBtnUp	=	(curBtnVal == HIGH);
	 long curT = millis();

	 if (true==FHasValidBtnDown)
	 // 剛剛有 ButtonDown --------------------------------------
	 {	 			
	 	  if (IsValid_DebounceInterval(curT - FBtnDownTime))
	 		if ( true==blCurBtnUp )
	 		// Down -> Up --------------------------------------------
	 		{	 			
	 			//Serial.println("dbg1");	 		
	 			FHasValidBtnDown = false;
	 			
	 			if (curT-FLastClickTime < cDbClk_Interval) 
	 			{
	 				FClickCount++; 	 				
	 			}
	 			else 
	 			{	 			  	 				
	 			  FClickCount=1;
	 			}	 			
	 			FLastClickTime = curT;	 			
	 		}
	 		else if ( (curT-FLastLongDownTime) > cHoldTime) // && digitalRead(buttonPin)==LOW)
	 		// button 持續 down -------------------------------------------
	 		{	 						
	 			//Serial.println("dbg2");	 		
#ifdef FixNoiseSignal
	 			if (FLongDownCount>=cMaxContinusClickCount)
	 			{
	 				FHasValidBtnDown = false;
	 				//FLastClickTime = curT; 會導致 click 失效
	 			}
	 			else
#endif
	 			{	 					
	 				FLongDownCount++;	 				
	 				FLastLongDownTime = curT;
	 				bs=bsHolding;
	 				clickCnt = FLongDownCount;
	 				//if (NULL!=FOnButtonHoldCountIncreasing)
	 				//	FOnButtonHoldCountIncreasing(this, buttonPin, bsHolding,FLongDownCount);
	 			}
	 		}
	 }
	 else
	 	// 剛剛沒有 buttn down ----------------------------
	 {
	 	  int dMsec= curT-FLastClickTime;
	 		if ( (FClickCount>0 || FLongDownCount>0) && dMsec>cWaitAfterBtnUp) 
	 		{	 	  
	 			//Serial.print("dbg3:"); Serial.print(FClickCount); 
	 			//Serial.print(", "); Serial.println(FLongDownCount);
	 		
	 		#ifdef debugButtonUp
	 			bs=bsUp;
	 			clickCnt = 0;
	 		#endif
	 			  
	 	  	if (FLongDownCount>0)
	 	  	{
	 				//FLongDownCount %= (1+cMaxContinusClickCount);
	 				bs=bsHold;
	 				clickCnt = FLongDownCount;
	 	  	}
	 	  	else if (FClickCount>0)
	 	  	{
	 			 	//FClickCount %= (1+cMaxContinusClickCount);
	 			  bs=bsClick;
	 			  clickCnt = FClickCount;
	 	  	}	 	 	
	 	  		 			
	 			FClickCount =0;	 			  
	 			FLongDownCount = 0;
	 	  }	  	 		
	 }	 
	 	 
	 if (true==blCurBtnUp )
	 {
	 		FHasValidBtnDown = false;
	 }	 		
	 else
	 { 	 	 	 
	   if ( (false==FHasValidBtnDown)  )
	 	 {
	 			bs=bsDown;
	 			clickCnt = 0;
	 			
	 			FHasValidBtnDown = true;
	 			FBtnDownTime = millis();
	 			FLastLongDownTime = FBtnDownTime;
	 	 }
	 }
	 	
	 return (bs!=bsNone);
}


bool TButtonManager::GetButtonState(int buttonPin, TButtonState &bs, int &clickCnt)
{ 
	 bs = bsNone;	 
	 clickCnt = 0;
	 
	 /*
   
	 bool curBtnVal = digitalRead(buttonPin);   // value read from buttoncurBtnVal = digitalRead(buttonPin);
	 
   bool blCurBtnDown	=	(curBtnVal == LOW), 
   			blCurBtnUp		=	(curBtnVal == HIGH),
   			blLastBtnUp		=	(FLastBtnVal == HIGH), 
   			blLastBtnDown	=	(FLastBtnVal == LOW);
   
   // Button pressed down
   if (blCurBtnDown && blLastBtnUp && IsValid_DebounceInterval(millis() - FBtnUpTime)) //(millis() - FBtnUpTime) > cPressDebounce)
   // 按鈕 Up->Down 且 
   {
       FBtnDownTime = millis();
       FIgnoreUp = false;
       FWaitForUp = false;
       FSingleClkOK = true;
       FIsBtnHold = false;
       FIsBtnLongHold = false;
       if ((millis()-FBtnUpTime) < cDbClk_Interval && FDbClkOnUp == false && FDbClkWaiting == true)  FDbClkOnUp = true;
       else  FDbClkOnUp = false;
       FDbClkWaiting = false;
   }
   // Button released
   else if (blCurBtnUp && blLastBtnDown && IsValid_DebounceInterval(millis() - FBtnDownTime)) // (millis() - FBtnDownTime) > cPressDebounce)
   {        
       if (not FIgnoreUp)
       {
           FBtnUpTime = millis();
           if (FDbClkOnUp == false) FDbClkWaiting = true;
           else
           {
               bs = bsDoubleClick;
               clickCnt=2;
               FDbClkOnUp = false;
               FDbClkWaiting = false;
               FSingleClkOK = false;
           }
       }
   }
      
   // Test for normal click bs: cDbClk_Interval expired
   if ( blCurBtnUp && IsValid_DbClickInterval(millis()-FBtnUpTime)  //(millis()-FBtnUpTime) >= cDbClk_Interval 
   		  && FDbClkWaiting == true && FDbClkOnUp == false && FSingleClkOK == true && bs!=bsDoubleClick )
   {
       bs = bsClick;
       clickCnt=1;
       FDbClkWaiting = false;
   }
   
   // Test for hold
   if (blCurBtnDown && IsValid_HoldInterval(millis() - FBtnDownTime)) //(millis() - FBtnDownTime) >= cHoldTime) 
   {
       // Trigger "normal" hold
       if (not FIsBtnHold)
       {
           bs = bsHold;
           FWaitForUp = true;
           FIgnoreUp = true;
           FDbClkOnUp = false;
           FDbClkWaiting = false;
           //FBtnDownTime = millis();
           FIsBtnHold = true;
       }
       // Trigger "long" hold
       if ((millis() - FBtnDownTime) >= cLongHoldTime)
       {
           if (not FIsBtnLongHold)
           {
               bs = bsLongHold;
               FIsBtnLongHold = true;
           }
       }
   }
   FLastBtnVal = curBtnVal;
   
	 	
	 return (bs!=bsNone);
	 */
}



bool TButtonManager::Check_ButtonEvent(int buttonPin)
{ 
	 if (true==FProcessingEvent) return true;
	 
	 // Get button bs and act accordingly
   TButtonState bs;
   int clickCnt=0;
   
   GetButtonState_New(buttonPin, bs, clickCnt);
   
   /*
   if (b == 1) clickbs();
   if (b == 2) doubleClickbs();
   if (b == 3) holdbs();
   if (b == 4) longHoldbs();
   */
   
   if ( (NULL!=FOnButtonEvent) && (bs!=bsNone) )
   {     
   	 FProcessingEvent = true;
   	 FOnButtonEvent(this, buttonPin, bs, clickCnt);
   	 FProcessingEvent = false;
   	 return true;
   }
   
   return false;
}

void TButtonManager::Run()
{
}



TButtonManager ButtonManager(NULL);
