
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





#define EnableButton

#include <StringManage.h>
#include <CommonTypeDefine.h>
#include <PCSerialManage.h>
#include <BlueToothSerialManage.h>

#ifdef EnableButton
#include <ButtonManage.h>
#endif 

// constants --------------------------------------------------------------
const byte cCommandCount = 5;
#ifdef EnableButton
const int cButtonPin=4;  // Connect pin4 tp a button and a 10K ohm, then to Ground,  pin4--Button--10K Ohm--Ground
#endif 

// variables --------------------------------------------------------
char sPrnt[64];
String sCommand[cCommandCount];  //Array to store splited Strings
unsigned long gLastListerningTime=millis();
unsigned long gIdleMsec=3000;
int buttonState = 0;
const long cCheckInterval_UltraSonic=1000;



void Println_Serial(String sPrnt)
{
  PCSerialManager.Send_String(sPrnt);
}

void Println_BlueTooth(String sPrnt)
{
  BlueToothSerialManager.Send_String(sPrnt);
}


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

void initial_DigitalPins()
{
  for (int i=2; i<14; i++)
  //if (! isHardware_RxTx_Pin(i))
  {
    pinMode(i, OUTPUT);
    analogWrite(i, 0);  
    delay(100);
    analogWrite(i,255); 
  }  

  // intial button Pin to HIGH(button not pushed)-------------------------------------------   
#ifdef EnableButton
   pinMode(cButtonPin, INPUT_PULLUP);   //pinMode(cButtonPin, INPUT);
   digitalWrite(cButtonPin, HIGH );
#endif 
}


void Process_Command(String sCmd[])
{   
  /*
  if (sCmd[0].indexOf("A")==0)  // first Char is "A"
  { 
    // Do something here
  	// Add your own code here.  在這裡加上你的 Code ........... 
  }
  else if (sCmd[0].indexOf("C")==0)  // first char is "C"
  {    
    byte aR=sCmd[1].toInt(), aG=sCmd[2].toInt(), aB=sCmd[3].toInt();
    unsigned long delyMsc = sCmd[4].toInt();
    // Do something here
    //sprintf(sPrnt, "%d,%d,%d", aR,aG,aB,delyMsc);
    //Println_Serial(sPrnt);
  	// Add your own code here.  在這裡加上你的 Code ........... 
    
  }
  */
}



void Process_String(String &str)
{
  byte cmdCnt;

  str.toUpperCase();
  
  if (StringManager.Split(str, ",", sCommand, cmdCnt))
  {
    Process_Command(sCommand);
  }
}

#ifdef EnableButton
void OnButtonHoldCountIncreasing(TButtonManager*, int buttonPin, TButtonState bs, int incCnt)
{
//#ifdef debug
   sprintf(sPrnt, "Pin%d, Hold Cnt:%d", buttonPin, incCnt);   
   Serial.println(sPrnt); 
//#endif    
}

void Process_ButtonEvent(TButtonManager*, int buttonPin, TButtonState bs, int clickCnt)
{
  if (buttonPin==cButtonPin)
  {  
//#ifdef debug
    sprintf(sPrnt, "Pin%d: Sts:%d, incCnt:%d", buttonPin, bs, clickCnt);   
    Println_Serial(sPrnt); 
//#endif    

    switch (bs)
    {
      case bsClick: //按鈕點按
  		  // Add your own code here.  在這裡加上你的 Code ........... 
        if (1==clickCnt)
        {
        }
        else if (2==clickCnt)
        {         
        }
        break;
        
      case bsDoubleClick://按鈕雙擊     
  		  // Add your own code here.  在這裡加上你的 Code ...........  
        break;
        
      case bsHold:
  		  // Add your own code here.  在這裡加上你的 Code ........... 
        if (1==clickCnt)
        {
        }
        else if (2==clickCnt)
        {         
        }
        break;
        
      case bsLongHold:  //按鈕長按不放
  		  // Add your own code here.  在這裡加上你的 Code ........... 
        break;
    }
  }
  else
  {
  }
}
#endif 

void setup() {  
  initial_DigitalPins; // Can't move into any class as member function(), will affect BlueTooth

#ifdef EnableButton
  // Button 初始化 --------------------------------------------
  pinMode(cButtonPin, INPUT); //INPUT_PULLUP);   //pinMode(cButtonPin, INPUT);
  digitalWrite(cButtonPin, HIGH );
  ButtonManager.Initial( &Process_ButtonEvent);
#endif 
  // Serail 初始化 --------------------------------------------  
  PCSerialManager.begin(115200, &Process_String); 
  BlueToothSerialManager.begin(BaudRate_CH05, &Process_String);  // BaudRate_CH05 = 38400 
}

void loop() {
 // 檢查是否有輸入事件 Serial, Bluetooth, Button, UltraSonic ....
 if ( Check_HasInput(gLastListerningTime) )
  {
#ifdef EnableBuzzer  
     BuzzerPlayer.FAbortPlay = true;
#endif
      //有收到字串，接著繼續接收，不去作 Process_PlayMode() loading 大的工作
  }
  else   
  {
#ifdef EnableButton // This will affect BlueTooth, Remove the Bluetooth will fix it
    if ( ButtonManager.Check_ButtonEvent(cButtonPin) )
    {
  	  //Once get string, update gLastListerningTime to delay Do_SomeLoadingWork()
      gLastListerningTime = millis();   
    }
#endif 

    unsigned long dMsec = millis()-gLastListerningTime;
    if (dMsec > gIdleMsec)
    {
    	// Do some heavy loading coding here ------------------------------------
      // Do_SomeLoadingWork()
      
  
  		// Add your own code here.  在這裡加上你的 Code
  		// ......................
  
    } 
    
  }

}


