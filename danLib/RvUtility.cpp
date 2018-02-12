
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
  ���v�ŧi  (C) Daniel Lu, RasVector Technology.

  Email : dan59314@gmail.com
 	Web : 		http://www.rasvector.url.tw/
 	YouTube :	http://www.youtube.com/dan59314/playlist

  �ϥΩέק�n��A�е����ޥΥX�B��T�p�W�C���g�L�@�̩��ܦP�N�A�T��ϥΦb�ӷ~�γ~�C
  
  ----------------------------------------------------------------------------------
  
  
  

  Arduino �禡�w

  �\��G
    �Ψөw�q DanLib ���w�q��Ƶ��c���B�@

  �`�N
    1. �бN�Ҧ� XXXLIb �ϥΨ쪺 �ۭq��Ƶ��c�ŧi������� CommonTypDefine.pas

    2. �бN�Ҧ� Math2DLib ����Data type ���޲z��� XXXManager ���A�]�A�G
		  <1> ��ƪ��P�_�M�ഫ�GEX: IsEqual(), IsEmpty(), RadianToDegree()�K..�C
		  <2> ��ƪ���ƹB�@: EX: Add(), Release(), Delete(), Insert()�K..�C
		  <3> ��ƪ����o�GEX: QSin(), QCos()�K..�C

    3. �Ҧ���ƹB�@�ҥH �u��y�Шt�� ( �k��t�� ) �B�@�A�Y
       �f��������ɡA���׬����A ����������ɨ��׬��t�C

		4. ���m�� "DanielClasser" -> ���O�W(EX:M2dManager)", ���۸m�� "DanielClassFile" -> �ɮצW��(EX: M2dManage)
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

	�ϥ� tone() �N�M pin 3, pin 11 �Ĭ�A pin10,11 �q�`���ӨϥΦb SoftSerial
	
	Servo �M Arduino board �������O�W�ߨѹq�A�_�hServo.write() ��|�ɭParduino ���_ Reset();
*/






#include "RvUtility.h"


// Constants --------------------------------------------






// Variables --------------------------------------------






// Functions --------------------------------------------------
int Get_CheckSum(String sStr)
{ 
	int aSum=0;
	
	for (int i=0; i<sStr.length(); i++)
	{
		//Serial.print("(byte)sStr[i]: "); Serial.println((byte)sStr[i]);
	  aSum += (int)sStr[i];
	}
	//Serial.print("aSum: "); Serial.println(aSum);
	return aSum;
}



byte Get_CheckSum_Byte(String sStr)
{ 
	return (byte)(Get_CheckSum(sStr) & 0xFF );
}


byte CRC8(const byte *data, byte len) 
{
  byte crc = 0x00;
  while (len--) {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}

void ByteArrayToString(byte byteAry[], String &str)
{
	str = String((char*)byteAry);
}


void StringToByteArray(String str, int arrayLength, byte* byteAry)
{
	//str.getBytes(byteAry, arrayLength);
	
	int num=min(arrayLength, str.length());
	
	int i=0;
	while (i<num)
	{
		byteAry[i] = (byte) str[i];
		i++;
	}
}