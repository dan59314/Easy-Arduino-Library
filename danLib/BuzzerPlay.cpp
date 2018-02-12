
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







#include "BuzzerPlay.h"



TBuzzerPlayer::TBuzzerPlayer()
{
	CreateMembers();
	InitialMembers();
}

TBuzzerPlayer::~TBuzzerPlayer()
{
	ReleaseMembers();
}

TBuzzerPlayer::TBuzzerPlayer(int buzzerPin, byte aTempo=cNormalTempo,  void(*pOnPlayNote)(TBuzzerPlayer *, byte aNote, byte aBeat, bool &abortPlay))
{
	CreateMembers();
	InitialMembers();

	FBuzzerPin = buzzerPin;
  pinMode(FBuzzerPin, OUTPUT);
  
	FTempo = aTempo;
	
	FOnPlayNote = pOnPlayNote;
}


void TBuzzerPlayer::CreateMembers()
{

}

void TBuzzerPlayer::InitialMembers()
{

}

void TBuzzerPlayer::ReleaseMembers()
{

}


void TBuzzerPlayer::Initial(int buzzerPin, byte aTempo=cNormalTempo,  
  	void(*pOnPlayNote)(TBuzzerPlayer *, byte aNote, byte aBeat, bool &abortPlay))
{
	FBuzzerPin = buzzerPin;
	
  pinMode(FBuzzerPin, OUTPUT);
  
	Set_Tempo( aTempo );
	
	FOnPlayNote = pOnPlayNote;
}


void TBuzzerPlayer::Do_Tone(int buzzerPin, unsigned short aFrequency, unsigned short aDuration)
{
	tone(buzzerPin, aFrequency, aDuration);
  	
  // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  int pauseBetweenNotes = aDuration * 1.30;
  delay(pauseBetweenNotes);
   
  noTone(buzzerPin);
}


void TBuzzerPlayer::Do_Tone1(int buzzerPin, unsigned short aFrequency, unsigned short aDuration)
{
	long nFrequency= 3000-aFrequency, nDuration=aDuration*1000L;

	//Serial.print(nFrequency); Serial.print(", "); Serial.println(aDuration);
	
	for (long i = 0; i < nDuration; i += nFrequency*2) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(nFrequency);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(nFrequency);
  }
  
  delay( 50 );  
  //noTone(buzzerPin);
}


void TBuzzerPlayer::Play_Tone(unsigned short aFrequency, unsigned short aDuration)
{
  	//Do_Tone1(FBuzzerPin, aFrequency, aDuration); //無法解決 pin3, pin11 和tone()共用衝突的問題
  	Do_Tone(FBuzzerPin, aFrequency, aDuration);
}
	
	
void TBuzzerPlayer::Play_Note(TToneNote aNote, TToneBeat aBeat)
{	  
   if ( (NULL!=FOnPlayNote) ) 
   {     
   		FOnPlayNote( this, aNote, aBeat, FAbortPlay);
   		if (true==FAbortPlay) return;
   }
      
   
  int nDuration = aBeat*cMsecPerBeat/cTempoDiv*FTempo;
    
  if (aNote == cRest) 
  {
     delay(nDuration); // rest
  } 
  else 
  {
  	Play_Tone(cFrequency[aNote], nDuration);
  }

  // pause between notes
}


void TBuzzerPlayer::Play_Song(TToneNote aNotes[], TToneBeat aBeats[], int noteCnt) 
{    
	FAbortPlay = false;
	
  for (int i = 0; i < noteCnt; i++) 
  {
    Play_Note( aNotes[i], aBeats[i]);
    
    if (true==FAbortPlay) return;
  }
    
  
}

void TBuzzerPlayer::Set_Tempo(byte aTempo)
{
	FTempo = constrain(aTempo, cMinTempo, cMaxTempo);
}

void TBuzzerPlayer::Increase_Tempo(byte stepVal)
{
	int aTempo = FTempo - stepVal;
	
	FTempo = max(cMinTempo, aTempo);
}

void TBuzzerPlayer::Decrease_Tempo(byte stepVal)
{
	int aTempo = FTempo + stepVal;
	
	FTempo = min(cMaxTempo, aTempo);
}

  
  
void TBuzzerPlayer::Run()
{
}



TBuzzerPlayer BuzzerPlayer(7,cNormalTempo,NULL);
