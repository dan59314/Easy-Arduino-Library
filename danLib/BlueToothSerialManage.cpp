
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







#include "BlueToothSerialManage.h"

#include <SoftwareSerial.h>


TBlueToothSerialManager::TBlueToothSerialManager()
{
	CreateMembers();
	InitialMembers();
}

TBlueToothSerialManager::~TBlueToothSerialManager()
{
	ReleaseMembers();
}

TBlueToothSerialManager::TBlueToothSerialManager(long BaudRate, void(*pProcessString)(String &str) )
{
	CreateMembers();
	InitialMembers();

	FBaudRate = BaudRate;  
	FOnProcessString = pProcessString;
	
	//this->begin(FBaudRate, FOnProcessString);
}


void TBlueToothSerialManager::CreateMembers()
{

}

void TBlueToothSerialManager::InitialMembers()
{
  FBaudRate = 38400;
  FLineStr = "";
}

void TBlueToothSerialManager::ReleaseMembers()
{

}


/*void TBlueToothSerialManager::HelloWorld(TDanEnum de)
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


void TBlueToothSerialManager::begin(long BaudRate, void(*pProcessString)(String &str))
{
	FBaudRate = BaudRate;  
	FOnProcessString = pProcessString;
	
  gBlueToothSerial.begin(FBaudRate);
  
  /*
  char tbs[160];  有時會造成debug視窗無法 Serial.println() 出訊息的情況
  sprintf(tbs, "BlueTooth-Arduino Serial Initial, BaudRate=%lu", FBaudRate);
  gBlueToothSerial.println(tbs); //"BlueTooth-Arduino Serial Initial, BaudRate=38400");
	*/
	
  gBlueToothSerial.print("BlueTooth-Arduino Serial Initial, BaudRate=");
  gBlueToothSerial.println(FBaudRate);
}

bool TBlueToothSerialManager::Receive_CompleteString(String &str, bool &blHasInput)
{
	 blHasInput=false;
	 
	 while (gBlueToothSerial.available()) {
	 	
	 	blHasInput = true;
	 	
    //　逐一加入字元直到遇到 \n--------------------
    char inChar = (char)gBlueToothSerial.read();
  
#ifdef debug
    //int number = atoi( &inChar );    
    //gBlueToothSerial.print( number );
    //if (0==number) return false;
#endif
        
    //　如果字元 = \n 　則跳出 -----------
    if ((inChar == '\r') || (inChar == '\n') || (str.length() > gMaxLineStrLength) ) 
    {
      return (str != "");
    }
    else
    { 
      str += inChar;    
      return false;
    }
  }
  
  return false;
}


void TBlueToothSerialManager::DoProcessString(String &lnStr)
{
	if (NULL != FOnProcessString)
	{
		FOnProcessString(lnStr);
  }

}
	

bool TBlueToothSerialManager::Check_And_Process_String(String &lnStr)
{
	bool blHasInput;
	
	if ( this->Receive_CompleteString(lnStr, blHasInput) )
	{
    //gBlueToothSerial.println( lnStr );
  
		DoProcessString(lnStr);
		lnStr = "";
	}
	
	return blHasInput;		
}

	

bool TBlueToothSerialManager::Check_And_Process_String()
{
	bool blHasInput;
	
	if ( this->Receive_CompleteString(FLineStr, blHasInput) )
	{
    //gBlueToothSerial.println( FLineStr );
  
		DoProcessString(FLineStr);
		FLineStr = "";
	}
	
	return blHasInput; //false;		
}

void TBlueToothSerialManager::Send_String(String &str)
{
	gBlueToothSerial.println(str);
}

void TBlueToothSerialManager::Send_ByteArray(byte *byteAry, int aLength)
{
	for (int i=0; i<aLength; i++) gBlueToothSerial.print( (char)byteAry[i] );
	
	/* 下列方法錯誤
	  String aStr="";
	  for (int i=0; i<aLength; i++) aStr=aStr+(char) byteAry[i];
	  gBlueToothSerial.print( aStr );
	  
	*/
}

void TBlueToothSerialManager::println(String &str)
{
	this->Send_String(str);
}

// BlueTooth in Pin10(RX) Pin11(TX)  PWM Pin -------------------------
#ifdef ReceiveOnly
const byte pinRx = 10, pinTx = 0;
#elif SendOnly
const byte pinRx = 0, pinTx = 11;
#else
const byte pinRx = 10, pinTx = 11;
#endif

SoftwareSerial gBlueToothSerial(pinRx, pinTx); //10, 11); // RX, TX

TBlueToothSerialManager BlueToothSerialManager(38400, NULL);
