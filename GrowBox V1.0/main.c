#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
#include "ST7920.h"
#include "delay.h"
#include "DS1302.h"
#include "AM2320.h"
#include "18b20.h"
#include "Interface.h"
#include "key.h"
#include "i2c.h"




//sbit beep       = P3^0;    //����������˿�
sbit Lamp_Ctrl  = P2^0;    //�ƿ��ƶ˿�
sbit Fan_Ctrl   = P2^1;    //�ƿ��ƶ˿�
sbit Pump1_Ctrl = P2^2;    //��ˮ�ÿ��ƶ˿�
sbit Pump2_Ctrl = P2^3;
sbit Pump3_Ctrl = P2^4;    //ѭ���ÿ��ƶ˿�

sbit LCD_C      = P3^5;   //��Ļ�������
sbit Water_Low  = P4^1;   //��ˮλ���
sbit Water_Mid  = P4^0;   //��ˮλ���
sbit Water_Hig  = P4^3;   //��ˮλ���
sbit PWM        = P3^6;   //���Ʒ���ת��

sbit Low_Waterr = P2^6;   //��ˮλ�Ƶ���
sbit High_Water = P4^6;   //����ˮλ�̵���
sbit No_Waterr  = P4^4;   //��ˮλ�����



unsigned int   Lamp_flag1=0;   //�ƿ��Ʊ�־λ
extern unsigned char  Fan_flag; //���ȿ��Ʊ�־λ
unsigned char Pump1_flag=0;     //��ˮ�ÿ��Ʊ�־
unsigned char Pump2_flag=0;     //��ˮ�ñÿ��Ʊ�־��Ӳ��������ȥ����
unsigned char Pump3_flag;       //ѭ���ÿ��Ʊ�־
unsigned int  LCD_flag;         //������Ʊ�־λ
unsigned int  Eprom_flag=0;     //24C02���Ʊ�־
unsigned char flag3c;
unsigned char flag4c;
unsigned char flag7c=0;
unsigned char AMflag=0;
unsigned char innit;

unsigned char  TTT=0;


extern unsigned char flagc;


int Interface;	  //�������
unsigned char KeyValue;	//���ذ���ֵ
unsigned int  Power_flag=0;
extern unsigned char Lamp_flag;   //�ƹ���ģʽ��־
extern unsigned char code pic0[];
extern unsigned char code pic1[];
extern unsigned char flag3;
extern unsigned char flag4;
extern unsigned char flag7;   //���ȹ���ģʽѡ���־
extern unsigned char temp8;
extern unsigned char CTimeflag;
extern unsigned char Lamp_flag2;
extern unsigned char Cflag3;
extern unsigned char Cflag4;

unsigned char   Alarm[5];	      //���ӱ���������������
unsigned char   Alarm_Pour1[16];
unsigned char 	Eprom[4];

unsigned int   flush,num;    //PWMռ�ձȵ���
unsigned char  Alarmflag=0; //���ӱ�����־	Ĭ�ϲ�����



unsigned int temper;
extern float temperature;
unsigned char timeflag=0;

extern unsigned char last_key_detected;
extern unsigned char last_key_pressed;


void Sys_Start_Modechage();
void Pump_Modechange();
int is_light_change_time();
int is_pump_change_time();


/*************�̵������ƺ���**************/
void  Raly_Contrl()
{
	if (is_light_change_time()) {
		Sys_Start_Modechage();
	}
	if (is_pump_change_time()) {
		Pump_Modechange();
	}
}


void Init_Timer0(void)
{
	 TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
	 EA=1;            //���жϴ�
	 ET0=1;           //��ʱ���жϴ�
	 TR0=1;           //��ʱ�����ش�
}

void Init_Timer1(void)
{
	 TMOD |= 0x10;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
	 EA=1;            //���жϴ�
	 ET1=1;           //��ʱ���жϴ�

}

void SaData()
{
		ISendStr(0xae,80,&Alarm[0],1);         //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,81,&Alarm[1],1); 
		ISendStr(0xae,82,&Alarm[2],1);          //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,83,&Alarm[3],1); 
		 
	  ISendStr(0xae,0,  &innit,1);          //24c02��������
		ISendStr(0xae,8,  &Alarm_Pour1[0],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,16, &Alarm_Pour1[1],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,24, &Alarm_Pour1[2],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,32, &Alarm_Pour1[3],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,40, &Alarm_Pour1[4],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,48, &Alarm_Pour1[5],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,56, &Alarm_Pour1[6],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,128,&Alarm_Pour1[7],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,136,&Alarm_Pour1[8],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,144,&Alarm_Pour1[9],1);  //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,152,&Alarm_Pour1[10],1); //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,160,&Alarm_Pour1[11],1); //�ϵ籣�����ݵ�24C02
  	ISendStr(0xae,168,&Fan_flag,1);        //24c02��������
//		IRcvStr(0xae,176,&Alarm_Pour1[13],1); //24c02��������
//		IRcvStr(0xae,200,&Alarm_Pour1[14],1); //24c02��������
//		IRcvStr(0xae,208,&Alarm_Pour1[15],1); //24c02��������
    ISendStr(0xae,210, &flag3,1);            //�ϵ籣�����ݵ�24C02
		ISendStr(0xae,211, &flag4,1);
		ISendStr(0xae,248, &flag7,1);           //�ϵ籣�����ݵ�24C02


}


void Read_24C02()
{
		IRcvStr(0xae,80,&Alarm[0],1);         //��24c02�������� 
		IRcvStr(0xae,81,&Alarm[1],1); 
		IRcvStr(0xae,82,&Alarm[2],1);          //��24c02�������� 
		IRcvStr(0xae,83,&Alarm[3],1); 
		
	  IRcvStr(0xae,0,  &innit,1);           //�ж�24c02�Ƿ����³�ʼ��
		IRcvStr(0xae,8,  &Alarm_Pour1[0],1);  //24c02��������
		IRcvStr(0xae,16, &Alarm_Pour1[1],1);  //24c02��������
		IRcvStr(0xae,24, &Alarm_Pour1[2],1);  //24c02��������
		IRcvStr(0xae,32, &Alarm_Pour1[3],1);  //24c02��������
		IRcvStr(0xae,40, &Alarm_Pour1[4],1);  //24c02��������
		IRcvStr(0xae,48, &Alarm_Pour1[5],1);  //24c02��������
		IRcvStr(0xae,56, &Alarm_Pour1[6],1);  //24c02��������
		IRcvStr(0xae,128,&Alarm_Pour1[7],1);  //24c02��������
		IRcvStr(0xae,136,&Alarm_Pour1[8],1);  //24c02��������
		IRcvStr(0xae,144,&Alarm_Pour1[9],1);  //24c02��������
		IRcvStr(0xae,152,&Alarm_Pour1[10],1); //24c02��������
		IRcvStr(0xae,160,&Alarm_Pour1[11],1); //24c02��������
		IRcvStr(0xae,168,&Fan_flag,1); //24c02��������
//		IRcvStr(0xae,176,&Alarm_Pour1[13],1); //24c02��������
//		IRcvStr(0xae,200,&Alarm_Pour1[14],1); //24c02��������
//		IRcvStr(0xae,208,&Alarm_Pour1[15],1); //24c02��������
    IRcvStr(0xae,210, &flag3,1);            //24c02��������
		IRcvStr(0xae,211, &flag4,1);
		IRcvStr(0xae,248, &flag7,1); //24c02��������
		
}

void main()
{  
	  unsigned int i;
	  unsigned int  Sflag=1;
	
    Water_Low=1;
	  Water_Mid=1;
	  Water_Hig=1;
	  LCD_C=0;
	  Power_C=0;
	  Lamp_Ctrl=1;
	  Fan_Ctrl=1;
	  Pump1_Ctrl=1;
	  
	 if(!Power)
	 {
		   DelayMs(10);
		   if(!Power)
			 {
				 Power_C=1;
				 Power_flag=~Power_flag;
		   }
   }
	 
	 if(Power_flag)
	 {		 
		  LCD_flag=1;
		  
	 	
		 Init_Timer0();   //��ʱ��T0��ʼ��
		 Init_Timer1();
		 Ds1302_Init();   //DS1302��ʼ��
  	 Init_DS18B20();
		 Init_ST7920();   //LCD12864��ʼ��  
	   Read_24C02();
		 DelayMs(5);
//�µ�24C02����Ҫ���³�ʼ��
		 if(innit!=0x55)
		 {
				innit=0x55;
			  flag3=2;
			  flag4=1;
			  flag7=3;
			  Alarm[0]=9;
			  Alarm[1]=0;
			  
			 ISendStr(0xae,0,  &innit,1);
			 ISendStr(0xae,80, &Alarm[0],1);         //�ϵ籣�����ݵ�24C02
		   ISendStr(0xae,81, &Alarm[1],1); 
				
			 ISendStr(0xae,210, &flag3,1);            //��ʼ��LED��ģʽ
		   ISendStr(0xae,211, &flag4,1);            //��ʼ���õĹ���ģʽ
		   ISendStr(0xae,248, &flag7,1);            //��ʼ�����ȵĹ���ģʽ
				
     }
			 
		 Eprom[0]=Alarm[0];
		 Eprom[1]=Alarm[1];
		 Eprom[2]=Alarm[2];
		 Eprom[3]=Alarm[3];
		    		 
		 LCD_PutGraphic(pic0);
		 for(i=0;i<10;i++)
		 DelayMs(100);
   
   CGRAM(0);
   Interface=MainInterface;//Ĭ�Ͻ���=������

   while(Power_flag)
   { 	
		 	Power_C=1; 
	   	Pump3_flag=1;	
		      	 
      KeyValue=KeyScan();
			if(KeyValue!=0)    //����м�����
			{
				  Bell();
					Keyprocessing(KeyValue); //��ֵ����
					LCD_PutGraphic(pic0);
					ClrScreen();
				  LCD_flag=1;				
          num=0;
          DelayMs(50);				
				  
			} else if (last_key_pressed != 0) {
				  Bell();
					Keyprocessing(last_key_pressed); //��ֵ����
					LCD_PutGraphic(pic0);
					ClrScreen();
				  LCD_flag=1;				
          num=0;
				  last_key_pressed = 0;
			}
     
			LED_Mode(flag3)	;
		
		 		
		  if(!TTT) 
      {
			   Power_flag=0; 	
			}
			if(Power_flag==0)
      {				
  				SaData();
				  DelayMs(10);
				  Power_C=0;
          LCD_flag=0;				
			}
		
      //Schelduleʱ���ʱ���߷ָı�			
			if((Eprom[0]!=Alarm[0])||(Eprom[1]!=Alarm[1])||((Eprom[0]!=Alarm[0])&&(Eprom[1]!=Alarm[1]))||(CTimeflag==1))//||(Cflag3==1))	
			{	
				 IRcvStr(0xae,210, &flag3,1);   //24c02��������
		     IRcvStr(0xae,211, &flag4,1);
				 LED_Mode(flag3);	
				 Pum_Mode();

				  Sys_Start_Modechage();
					Pump_Modechange();		
				  CTimeflag=0;
		  
					Eprom[0]=Alarm[0];
					Eprom[1]=Alarm[1];
					Eprom[2]=Alarm[2];
					Eprom[3]=Alarm[3]; 
                 
      }
			
       if(Cflag3==1)      //ģʽ�仯��־
			 {
				  LED_Mode(flag3);
				  Sys_Start_Modechage();
				  Cflag3=0;
       } 
				
			 if(Cflag4==1)    //��ģʽ�仯��־
			 {
          Pum_Mode();
					Pump_Modechange();
          Cflag4=0	;				
       }
	    
			 if(LCD_flag)
			 {
				LCD_C=1;	
       }
			 else LCD_C=0;
			
			if(Fan_flag)         //������ͣ���Ʊ�־
			{
         Fan_Ctrl=0;
         TR1=1;					
      }
			else 
			{
				 Fan_Ctrl=1;
				 TR1=0;
			}
			
			if(!Water_Low)        //��ˮλ���
			{
				  No_Waterr=0;
				  High_Water=1;
				  Low_Waterr=1;
				  
				  Pump1_flag=0;
				  Pump2_flag=0;
				  Pump3_flag=0;	  
      }
			else
			{
        	No_Waterr=1;
			    Pump3_flag=1;	
    	}				

			if((!Water_Hig)||Pump2_flag)       //��ˮλ��⣬��ˮλʱPUMP2��ʼ�����ˮ
			{
				Pump2_Ctrl=0;	
				
				No_Waterr=1;                    //��ˮλʱ�����˸
				 DelayMs(5000);
				No_Waterr=0;
				
				High_Water=1;
		  	Low_Waterr =1;
				
      }
			else 
			{
				Pump2_Ctrl=1;	
			}    
			
			if((Water_Hig)&&(Water_Low)&&(!Water_Mid))//��ˮλʱ�Ƶ���
			{
				No_Waterr=1;
				High_Water=1;
		  	Low_Waterr =0;
     }
		 
		 if((Water_Hig)&&(Water_Low)&&(Water_Mid))  //����ˮλʱ�̵���
		 {
			  No_Waterr=1;
				High_Water=0;
		  	Low_Waterr =1;
     }
		 
			
      Ds1302_Read_Time();	//��ȡʱ��
			Raly_Contrl();
		  if(Sflag)
			{
				 LED_Mode(flag3);
				 Pum_Mode();
			   Pump_Modechange();
				 Sys_Start_Modechage();
         DelayMs(10);				
				 Sflag=0;
			}
		  	  
			if(Lamp_flag1)
			{
				 Lamp_Ctrl=0; 		
      }
			else Lamp_Ctrl=1;
			
		  if(Pump1_flag)
			{
				Pump1_Ctrl=0; 	
      }
			else Pump1_Ctrl=1;
			
			
			if(Pump3_flag)
			{
				Pump3_Ctrl=0;
      }
			else Pump3_Ctrl=1;
			
			DelayMs(1);
			tempdat();
			DelayMs(1);
			AM2320_Init();
      			
			interface_display(Interface);//��ʾ����
			
   }
 }
}
//��ʱ��0�жϷ�����
void Timer0_isr(void) interrupt 1 using 1	
{
	  unsigned char keyval;
		TR0 = 0;
		TH0 = (65536-10000)/256;   
		TL0 = (65536-10000)%256;
		num++;
	
	  load_key();
		if (temp8 != last_key_detected) {
			last_key_detected = temp8;
		  KeyPress();
		}
	  if(num==6000)
		{
			num=0;
			LCD_flag=0;
    }

   TR0 = 1;		
}
//��ʱ��1�жϷ�����
void Timer1_isr(void) interrupt 3 using 1	
{
	  unsigned int time;
    	
		TR1 = 0;
		TH1 = (65536-60)/256;   
		TL1 = (65536-60)%256;
	
	  time++;
	 
	  if(time>=40)  time=0;
	  
	  if(flag7==3)
		{
       if((time>=0)&&(time<=14)) PWM=0;
				 
			 else         PWM=1;
				 
    }
		else if(flag7==2)
		{
        if((time>=0)&&(time<=14)) PWM=0;
			  
			  else         PWM=1;

    }
		else if(flag7==1)
		{
		     if(time<=40)  PWM=0;
		
			  
			    else         PWM=1;
		}
    TR1 = 1;
}

int mod_24(int x)
{
	return x - (x / 24) * 24;
}

int get_led_end_hour(int start_time, int led_mode)
{
	int led_time;
	switch(led_mode)
	{
		case 1:
			led_time = 12;
		  break;
		case 2:
			led_time = 18;
		  break;
		default:
			led_time = 24;
	}
  return mod_24(start_time + led_time);
}

int led_state(int start_hour, int end_hour, int minute, int current_hour, int current_minute)
{
	if (current_hour == start_hour)
	{
    if (current_minute >= minute)
			return 1;
		else
			return 0;
	}
	if (current_hour == end_hour)
	{
    if (current_minute < minute)
			return 1;
		else
			return 0;
	}
	if (end_hour > start_hour)
	{
    if ((current_hour > start_hour) && (current_hour < end_hour))
			return 1;
		else
			return 0;
	}
	else
	{
		if ((current_hour > start_hour) || (current_hour < end_hour))
			return 1;
		else
			return 0;
	}
}

//ϵͳʱ��ı俪ʼʱ��LEDģʽ�ı����LED�ĵ�ǰ״̬
void Sys_Start_Modechage()
{
	IRcvStr(0xae,80,&Alarm[0],1);         //��24c02�������� 
	IRcvStr(0xae,81,&Alarm[1],1);
	
	if (flag3 == 3)
	{
		Lamp_flag1 = 1;
	}
  else
	{		
		int led_start_hour = Alarm[0];
		int led_end_hour = get_led_end_hour(led_start_hour, flag3);
		int led_minute = Alarm[1];
		int current_hour = time_buf1[4];
		int current_minute = time_buf1[5];
		Lamp_flag1 = led_state(led_start_hour, led_end_hour, led_minute, current_hour, current_minute);
	}
}

int time_plus_pump_time_hour(int start_hour, int start_minute, int pump_minutes) {
	int end_minute = start_minute + pump_minutes;
	if (end_minute >= 60)
		return mod_24(start_hour + end_minute/60);
	else
		return start_hour;
}

int time_plus_pump_time_minute(int start_minute, int pump_minutes) {
	int end_minute = start_minute + pump_minutes;
	return end_minute - (end_minute/60)*60;
}

int is_pumping_time(int pump_start_hour, int pump_end_hour, int pump_start_minute, int pump_end_minute, int current_hour, int current_minute)
{
	if ((current_hour != pump_start_hour) && (current_hour != pump_end_hour))
		return 0;
	if (pump_start_hour == pump_end_hour) {
		if ((current_minute >= pump_start_minute) && (current_minute < pump_end_minute))
			return 1;
		else
			return 0;
	}
	if (current_hour == pump_start_hour)
	{
		if (current_minute >= pump_start_minute)
			return 1;
		else
			return 0;
	}
	if (current_hour == pump_end_hour)
	{
		if (current_minute < pump_end_minute)
			return 1;
		else
			return 0;
	}
	return 0;
}

int pump1_state(int led_mode, int pump_mode, int led_start_hour, int led_start_minute, int current_hour, int current_minute)
{
	int led_hours;
	int pump_interval;
	int hour_increment = 1;
	int pump_session;
	switch(led_mode)
	{
		case 1:
			led_hours = 12;
		  break;
		case 2:
			led_hours = 18;
		  break;
		default:
			led_hours = 24;
	}
	
	switch(pump_mode)
	{
		case 1:
			pump_interval = 24;
		  break;
		case 2:
			pump_interval = led_hours / 2;
		  break;
		default:
			pump_interval = led_hours / 3;
		  break;
	}
	for(pump_session = 0; pump_session < pump_mode; pump_session++)
	{
		int pump_start_hour = mod_24(led_start_hour + hour_increment);
		int pump_start_minute = led_start_minute;
		int pump_end_hour = time_plus_pump_time_hour(pump_start_hour, pump_start_minute, 15);
		int pump_end_minute = time_plus_pump_time_minute(pump_start_minute, 15);
		// delete the next three lines if you want the pump to start an hour after the LEDs
		//  instead of the next round hour after the LEDs
		pump_end_hour = pump_start_hour;
    pump_start_minute = 0;
		pump_end_minute = 15;
		if (is_pumping_time(pump_start_hour, pump_end_hour, pump_start_minute, pump_end_minute, current_hour, current_minute))
			return 1;
		hour_increment = mod_24(hour_increment + pump_interval);
	}
	return 0;
}


void Pump_Modechange()
{    
		int led_start_hour = Alarm[0];
		int led_start_minute = Alarm[1];
		int current_hour = time_buf1[4];
		int current_minute = time_buf1[5];

		IRcvStr(0xae,210, &flag3,1);            //24c02��������
		IRcvStr(0xae,211, &flag4,1); 
		IRcvStr(0xae,80,&Alarm[0],1);         //��24c02�������� 
		IRcvStr(0xae,81,&Alarm[1],1);
	
		led_start_hour = Alarm[0];
		led_start_minute = Alarm[1];
		current_hour = time_buf1[4];
		current_minute = time_buf1[5];

		Pump1_flag = pump1_state(flag3, flag4, led_start_hour, led_start_minute, current_hour, current_minute);
}

int is_light_change_time() {
	// State changes only occur on the minute.
	if (time_buf1[6] != 0) {
		return 0;
	}
	// Check if it is time for the light to turn on
	if (((Alarm[0] == time_buf1[4]) && (Alarm[1] == time_buf1[5])) ||
      ((Alarm[2] == time_buf1[4]) && (Alarm[3] == time_buf1[5]))) {
		return 1;
	}
	return 0;
}

int is_pump_change_time() {
	// State changes only occur on the minute.
	if (time_buf1[6] != 0) {
		return 0;
	}
	// Check if the current time and minute match the first pump on/off time
	if (((Alarm_Pour1[0] == time_buf1[4]) && (Alarm_Pour1[1] == time_buf1[5])) ||
      ((Alarm_Pour1[2] == time_buf1[4]) && (Alarm_Pour1[3] == time_buf1[5])))	{
		return 1;
	}
	// If pump mode is 2x or more, check if the current time and minute match the second pump on/off time
	if (flag4 == 1) {
		return 0;
	}
	if (((Alarm_Pour1[4] == time_buf1[4]) && (Alarm_Pour1[5] == time_buf1[5])) ||
      ((Alarm_Pour1[6] == time_buf1[4]) && (Alarm_Pour1[7] == time_buf1[5])))	{
		return 1;
	}
	// If pump mode is 3x or more, check if the current time and minute match the second pump on/off time
	if (flag4 == 2) {
		return 0;
	}
	if (((Alarm_Pour1[8] == time_buf1[4]) && (Alarm_Pour1[9] == time_buf1[5])) ||
      ((Alarm_Pour1[10] == time_buf1[4]) && (Alarm_Pour1[11] == time_buf1[5])))	{
		return 1;
	}
	return 0;
}