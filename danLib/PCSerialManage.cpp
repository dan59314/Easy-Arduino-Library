
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







#include "PCSerialManage.h"



TPCSerialManager::TPCSerialManager()
{
	CreateMembers();
	InitialMembers();
}

TPCSerialManager::~TPCSerialManager()
{
	ReleaseMembers();
}

TPCSerialManager::TPCSerialManager(long BaudRate, void(*pProcessString)(String &str) )
{
	CreateMembers();
	InitialMembers();

	FBaudRate = BaudRate;  
	FOnProcessString = pProcessString;
	
	//this->begin(FBaudRate, FOnProcessString);
}


void TPCSerialManager::CreateMembers()
{

}

void TPCSerialManager::InitialMembers()
{
  FBaudRate = 115200;
  FLineStr = "";
}

void TPCSerialManager::ReleaseMembers()
{

}



void TPCSerialManager::begin(long BaudRate, void(*pProcessString)(String &str))
{
	FBaudRate = BaudRate;
	FOnProcessString = pProcessString;
	
  Serial.begin(BaudRate);
	
  /*char tbs[32]; 有時會造成debug視窗無法 Serial.println() 出訊息的情況
  sprintf(tbs, "PC-Arduino Serial Initial, BaudRate=%lu", FBaudRate);
  Serial.println(tbs); //"PC-Arduino Serial Initial, BaudRate=115200");
  */
  Serial.print("PC-Arduino Serial Initial, BaudRate=");
  Serial.println(FBaudRate);
}

bool TPCSerialManager::Receive_CompleteString(String &str, bool &blHasInput)
{
	 blHasInput = false;
	 
	 while (Serial.available()) {
	 	
	 	blHasInput = true;
	 	 
    //　逐一加入字元直到遇到 \n--------------------
    char inChar = (char)Serial.read();
  
#ifdef debug
    //int number = atoi( &inChar );    
    //Serial.print( number );
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


void TPCSerialManager::DoProcessString(String &lnStr)
{
	if (NULL != FOnProcessString)
	{
		FOnProcessString(lnStr);
  }

}
	

bool TPCSerialManager::Check_And_Process_String(String &lnStr)
{
	bool blHasInput;
	
	if ( this->Receive_CompleteString(lnStr, blHasInput) )
	{
    //Serial.println( lnStr );
  
		DoProcessString(lnStr);
		lnStr = "";
	}
	
	return blHasInput; //false;		
}

	

bool TPCSerialManager::Check_And_Process_String()
{
	bool blHasInput;
	
	if ( this->Receive_CompleteString(FLineStr, blHasInput) )
	{
    //Serial.println( FLineStr );
  
		DoProcessString(FLineStr);
		FLineStr = "";
	}
	
	return blHasInput; //false;		
}

void TPCSerialManager::Send_String(String &str)
{
  Serial.println(str);
}


void TPCSerialManager::Send_ByteArray(byte *byteAry, int aLength)
{	
	for (int i=0; i<aLength; i++) Serial.print( (char)byteAry[i] );
	
	/* 下列方法錯誤
	  String aStr="";
	  for (int i=0; i<aLength; i++) aStr=aStr+(char) byteAry[i];
	  Serial.print( aStr );
	  
	*/
}

void TPCSerialManager::println(String &str)
{
	this->Send_String(str);
}


TPCSerialManager PCSerialManager(115200, NULL);
