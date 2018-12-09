#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#include "DS1302.h"
#include "ST7920.h"
#include "delay.h"
#include "AM2320.h"

#define  MainInterface          0  //主界面
#define  Mainmenu               1  //主菜单
#define  DateSetInterface       2  //日期设置界面
#define  TimeSetInterface       3  //时间设置界面
#define  AlarmSetInterface      4  //闹钟设置界面
#define  Lamp_Mode              5  //灯工作模式选择
#define  Pump_Mode              6  //泵工作模式选择
#define  Date_Time_Setting      7  //日期时间设置选择界面
#define  Lamp_Pump_Fan_Setting  8  //灯泵风扇设置选择界面
#define  Fan_Mode               9  //风扇工作模式选择界面
#define  Low_Water              10
#define  No_Water               11


extern unsigned char code num1632[11][64];
extern unsigned int  tab[3];


void interface_display(int x);
void Conversion(bit c,unsigned char year,unsigned char month,unsigned char day);

#endif