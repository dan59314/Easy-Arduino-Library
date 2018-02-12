
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
*/




#define debug


#include <BuzzerPlay.h>
#include <BuzzerNoteDefine.h>
#include <PCSerialManage.h>
#include <BlueToothSerialManage.h>

// Const -----------------------------------------------------
const int cBuzzerPin = 7;

// Variables -------------------------------------------------
#ifdef debug
char sPrnt[32];
#endif
byte gTempo = cNormalTempo, gPriTempo=255;
int iDir=1;
unsigned long gIdleMsec=2000;
unsigned long gLastListerningTime=millis();
const long cCheckInterval_UltraSonic=1000;

// Little Start --------------------------
TToneNote gNotes[] = {
    Do4,Do4,So4,So4,La4,La4,So4, 
    Fa4,Fa4,Mi4,Mi4,Re4,Re4,Do4,
    So4,So4,Fa4,Fa4,Mi4,Mi4,Re4,
    So4,So4,Fa4,Fa4,Mi4,Mi4,Re4,
    Do4,Do4,So4,So4,La4,La4,So4, 
    Fa4,Fa4,Mi4,Mi4,Re4,Re4,Do4, cRest };

TToneBeat gBeats[] = {
    cB1,cB1,cB1,cB1,cB1,cB1,cB2,
    cB1,cB1,cB1,cB1,cB1,cB1,cB2,
    cB1,cB1,cB1,cB1,cB1,cB1,cB2,
    cB1,cB1,cB1,cB1,cB1,cB1,cB2,
    cB1,cB1,cB1,cB1,cB1,cB1,cB2,
    cB1,cB1,cB1,cB1,cB1,cB1,cB2, cB4 };

int length = sizeof(gNotes)/sizeof(TToneNote);  // /sizeof(byte0; 長度會錯誤


bool Check_HasInput(unsigned long &lastListenTime, byte checkCount=1)
{    
  for (int i=0; i<checkCount;i++)
  {
    if (BlueToothSerialManager.Check_And_Process_String())
    {
        lastListenTime = millis();
        return true;
    }
    else if (PCSerialManager.Check_And_Process_String())
    {
        lastListenTime = millis();
        return true;
    }
    else if ( millis()-gLastListerningTime > cCheckInterval_UltraSonic)
    {
  #ifdef EnableButton
      if ( ButtonManager.Check_ButtonEvent(cButtonPin) )
      {
        return true;
      }
  #endif
  #ifdef EnableUltraSonic
      else if (UltraSonicGesturer.Check_WaveEvent() )
      {
        return true;
      }
  #endif
    }
    delay(10);  
  }
   
  return false;
}


void OnPlayNote(TBuzzerPlayer*, byte aNote, byte aBeat, bool &abortPlay)
{
   
   // 檢查一下，是否有輸入 ------------------------
   if ( Check_HasInput(gLastListerningTime) )
   { 
      abortPlay = true;
#ifdef debug
  Serial.println("abortPlay=true");
#endif
      return;
   }
   else
     abortPlay = false;
   
   // 改變燈光顏色
  
#ifdef debug
   sprintf(sPrnt, "N:%d, B:%d", aNote,aBeat);
   Serial.println(sPrnt);
#endif    
}


void Process_String(String &str)
{  
}

void setup() {
  BuzzerPlayer.Initial(cBuzzerPin, gTempo, &OnPlayNote);
  
  PCSerialManager.begin(115200, &Process_String); 
  BlueToothSerialManager.begin(BaudRate_CH05, &Process_String);  // BaudRate_CH05 = 38400 
}

void loop() {
  if ( Check_HasInput(gLastListerningTime) )
  {
#ifdef EnableBuzzer  
     BuzzerPlayer.FAbortPlay = true;
#endif
      //有收到字串，接著繼續接收，不去作 Process_PlayMode() loading 大的工作
  }
  else
  {
    unsigned long dMsec = millis()-gLastListerningTime;    
    
    if (dMsec > gIdleMsec)
    {
#ifdef EnableRgbLED
      //RgbLedControler.Process_PlayMode();
#endif
#ifdef EnableBuzzer
      BuzzerPlayer.Play_Song(gNotes, gBeats, length);
      gLastListerningTime = millis();
#endif
    }     
  }


  /* if (iDir>0)
    BuzzerPlayer.Increase_Tempo();
  else
    BuzzerPlayer.Decrease_Tempo();

  if (BuzzerPlayer.FTempo == gPriTempo) iDir = -iDir;
  gPriTempo = BuzzerPlayer.FTempo;
  */

}
