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
  
// 2016'3'7  Daniel Lu CopyRight  ( dan59314@gmail.com) ---------
//
// 	Web : 		http://www.rasvector.url.tw/
// 	YouTube :	http://www.youtube.com/dan59314/playlist
//
//  跨平台、跨語言 核心函式庫
//
//  功能：
//    用來管理 DanLib 內所有資料結構的運作
//
//  注意
//    1. 請將所有 XXXLIb 使用到的 自訂資料結構宣告全部放到 XXXTypDefine.pas
//
//    2. 請將所有 Math2DLib 內的Data type 的管理放到 XXXManager 內，包括：
//		  <1> 資料的判斷和轉換：EX: IsEqual(), IsEmpty(), RadianToDegree()…..。
//		  <2> 資料的資料運作: EX: Add(), Release(), Delete(), Insert()…..。
//		  <3> 資料的取得：EX: QSin(), QCos()…..。
//
//    3. 所有資料運作皆以 真實座標系統 ( 右手系統 ) 運作，即
//       逆時鐘旋轉時，角度為正， 順時鐘旋轉時角度為負。
//
//		4. 先置換 "ButtonManager" -> 類別名(EX:M2dManager)", 接著置換 "ButtonManage" -> 檔案名稱(EX: M2dManage)
//		
//		5. Servo 和 Arduino board 必須分別獨立供電，否則Servo.write() 後會導致arduino 不斷 Reset();
//-------------------------------------------------------------------------
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


// BuzzerNoteDefine.h

#ifndef _BUZZERNOTEDEFINE_h

#define _BUZZERNOTEDEFINE_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


#define DefineTToneNoteAsByte  // 定義成 enum Size 較大，定義成 byte會較小
//#define ReduceNoteSize

/*************************************************
 * Public Constants
 *************************************************/
#ifdef DefineTToneNoteAsByte
typedef byte TToneNote;   // typedef const byte TToneNote;  加上 const 會導致宣告的變數成為 readonly
#else
enum TToneNote {
      cRest=0, Si0,
      Do1, Do1s, Re1, Re1s, Mi1, Fa1, Fa1s, So1, So1s, La1, La1s, Si1,
      Do2, Do2s, Re2, Re2s, Mi2, Fa2, Fa2s, So2, So2s, La2, La2s, Si2,
      Do3, Do3s, Re3, Re3s, Mi3, Fa3, Fa3s, So3, So3s, La3, La3s, Si3,
      Do4, Do4s, Re4, Re4s, Mi4, Fa4, Fa4s, So4, So4s, La4, La4s, Si4
#ifndef ReduceNoteSize,
      Do5, Do5s, Re5, Re5s, Mi5, Fa5, Fa5s, So5, So5s, La5, La5s, Si5,
      Do6, Do6s, Re6, Re6s, Mi6, Fa6, Fa6s, So6, So6s, La6, La6s, Si6,
      Do7, Do7s, Re7, Re7s, Mi7, Fa7, Fa7s, So7, So7s, La7, La7s, Si7,
      Do8, DoS8, Re8, tReS8
#endif
				}; 
#endif

const unsigned short
  cFrequency[]= {
  		// Rest, Si0
  		0, 31, 
      // Do, #Do, Re, #Re, Mi, Fa, #Fa, So, #So, La, #La, Si,
  		33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62,
  		65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123,
  		131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
  		262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494
#ifndef ReduceNoteSize
  		,
  		523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 
			1047, 1109, 1175, 1245, 1319,1397, 1480, 1568, 1661, 1760,1865,1976, 
  		2093, 2217, 2349, 2489, 2637, 2794, 2960,3136, 3322, 3520, 3729, 3951,
  		4186, 4435, 4699, 4978
#endif
				};
  	
typedef byte TToneBeat;  	 // typedef const byte TToneBeat;  加上 const 會導致宣告的變數成為 readonly

const byte
  cMsecPerBeat = 125;
    
const byte 
  cTempoDiv = 50,
  cMinTempo=5, cMaxTempo=100,
  cNormalTempo = 25, // 一拍　= cB1*10 = 400 毫秒
  cQuickTempo = 10,
  cSlowTempo = 50;
  


// 1/8拍=200msec/8, 1/4拍, 1/2拍  
#define cB_8	1     // 1/8 拍
#define cB_4	2     // 1/4 拍,   12 5*2=250 msec
#define cB_2 	4     // 1/2 拍,  125*4=50 0 msec;
#define cB1 	8     // 1 拍,  125*8=1000 ms e c
#define cB2 	16    // 2 拍,
#define cB3 	24    // 3 拍,
#define cB4 	32    // 4 拍,
  


#ifdef DefineTToneNoteAsByte
  #define cRest	0		//0 	= cFrequency[0]
  #define Si0  1		//31 	= cFrequency[1]
  //--------------------------------------
  #define Do1  2		//33   = cFrequency[2]
  #define Do1s 3		//35 	 = cFrequency[3]	//#Do, 升Do
  #define Re1  4		//37   = cFrequency[4]
  #define Re1s 5		//39
  #define Mi1  6		//41
  #define Fa1  7		//44
  #define Fa1s 8		//46
  #define So1  9		//49
  #define So1s 10	//52
  #define La1  11	//55
  #define La1s 12	//58
  #define Si1  13	//62
  //--------------------------------------
  #define Do2  14	//65
  #define Do2s 15	//69
  #define Re2  16	//73
  #define Re2s 17	//78
  #define Mi2  18	//82
  #define Fa2  19	//87
  #define Fa2s 20	//93
  #define So2  21	//98
  #define So2s 22	//104
  #define La2  23	//110
  #define La2s 24	//117
  #define Si2  25	//123
  //--------------------------------------
  #define Do3  26	//131
  #define Do3s 27	//139
  #define Re3  28	//147
  #define Re3s 29	//156
  #define Mi3  30	//165
  #define Fa3  31	//175
  #define Fa3s 32	//185
  #define So3  33	//196
  #define So3s 34	//208
  #define La3  35	//220
  #define La3s 36	//233
  #define Si3  37	//247
  //--------------------------------------
  #define Do4  38	//262
  #define Do4s 39	//277
  #define Re4  40	//294
  #define Re4s 41	//311
  #define Mi4  42	//330
  #define Fa4  43	//349
  #define Fa4s 44	//370
  #define So4  45	//392
  #define So4s 46	//415
  #define La4  47	//440
  #define La4s 48	//466
  #define Si4  49	//494
#ifndef ReduceNoteSize
  //--------------------------------------
  #define Do5  50	//523
  #define Do5s 51	//554
  #define Re5  52	//587
  #define Re5s 53	//622
  #define Mi5  54	//659
  #define Fa5  55	//698
  #define Fa5s 56	//740
  #define So5  57	//784
  #define So5s 58	//831
  #define La5  59	//880
  #define La5s 60	//932
  #define Si5  61	//988
  //--------------------------------------
  #define Do6  62	//1047
  #define Do6s 63	//1109
  #define Re6  64	//1175
  #define Re6s 65	//1245
  #define Mi6  66	//1319
  #define Fa6  67	//1397
  #define Fa6s 68	//1480
  #define So6  69	//1568
  #define So6s 70	//1661
  #define La6  71	//1760
  #define La6s 72	//1865
  #define Si6  73	//1976
  //--------------------------------------
  #define Do7  74	//2093
  #define Do7s 75	//2217
  #define Re7  76	//2349
  #define Re7s 77	//2489
  #define Mi7  78	//2637
  #define Fa7  79	//2794
  #define Fa7s 80	//2960
  #define So7  81	//3136
  #define So7s 82	//3322
  #define La7  83	//3520
  #define La7s 84	//3729
  #define Si7  85	//3951
  //--------------------------------------
  #define Do_8  86	//4186
  #define Do_S8 87	//4435
  #define Re_8  88	//4699
  #define Re_S8 89	//4978
#endif
#else
#endif


#endif
