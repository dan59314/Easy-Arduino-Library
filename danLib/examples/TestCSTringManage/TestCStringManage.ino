/* -----------------------------------------------------------------------------
  Copyright: (C) Daniel Lu, RasVector Technology.

  Email : dan59314@gmail.com
 	Web : 		http://www.rasvector.url.tw/
 	YouTube :	http://www.youtube.com/dan59314/playlist

  This software may be freely copied, modified, and redistributed
  provided that this copyright notice is preserved on all copies.
  The intellectual property rights of the algorithms used reside
  with the Daniel Lu, RasVector Technology.

  You may not distribute this software, in]
  
 whole or in part, as
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




#include <StringManage.h>


void setup()
{

  Serial.begin(115200);  // Uno Use Baud Rate 115200
  while (!Serial) {};  	 // wait for serial port to connect. Needed for Leonardo only

  String lnS="The way to a man's heart is through his stomach.";
  String strs[20]; 
  byte cmdNum;

  Serial.println(sizeof(strs) / sizeof(String));  

  if ( StringManager.Split(lnS, " ", strs, 20, cmdNum) )
  { 
    Serial.print("CmdNum: "); Serial.println(cmdNum);
    for (byte i = 0; i < cmdNum; i++)
      Serial.println(strs[i]);
  }

   
   int nxtID=0, incID=0;
   String sField=lnS;
   while (StringManager.Get_NextString(lnS," ",nxtID, sField, true) )
   {
      //Serial.print(nxtID);
      //Serial.print(", ");
      Serial.println(sField);
   }
}

void loop()
{

  /* add main program code here */

}
