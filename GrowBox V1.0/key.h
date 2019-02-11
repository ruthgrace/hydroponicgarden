#ifndef __KEY_H__
#define __KEY_H__

#include "reg52.h"
#include "Interface.h"
#include "i2c.h"
#include "DS1302.h"



sbit HC165_nPL = P3^2;
sbit HC165_OUT = P3^3;
sbit HC165_CK  = P3^4;


sbit Power = P4^2;
sbit PumP2 = P3^6;
sbit Power_C  = P2^5;

sbit beep       = P3^0;    //定义蜂鸣器端口

extern unsigned char flagc;
extern unsigned char temp8;
extern unsigned char code pic0[];
extern unsigned char Alarm[5];
//extern unsigned char Alarm_Moto[5];
extern unsigned char Alarm_Pour1[16];
//extern unsigned char Alarm_Pour2[5];

extern unsigned int   flag1;
extern unsigned int   flag2;
extern unsigned char  flag3;
extern unsigned char  flag4;
extern unsigned int   flag5;
extern unsigned int   flag6;
extern unsigned char  flag7;
extern unsigned int   flag8;
extern unsigned char last_key_pressed;
extern unsigned char last_key_detected;
extern unsigned int key_press_num;

extern unsigned int  LCD_flag;
extern unsigned char Lamp_flag2;
extern unsigned int  Power_flag;
extern unsigned int  Lamp_flag1;
//extern unsigned char  Fan_flag;
extern unsigned char Pump1_flag;
extern unsigned char Pump2_flag;
extern unsigned char Pump3_flag;
extern unsigned char Lamp_flag;

extern unsigned char timeflag;
extern unsigned char CTimeflag;
extern unsigned char Cflag3;
extern unsigned char Cflag4;
extern unsigned char TTT;
 
//extern unsigned char Alarmflag;
/*------------------------------------------------
                    定义按键
------------------------------------------------*/



extern int Interface;

unsigned char KeyScan(void);
void Keyprocessing(int x);
void LED_Mode(unsigned char flag10);

//void Pump_Mode(unsigned char flag11,unsigned char flag12);
void Pum_Mode(void);


#endif