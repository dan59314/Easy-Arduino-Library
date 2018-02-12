
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










#include "UltraSonicManage.h"




// ************************************************************************************************
// TUltraSonicManager 
// ************************************************************************************************

TUltraSonicManager::TUltraSonicManager()
{
	CreateMembers();
	InitialMembers();
}

TUltraSonicManager::~TUltraSonicManager()
{
	ReleaseMembers();
}

TUltraSonicManager::TUltraSonicManager(int trigPin, int echoPin,  void(*pGetDistanceFunc)(TUltraSonicManager *, float &distCM))
{
	CreateMembers();
	InitialMembers();
	
	FTrigPin=trigPin;
	FEchoPin=echoPin;

	FOnGetDistance = pGetDistanceFunc;
}


void TUltraSonicManager::Initial(int trigPin, int echoPin, void(*pGetDistanceFunc)(TUltraSonicManager *, float &distCM))
{
	
	FTrigPin=trigPin;
	FEchoPin=echoPin;
	
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
	
	FOnGetDistance = pGetDistanceFunc;
}

void TUltraSonicManager::CreateMembers()
{

}

void TUltraSonicManager::InitialMembers()
{

}

void TUltraSonicManager::ReleaseMembers()
{

}

float TUltraSonicManager::Distance_CM()
{// Clears the trigPin
  digitalWrite(FTrigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(FTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(FTrigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  int aDuration = pulseIn(FEchoPin, HIGH);


  delayMicroseconds(20); //避免不斷移動造成的雜訊

  
  // Calculating the distance
  return aDuration*0.034/2;
}











// ************************************************************************************************
// TUltraSonicGesturer
// ************************************************************************************************

TUltraSonicGesturer::TUltraSonicGesturer():
	//只有constructor 才需要初始化 parent class constructor-------------------------------------------
	TUltraSonicManager() 
{
	CreateMembers();
	InitialMembers();
}

TUltraSonicGesturer::~TUltraSonicGesturer()
{
	ReleaseMembers();
}

TUltraSonicGesturer::TUltraSonicGesturer(int trigPin, int echoPin, void(*pWaveEvent)(TUltraSonicGesturer*, TWaveState waveState, int aCnt, float frDistCM, float toDistCM)):
	 //只有constructor 才需要初始化 parent class constructor------------------------------------
	 TUltraSonicManager(trigPin, echoPin, pWaveEvent)
{
		
	FOnWaveEvent = pWaveEvent;
}


void TUltraSonicGesturer::Initial(int trigPin, int echoPin, 
	  void(*pWaveEvent)(TUltraSonicGesturer *, TWaveState waveState, int aCnt, float frDistCM, float toDistCM))
{
	TUltraSonicManager::Initial(trigPin, echoPin, pWaveEvent);
		
	FOnWaveEvent = pWaveEvent;
}

void TUltraSonicGesturer::CreateMembers()
{

}

void TUltraSonicGesturer::InitialMembers()
{

}

void TUltraSonicGesturer::ReleaseMembers()
{

}


bool TUltraSonicGesturer::IsValid_DebounceInterval(long dMsec, long aDebounceMsec)
{
	return dMsec > aDebounceMsec;
}

bool TUltraSonicGesturer::GetWaveState(TWaveState &vs, int &clickCnt, float &frDist, float &toDist)
{ 
	 vs = wsNone;	 
	 clickCnt =0;
	 frDist=0; toDist = 0.0;
   
	 float curDist = this->Distance_CM_DeNoise();   // value read from WavecurDist = digitalRead(WavePin);

	 bool	blIsOutSide	=	(curDist > FOutMaxDistCM ||  curDist < FOutMinDistCM);
	 long curT = millis();
	 
	 
	 //Serial.println(curDist);
	 

	 if (true==blIsOutSide )
	 // 在範圍外 -----------------------------------------------
	 {
		  if (true==FHasValidWaveIn)
			// 剛剛有 WaveIn, 現在 WaveOut --------------------------------------
	 		{

				if ( IsValid_DebounceInterval(curT - FWaveInTime, cWaveDebounce) )
	 			// WaveIn->WaveOut 時間很短 或者沒有 ClickCount, 且不是雜訊  --------------------------------------------
				{
#ifdef debug
Serial.println("dbgOut-4");
#endif
					if (curT-FLastOutTime > cDbWave_Interval)
					// 離上 click很久了-------------------------
					{
						FClickCount=1;  //重設click count
					}
					else
					// 離上次 click不久，繼續累加 -----------------------
					{
						FClickCount++;
					}
					FLastOutTime = curT;
					FHasValidWaveIn = false;
				}
			}

			int dMsec= curT-FWaveInTime;

			if ( IsValid_DebounceInterval(dMsec, 500) )
			{
				if ( (FClickCount>0 || FLongDownCount>0)  )
				//  且還沒處理 Click Count ---------------------------
				{
					#ifdef debugWaveOut
					vs=wsWaveOut;
					clickCnt = 0;
					#endif

					if (FLongDownCount>0)
					{
#ifdef debug
Serial.println("dbgOut-1");
#endif
						clickCnt = FLongDownCount;
						frDist=FLongDownStartDistCM;
						toDist=FLongDownEndDistCM;
						//FLongDownCount %= (1+cMaxContinusWaveCount);
						float travelDist = toDist-frDist;

						if ( abs(travelDist)> cStepToleranceCM)
						{
							if (travelDist>0.0)
							{
								vs = wsNearToFar;
							}
							else
							{
								vs = wsFarToNear;
							}
						}
						else
						{
							vs=wsHold;
						}
						FClickCount = 0;  //處理hold後，就不處理 click						
					}
					else if (FClickCount>0)
					{
#ifdef debug
Serial.println("dbgOut-2");
#endif
						frDist=FWaveInStartDistCM;
						toDist=curDist;
							//FClickCount %= (1+cMaxContinusWaveCount);
						vs=wsWave;
						clickCnt = FClickCount;
					}
					FClickCount =0;
					FLongDownCount = 0;
				}

				FHasValidWaveIn = false;
			}
	 }
	 else
	 // 在範圍內 ----------------------------------------------------
	 {
			//Serial.println(curDist);

			if (IsValid_DebounceInterval(curT - FLastOutTime, cWaveDebounce))
			// 離上次 wave out 夠久----------------
			{
			if ( (false==FHasValidWaveIn)  )
			// 第一次 Wave In --------------------------------------------
				{
#ifdef debug
Serial.println("dbgIn-1");
#endif
					//vs=wsWaveIn;
					clickCnt = 0;

					frDist = curDist;
					toDist = curDist;

					FHasValidWaveIn = true;
					FWaveInTime = curT; //millis();
					FLastLongDownTime = FWaveInTime;
					FWaveInStartDistCM = curDist;
				}
				else
				// 持續 Wave In -------------------------------------------
				{
					if ( (curT-FLastLongDownTime) > cWaveInHoldTime)
				  // 離上次計算時間夠長，累加 Hold Count ----------------
					{
#ifdef debug
Serial.println("dbgIn-2");
#endif
						FLongDownCount++;

						if (1==FLongDownCount)
						{
							FLongDownStartDistCM=curDist;
							FLongDownEndDistCM=FLongDownStartDistCM;
						}
						else
						{
							FLongDownEndDistCM=curDist;
						}

						FLastLongDownTime = curT;
						
						
						clickCnt = FLongDownCount;
						frDist=FLongDownStartDistCM;
						toDist=curDist;
						vs=wsHolding;
					}
				}
				FLastOutTime = curT;
			}
	 }
	 	
	 return (vs!=wsNone);
}


bool TUltraSonicGesturer::Check_WaveEvent()
{
	
	 if (true==FProcessingEvent) return true;
	 
	 // Get Wave vs and act accordingly
   TWaveState vs=wsNone;
   int clickCnt=0;
   float frDist=0.0, toDist=0.0;
   
   GetWaveState(vs, clickCnt, frDist, toDist);
   
   
   if ( (NULL!=FOnWaveEvent) && (vs!=wsNone) )
   {     
   	 FProcessingEvent = true;
   	 FOnWaveEvent(this, vs, clickCnt, frDist, toDist);
   	 FProcessingEvent = false;
   	 return true;
   }
   
   return false;
}


void TUltraSonicGesturer::Set_WaveRange(float minCM, float maxCM)
{
	FOutMinDistCM = min(minCM, maxCM);
	FOutMaxDistCM = max(minCM, maxCM);
}

float TUltraSonicGesturer::Distance_CM_DeNoise()
{
	return this->Distance_CM();
	
	
	/*
		const float tolCM=3.0;
		const int cSampleCnt = 5;
		
		float samples[cSampleCnt], incMinDist=0.0, incMaxDist=0.0, minDist,maxDist, midDist;
		int numMin=0,numMax=0;
		
		
		minDist=99999; maxDist=-99999;
		
		for (int i=0; i<cSampleCnt; i++)
		{
			samples[i] = this->Distance_CM();
			if (samples[i]<minDist) minDist=samples[i];
			if (samples[i]>maxDist) maxDist=samples[i];
		}
		
		midDist=(minDist+maxDist)/2.0;
		incMinDist=0.0, incMaxDist=0.0; numMin=0,numMax=0;
		for (int i=0; i<cSampleCnt; i++)
		{
			if (samples[i]<minDist) {numMin++; incMinDist+=samples[i];}
			else {numMax++; incMaxDist+=samples[i];}				
		}
		
		
		if (numMin>numMax) return incMinDist/numMin;
	  else return incMaxDist/numMax;
	*/
	  			
}




//TUltraSonicManager UltraSonicManager(12,13,NULL);

TUltraSonicGesturer UltraSonicGesturer(12,13,NULL);
