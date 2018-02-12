
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




#define debug

#include <UltraSonicManage.h>


// Constants ---------------------------------------------
const int cTrigPin = 12;
const int cEchoPin = 13;

// Variables -------------------------------------
#ifdef debug
char sPrnt[64];
#endif
float gDistance, gDistance_Denoise;

/*
void Initial_Pins(int trigPin, int echoPin)
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}*/

void setup() 
{
  //Initial_Pins(cTrigPin, cEchoPin);
  //超音波初始化 --------------------------------------------
  UltraSonicGesturer.Initial(cTrigPin, cEchoPin);
  
#ifdef debug
  Serial.begin(115200); // Starts the serial communication
#endif
}

void loop() 
{
  // 取得超音波模組距離 ---------------------------------
  gDistance = UltraSonicGesturer.Distance_CM();
  //gDistance_Denoise = UltraSonicGesturer.Distance_CM_DeNoise();
  
  
  // Add your own code here.  在這裡加上你的 Code
  // ......................
  
  
  
 #ifdef debug
   //sprintf(sPrnt, "Dist:%d", gDistance, gDistance_Denoise);   
   //Serial.println(sPrnt); 

   Serial.print("Dist: ");
   Serial.print(gDistance);
   //Serial.print(", ");
   //Serial.print(gDistance_Denoise);
   Serial.println("");
 #endif
}
