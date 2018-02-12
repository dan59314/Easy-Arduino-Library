

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







#include "StringManage.h"

//#define debug

TStringManager::TStringManager()
{
	CreateMembers();
	InitialMembers();
}

TStringManager::~TStringManager()
{
	ReleaseMembers();
}

TStringManager::TStringManager(PByte pActionStart0,  void(*pCallbackFunc)(TStringManager *, byte &irCode))
{
	CreateMembers();
	InitialMembers();

	FOnCallBack_myCheck = pCallbackFunc;
}


void TStringManager::CreateMembers()
{

}

void TStringManager::InitialMembers()
{

}

void TStringManager::ReleaseMembers()
{

}

void TStringManager::DomyCheck()
{
	if (NULL != this->FOnCallBack_myCheck)
	{
		byte irCode = 0;
		FOnCallBack_myCheck(this, irCode);
	}
}

bool TStringManager::Split(String sInput, const char *sDiv, String sCommands[], int cmdLength, byte &cmdNum)
{

	if (sInput == "" || sDiv == "") return false;
	
	char *p = &sInput[0];
	char *str;

	// byte arrayCnt = / sizeof(sCommands) / sizeof(String); 傳回 0，無法傳回真正的陣列長度

	cmdNum = 0;

	//　以 "," 　來拆解字串----------------------------------------
	while (cmdNum< cmdLength && (str = strtok_r(p, sDiv, &p)) != NULL)
	{
		sCommands[cmdNum] = str;
		//sCommands[cmdNum].toUpperCase();
		cmdNum++;
	}


#ifdef debug
	//Serial.println(cmdNum);
#endif

	return (cmdNum>0);
}



bool TStringManager::Split(String sInput, const char *sDiv, String sCommands[], byte &cmdNum)
{
	return this->Split(sInput, sDiv, sCommands, gMaxLineStrLength, cmdNum);
}


void TStringManager::Initial(PByte pRunStart0,  void(*pCallbackFunc)(TStringManager *, byte &irCode))
{
	FOnCallBack_myCheck = pCallbackFunc;


	/*String strs[20];
	byte cmdNum;

	Serial.println(sizeof(strs) / sizeof(String));

	if ( this->Split("The way to a man"s heart is through his stomach.", " ", strs,cmdNum) )
	{
	Serial.print("CmdNum: "); Serial.println(cmdNum);
	for (byte i = 0; i < cmdNum; i++)
	Serial.println(strs[i]);
	}*/
}


int TStringManager::Pos(String subStr, String allStr)
{
	return allStr.indexOf(subStr);
}

bool TStringManager::Get_NextString(String &sMix, String sDivide, int &startID, String &subStr, bool JumpToNextFieldStart=true)
{
  int i,j,sL;
  
  subStr = "";

  sL = sMix.length();
  if (startID>=sL) return false;

  while (startID<sL)
  {
    if (Pos( (String)sMix[startID], sDivide)>=0) // 是 分隔字元, 則往下找
      startID++;
    else
      break;
  }

  i=startID;
  while (i<sL)
  {
    if (Pos((String)sMix[i], sDivide)<0) // sMix[i] 不存在 sDivide 內
      //subStr=subStr+sMix[i] 為了增快速度，不用 字串相加，而以下方 sXXX=Copy() 代替      
    	i++;
		else
      break;
  }  
  
  subStr = sMix.substring(startID, i); // Copy(sMix, startID, i-startID);
  
  /*Serial.print(startID); 
  Serial.print(", ");
  Serial.print(i);
  Serial.print(", ");
  Serial.println(subStr);*/

  startID = i;


  // 找到下一個非 sDiv 字元的開頭 ------------------
  if (true==JumpToNextFieldStart)
  {
    if (i<sL)
    for (j = i; j<sL; j++) 
    if (Pos((String)sMix[j], sDivide)<0) // sMix[i] 不存在 sDivide 內
    {
      startID=j;
      break;
    }
  }
  
  return  (subStr!="");
}


void TStringManager::Run()
{
	//DomyCheck();
}



TStringManager StringManager;
