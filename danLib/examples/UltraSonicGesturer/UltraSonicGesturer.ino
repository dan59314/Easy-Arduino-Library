
/* -----------------------------------------------------------------------------
  Copyright: (C) Daniel Lu, RasVector Technology.

  Email : dan59314@gmail.com
   Web :     http://www.rasvector.url.tw/
  YouTube : http://www.youtube.com/dan59314/playlist

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
  Web :     http://www.rasvector.url.tw/
  YouTube : http://www.youtube.com/dan59314/playlist

  使用或修改軟體，請註明引用出處資訊如上。未經過作者明示同意，禁止使用在商業用途。
*/



#define UseBuzzer
#define UseUltraSonic
#define debug


#ifdef UseUltraSonic
#include <UltraSonicManage.h>
#endif
#ifdef UseBuzzer
#include <BuzzerPlay.h>
#include <BuzzerNoteDefine.h>
#endif



// Constants ---------------------------------------------
#ifdef UseBuzzer
const int cBuzzerPin = 7;
#endif
#ifdef UseUltraSonic
const int cTrigPin = 12;
const int cEchoPin = 13;
#endif

// Variables -------------------------------------
#ifdef debug
char sPrnt[64];
String sStates[7] = {"wsNone", "wsWaveIn", "wsWaveOut", "wsWave", "wsHold", "wsNearToFar", "wsFarToNear"};
#endif
#ifdef UseBuzzer
byte gTempo = cNormalTempo;
#endif
#ifdef UseUltraSonic
float gDistance;
#endif

/*
void Initial_Pins(int trigPin, int echoPin)
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}*/



void BuzzerPlay(unsigned short aFrequency, unsigned short aDuration)
{
#ifdef UseBuzzer
  //BuzzerPlayer.Play_Tone( aFrequency, aDuration );
#endif
}




#ifdef UseUltraSonic
// Triggered if WaveEven ---------------------------------
void Process_WaveEvent(TUltraSonicGesturer*, TWaveState ws, int aCnt, float frDistCM, float toDistCM)
{
#ifdef debug
    sprintf(sPrnt, ", N:%d, D: ",  aCnt);
    Serial.print(sStates[(int)ws]);
    Serial.print(sPrnt);
    Serial.print(frDistCM);
    Serial.print(" -> ");
    Serial.println(toDistCM);
#endif    

  switch (ws)
  {
    case wsWaveIn:  //手勢揮入時,相當於MouseDown()
  		// Add your own code here.  在這裡加上你的 Code ........... 
      BuzzerPlay(Do1, 50 );
      break;
      
    case wsWaveOut://手勢揮出時,相當於 MouseUP()
  		// Add your own code here.  在這裡加上你的 Code ........... 
      BuzzerPlay(Do1, 50 );
      break;
      
    case wsWave://手勢揮出入時，相當於 MouseClick()
  		// Add your own code here.  在這裡加上你的 Code ........... 
       for (int i=0; i<aCnt; i++)
         BuzzerPlay(Do4, 100 );
      break;

    case wsHolding:  // 手勢進入超音波模組範圍不動時觸發
  		// Add your own code here.  在這裡加上你的 Code ........... 
      BuzzerPlay( 3000-aDistCM*20, 50 );
      break;
      
    case wsHold: //手勢進入超音波模組範圍時觸發
  		// Add your own code here.  在這裡加上你的 Code ........... 
       BuzzerPlay(Do4, 250 );
      break;
      
    case wsNearToFar:// 手勢進入超音波範圍，且由近到遠離開時觸發
  		// Add your own code here.  在這裡加上你的 Code ........... 
       BuzzerPlay(Do6, 250 );
      break;
      
    case wsFarToNear:// 手勢進入超音波範圍，且由遠到近靠近時觸發
  		// Add your own code here.  在這裡加上你的 Code ........... 
       BuzzerPlay(Do2, 250 );
      break;
  }
}
#endif


void setup() 
{
#ifdef UseBuzzer
  // 蜂鳴器初始化---------------------------------------------------
  BuzzerPlayer.Initial(cBuzzerPin, 3, NULL);
#endif


#ifdef UseUltraSonic
  //超音波初始化 --------------------------------------------
  UltraSonicGesturer.Initial(cTrigPin, cEchoPin, &Process_WaveEvent);
#endif
  
#ifdef debug
  Serial.begin(115200); // Starts the serial communication
#endif
}

void loop() 
{
  //gDistance = UltraSonicGesturer.Distance_CM();
  //if (gDistance<50) BuzzerPlay( 6000/gDistance, 10 );
    
  
#ifdef UseUltraSonic
  // 檢查超音波輸入事件
  UltraSonicGesturer.Check_WaveEvent();
#endif

 #ifdef debug
  //Serial.print("Distance: ");
  //Serial.println(gDistance);
 #endif
}
