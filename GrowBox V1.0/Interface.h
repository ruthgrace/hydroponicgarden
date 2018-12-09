#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#include "DS1302.h"
#include "ST7920.h"
#include "delay.h"
#include "AM2320.h"

#define  MainInterface          0  //������
#define  Mainmenu               1  //���˵�
#define  DateSetInterface       2  //�������ý���
#define  TimeSetInterface       3  //ʱ�����ý���
#define  AlarmSetInterface      4  //�������ý���
#define  Lamp_Mode              5  //�ƹ���ģʽѡ��
#define  Pump_Mode              6  //�ù���ģʽѡ��
#define  Date_Time_Setting      7  //����ʱ������ѡ�����
#define  Lamp_Pump_Fan_Setting  8  //�Ʊ÷�������ѡ�����
#define  Fan_Mode               9  //���ȹ���ģʽѡ�����
#define  Low_Water              10
#define  No_Water               11


extern unsigned char code num1632[11][64];
extern unsigned int  tab[3];


void interface_display(int x);
void Conversion(bit c,unsigned char year,unsigned char month,unsigned char day);

#endif