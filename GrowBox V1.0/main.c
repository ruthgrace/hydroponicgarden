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




sbit beep       = P3^0;    //定义蜂鸣器端口
sbit Lamp_Ctrl  = P2^0;    //灯控制端口
sbit Fan_Ctrl   = P2^1;    //灯控制端口
sbit Pump1_Ctrl = P2^2;    //淋水泵控制端口
sbit Pump2_Ctrl = P2^3;
sbit Pump3_Ctrl = P2^4;    //循环泵控制端口

sbit LCD_C      = P3^5;   //屏幕背光控制
sbit Water_Low  = P4^1;   //低水位检测
sbit Water_Mid  = P4^0;   //中水位检测
sbit Water_Hig  = P4^3;   //高水位检测
sbit PWM        = P3^6;   //控制风扇转动

sbit Low_Waterr = P2^6;   //中水位黄灯亮
sbit High_Water = P4^6;   //正常水位绿灯亮
sbit No_Waterr  = P4^4;   //低水位红灯亮



unsigned int   Lamp_flag1=0;   //灯控制标志位
extern unsigned char  Fan_flag; //风扇控制标志位
unsigned char Pump1_flag=0;     //淋水泵控制标志
unsigned char Pump2_flag=0;     //抽水泵泵控制标志（硬件部分已去掉）
unsigned char Pump3_flag;       //循环泵控制标志
unsigned int  LCD_flag;         //背光控制标志位
unsigned int  Eprom_flag=0;     //24C02控制标志
unsigned char flag3c;
unsigned char flag4c;
unsigned char flag7c=0;
unsigned char AMflag=0;
unsigned char innit;

unsigned char  TTT=0;


extern unsigned char flagc;


int Interface;	  //界面参数
unsigned char KeyValue;	//返回按键值
unsigned int  Power_flag=0;
extern unsigned char Lamp_flag;   //灯工作模式标志
extern unsigned char code pic0[];
extern unsigned char code pic1[];
extern unsigned char flag3;
extern unsigned char flag4;
extern unsigned char flag7;   //风扇工作模式选择标志
extern unsigned char temp8;
extern unsigned char CTimeflag;
extern unsigned char Lamp_flag2;
extern unsigned char Cflag3;
extern unsigned char Cflag4;

unsigned char   Alarm[5];	      //闹钟报警参数缓存数组
unsigned char   Alarm_Pour1[16];
unsigned char 	Eprom[4];

unsigned int   flush,num;    //PWM占空比调节
unsigned char  Alarmflag=0; //闹钟报警标志	默认不报警



unsigned int temper;
extern float temperature;
unsigned char timeflag=0;


void Sys_Start_Modechage();
void Pump_Modechange();

void Bell()
{
		beep=0;
	  DelayMs(100);
	  beep=1;
}


/*************继电器控制函数**************/
void  Raly_Contrl()
{
		if((time_buf1[6] == 0)||(temp8==0xfd)||(temp8==0xfb))    
		{
			if(((Alarm[0]==time_buf1[4])&&(Alarm[1]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfd))
			{
			    if(((Alarm[0]==time_buf1[4])&&(Alarm[1]==time_buf1[5])&&(time_buf1[4]!= 0))&&(time_buf1[6] == 0))
					{
						Lamp_flag1=1;
						
					}
				   if(Lamp_flag1==0)
					 Lamp_flag1=1; 

      }
		  if(((Alarm[2]==time_buf1[4])&&(Alarm[3]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfd))
			{
					if((Alarm[2]==time_buf1[4])&&(Alarm[3]==time_buf1[5])&&(time_buf1[4]!= 0)&&(time_buf1[6] == 0) )  
						 Lamp_flag1=0;	
				
					if(Lamp_flag1==1)
						 Lamp_flag1=0; 
			}
		 		
		 if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||
			  ((Alarm_Pour1[8]==time_buf1[4])&&(Alarm_Pour1[9]==time_buf1[5])&&(time_buf1[4]!= 0))||
		    ((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||
			  ((Alarm_Pour1[10]==time_buf1[4])&&(Alarm_Pour1[11]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
			{
          
        //Lamp点亮12hr
				
            if((flag3==1)&&(flag4==1)||(temp8==0xfb))
            {
							if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							{	
								if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0)) )    
								{
								  Pump1_flag=1;	
                }									
								if(timeflag)
								{
								  Pump1_flag=1;
								}
						  }
							
							if((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0)||(temp8==0xfb))
							   
							{ 
								if((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))
								{
								    Pump1_flag=0;
                }									
                if(!timeflag)
								{
								    Pump1_flag=0;
								}
						  }							
						 
            }	


            if((flag3==1)&&(flag4==2)||(temp8==0xfb))	
						{
							if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				         ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))|(temp8==0xfb) )
							 {
							   if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				            ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0)))
							    
								     Pump1_flag=1;	
							
								  if(timeflag)
									{
										Pump1_flag=1;
									}
							}
							
							
							if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				         ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))  
							 {  
								 if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				         ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))	)
								
  								 Pump1_flag=0;
							 
									if(!timeflag)
									{
											Pump1_flag=0;
									}
							 }
            }	
           
            if((flag3==1)&&(flag4==3)||(temp8==0xfb))
            {
							if((((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[8]==time_buf1[4])&&(Alarm_Pour1[9]==time_buf1[5])&&(time_buf1[4]!= 0)))||(temp8==0xfb))
							{  
							  if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[8]==time_buf1[4])&&(Alarm_Pour1[9]==time_buf1[5])&&(time_buf1[4]!= 0)))
							    
								    Pump1_flag=1;
								
								  if(timeflag)
									{
										Pump1_flag=1;
									}
								
							}
							
							if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[10]==time_buf1[4])&&(Alarm_Pour1[11]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							 {
								if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[10]==time_buf1[4])&&(Alarm_Pour1[11]==time_buf1[5])&&(time_buf1[4]!= 0)))
							  
								 Pump1_flag=0;
								 if(!timeflag)
									{
										Pump1_flag=0;
									}
								
							 }
							 
            }	


           //Lamp点亮18Hr
						 if((flag3==2)&&(flag4==1)||(temp8==0xfb))
            {
							if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							{
								if((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))
								Pump1_flag=1;
                
                  if(timeflag)
									{
										Pump1_flag=1;
									}								
							
						  }
							if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							{
								if((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))
							    Pump1_flag=0; 
								
								  if(!timeflag)
									{
										Pump1_flag=0;
									}								
								
              }
							
						 
            }	

            if((flag3==2)&&(flag4==2)||(temp8==0xfb))	
						{
							if((((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0)))||(temp8==0xfb))
							{
								  if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				            ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0)) )
							     
									   Pump1_flag=1;	
									
							    if(timeflag)
									{
										Pump1_flag=1;
									}					
							}
							if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				         ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							{
						     if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				            ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0)))
								
							     Pump1_flag=0;
							   
								 if(!timeflag)
								 {
										Pump1_flag=0;
								 }					
						 }
            }	
           
            if((flag3==2)&&(flag4==3)||(temp8==0xbf))
            {
							if((((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[8]==time_buf1[4])&&(Alarm_Pour1[9]==time_buf1[5])&&(time_buf1[4]!= 0)))||(temp8==0xfb))
							{
							  if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[8]==time_buf1[4])&&(Alarm_Pour1[9]==time_buf1[5])&&(time_buf1[4]!= 0)))
							  
								  Pump1_flag=1;	
							
							    if(timeflag)
									{
										Pump1_flag=1;
									}					
						 }
							if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[10]==time_buf1[4])&&(Alarm_Pour1[11]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							 
                {
									if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				            ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||
			              ((Alarm_Pour1[10]==time_buf1[4])&&(Alarm_Pour1[11]==time_buf1[5])&&(time_buf1[4]!= 0)))
    							  Pump1_flag=0;
							
							    if(!timeflag)
									{
										Pump1_flag=0;
									}		
								}									
            }	
						
						//Lamp点亮24hr
						 if((flag3==3)&&(flag4==1)||(temp8==0xfb))
            {
							if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							{
								if((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))
								Pump1_flag=1;	
							  if(timeflag)
								{
									Pump1_flag=1;
								}					
						   }
							if(((temp8==0xdf)||(Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
						 {
							  if((temp8==0xdf)||(Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))
							  
								Pump1_flag=0; 
               
								if(!timeflag)
								{
									Pump1_flag=0;
								}					
						 }							
						 
           }	

            if((flag3==3)&&(flag4==2)||(temp8==0xfb))	
						{
							if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				         ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb) )
							{
							   if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				         ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0)) )
							    Pump1_flag=1;	
							    if(timeflag)
								 {
									Pump1_flag=1;
								 }				
						  }
						
						if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				      ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
						  {
							   if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				         ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0)))
							   Pump1_flag=0;
							   if(!timeflag)
								 {
									Pump1_flag=0;
								 }				
						  }
           }	
				
           
            if((flag3==3)&&(flag4==3)||(temp8==0xfb))
            {
							if((((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[8]==time_buf1[4])&&(Alarm_Pour1[9]==time_buf1[5])&&(time_buf1[4]!= 0)))||(temp8==0xfb))
							{
							  if(((Alarm_Pour1[0]==time_buf1[4])&&(Alarm_Pour1[1]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[4]==time_buf1[4])&&(Alarm_Pour1[5]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[8]==time_buf1[4])&&(Alarm_Pour1[9]==time_buf1[5])&&(time_buf1[4]!= 0)))
							 
								Pump1_flag=1;
								
							  if(timeflag)
								{
									Pump1_flag=1;
								}				
							}
							if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[10]==time_buf1[4])&&(Alarm_Pour1[11]==time_buf1[5])&&(time_buf1[4]!= 0))||(temp8==0xfb))
							 {
								if(((Alarm_Pour1[2]==time_buf1[4])&&(Alarm_Pour1[3]==time_buf1[5])&&(time_buf1[4]!= 0))||
				        ((Alarm_Pour1[6]==time_buf1[4])&&(Alarm_Pour1[7]==time_buf1[5])&&(time_buf1[4]!= 0))||
			          ((Alarm_Pour1[10]==time_buf1[4])&&(Alarm_Pour1[11]==time_buf1[5])&&(time_buf1[4]!= 0)))
							 
  								Pump1_flag=0;
								 if(!timeflag)
								 {
									Pump1_flag=0;
								 }				
							 }
           }	
			}
		}
}


void Init_Timer0(void)
{
	 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
	 EA=1;            //总中断打开
	 ET0=1;           //定时器中断打开
	 TR0=1;           //定时器开关打开
}

void Init_Timer1(void)
{
	 TMOD |= 0x10;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
	 EA=1;            //总中断打开
	 ET1=1;           //定时器中断打开

}

void SaData()
{
		ISendStr(0xae,80,&Alarm[0],1);         //断电保存数据到24C02
		ISendStr(0xae,81,&Alarm[1],1); 
		ISendStr(0xae,82,&Alarm[2],1);          //断电保存数据到24C02
		ISendStr(0xae,83,&Alarm[3],1); 
		 
	  ISendStr(0xae,0,  &innit,1);          //24c02读出数据
		ISendStr(0xae,8,  &Alarm_Pour1[0],1);  //断电保存数据到24C02
		ISendStr(0xae,16, &Alarm_Pour1[1],1);  //断电保存数据到24C02
		ISendStr(0xae,24, &Alarm_Pour1[2],1);  //断电保存数据到24C02
		ISendStr(0xae,32, &Alarm_Pour1[3],1);  //断电保存数据到24C02
		ISendStr(0xae,40, &Alarm_Pour1[4],1);  //断电保存数据到24C02
		ISendStr(0xae,48, &Alarm_Pour1[5],1);  //断电保存数据到24C02
		ISendStr(0xae,56, &Alarm_Pour1[6],1);  //断电保存数据到24C02
		ISendStr(0xae,128,&Alarm_Pour1[7],1);  //断电保存数据到24C02
		ISendStr(0xae,136,&Alarm_Pour1[8],1);  //断电保存数据到24C02
		ISendStr(0xae,144,&Alarm_Pour1[9],1);  //断电保存数据到24C02
		ISendStr(0xae,152,&Alarm_Pour1[10],1); //断电保存数据到24C02
		ISendStr(0xae,160,&Alarm_Pour1[11],1); //断电保存数据到24C02
  	ISendStr(0xae,168,&Fan_flag,1);        //24c02读出数据
//		IRcvStr(0xae,176,&Alarm_Pour1[13],1); //24c02读出数据
//		IRcvStr(0xae,200,&Alarm_Pour1[14],1); //24c02读出数据
//		IRcvStr(0xae,208,&Alarm_Pour1[15],1); //24c02读出数据
    ISendStr(0xae,210, &flag3,1);            //断电保存数据到24C02
		ISendStr(0xae,211, &flag4,1);
		ISendStr(0xae,248, &flag7,1);           //断电保存数据到24C02


}


void Read_24C02()
{
		IRcvStr(0xae,80,&Alarm[0],1);         //从24c02读出数据 
		IRcvStr(0xae,81,&Alarm[1],1); 
		IRcvStr(0xae,82,&Alarm[2],1);          //从24c02读出数据 
		IRcvStr(0xae,83,&Alarm[3],1); 
		
	  IRcvStr(0xae,0,  &innit,1);           //判断24c02是否重新初始化
		IRcvStr(0xae,8,  &Alarm_Pour1[0],1);  //24c02读出数据
		IRcvStr(0xae,16, &Alarm_Pour1[1],1);  //24c02读出数据
		IRcvStr(0xae,24, &Alarm_Pour1[2],1);  //24c02读出数据
		IRcvStr(0xae,32, &Alarm_Pour1[3],1);  //24c02读出数据
		IRcvStr(0xae,40, &Alarm_Pour1[4],1);  //24c02读出数据
		IRcvStr(0xae,48, &Alarm_Pour1[5],1);  //24c02读出数据
		IRcvStr(0xae,56, &Alarm_Pour1[6],1);  //24c02读出数据
		IRcvStr(0xae,128,&Alarm_Pour1[7],1);  //24c02读出数据
		IRcvStr(0xae,136,&Alarm_Pour1[8],1);  //24c02读出数据
		IRcvStr(0xae,144,&Alarm_Pour1[9],1);  //24c02读出数据
		IRcvStr(0xae,152,&Alarm_Pour1[10],1); //24c02读出数据
		IRcvStr(0xae,160,&Alarm_Pour1[11],1); //24c02读出数据
		IRcvStr(0xae,168,&Fan_flag,1); //24c02读出数据
//		IRcvStr(0xae,176,&Alarm_Pour1[13],1); //24c02读出数据
//		IRcvStr(0xae,200,&Alarm_Pour1[14],1); //24c02读出数据
//		IRcvStr(0xae,208,&Alarm_Pour1[15],1); //24c02读出数据
    IRcvStr(0xae,210, &flag3,1);            //24c02读出数据
		IRcvStr(0xae,211, &flag4,1);
		IRcvStr(0xae,248, &flag7,1); //24c02读出数据
		
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
		  
		  Bell();
		 	
		 Init_Timer0();   //定时器T0初始化
		 Init_Timer1();
		 Ds1302_Init();   //DS1302初始化
  	 Init_DS18B20();
		 Init_ST7920();   //LCD12864初始化  
	   Read_24C02();
		 DelayMs(5);
//新的24C02，需要重新初始化
		 if(innit!=0x55)
		 {
				innit=0x55;
			  flag3=2;
			  flag4=1;
			  flag7=3;
			  Alarm[0]=9;
			  Alarm[1]=0;
			  
			 ISendStr(0xae,0,  &innit,1);
			 ISendStr(0xae,80, &Alarm[0],1);         //断电保存数据到24C02
		   ISendStr(0xae,81, &Alarm[1],1); 
				
			 ISendStr(0xae,210, &flag3,1);            //初始化LED的模式
		   ISendStr(0xae,211, &flag4,1);            //初始化泵的工作模式
		   ISendStr(0xae,248, &flag7,1);            //初始化风扇的工作模式
				
     }
			 
		 Eprom[0]=Alarm[0];
		 Eprom[1]=Alarm[1];
		 Eprom[2]=Alarm[2];
		 Eprom[3]=Alarm[3];
		    		 
		 LCD_PutGraphic(pic0);
		 for(i=0;i<10;i++)
		 DelayMs(100);
   
   CGRAM(0);
   Interface=MainInterface;//默认界面=主界面

   while(Power_flag)
   { 	
		 	Power_C=1; 
	   	Pump3_flag=1;	
		      	 
      KeyValue=KeyScan();
			if(KeyValue!=0)    //如果有键按下
			{	 				  
				  Bell();
					Keyprocessing(KeyValue); //键值处理
					LCD_PutGraphic(pic0);
					ClrScreen();
				  LCD_flag=1;				
          num=0;
          DelayMs(50);				
				  
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
		
      //Scheldule时间的时或者分改变			
			if((Eprom[0]!=Alarm[0])||(Eprom[1]!=Alarm[1])||((Eprom[0]!=Alarm[0])&&(Eprom[1]!=Alarm[1]))||(CTimeflag==1))//||(Cflag3==1))	
			{	
				 IRcvStr(0xae,210, &flag3,1);   //24c02读出数据
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
			
       if(Cflag3==1)      //模式变化标志
			 {
				  Sys_Start_Modechage();
				  Cflag3=0;
       } 
				
			 if(Cflag4==1)    //泵模式变化标志
			 {

					Pump_Modechange();
          Cflag4=0	;				
       }
	    
			 if(LCD_flag)
			 {
				LCD_C=1;	
       }
			 else LCD_C=0;
			
			if(Fan_flag)         //风扇起停控制标志
			{
         Fan_Ctrl=0;
         TR1=1;					
      }
			else 
			{
				 Fan_Ctrl=1;
				 TR1=0;
			}
			
			if(!Water_Low)        //低水位检测
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

			if((!Water_Hig)||Pump2_flag)       //高水位检测，高水位时PUMP2开始往外抽水
			{
				Pump2_Ctrl=0;	
				
				No_Waterr=1;                    //高水位时红灯闪烁
				 DelayMs(5000);
				No_Waterr=0;
				
				High_Water=1;
		  	Low_Waterr =1;
				
      }
			else 
			{
				Pump2_Ctrl=1;	
			}    
			
			if((Water_Hig)&&(Water_Low)&&(!Water_Mid))//中水位时黄灯亮
			{
				No_Waterr=1;
				High_Water=1;
		  	Low_Waterr =0;
     }
		 
		 if((Water_Hig)&&(Water_Low)&&(Water_Mid))  //正常水位时绿灯亮
		 {
			  No_Waterr=1;
				High_Water=0;
		  	Low_Waterr =1;
     }
		 
			
      Ds1302_Read_Time();	//读取时钟
			Raly_Contrl();
		  if(Sflag)
			{
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
      			
			interface_display(Interface);//显示界面
			
   }
 }
}
//定时器0中断服务函数
void Timer0_isr(void) interrupt 1 using 1	
{
		TR0 = 0;
		TH0 = (65536-50000)/256;   
		TL0 = (65536-50000)%256;
		num++;

	  if(num==1200)
		{
			num=0;
			LCD_flag=0;
    }

   TR0 = 1;		
}
//定时器1中断服务函数
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

//系统时间改变开始时间LED模式改变计算LED的当前状态
void Sys_Start_Modechage()
{
	 IRcvStr(0xae,80,&Alarm[0],1);         //从24c02读出数据 
	 IRcvStr(0xae,81,&Alarm[1],1); 
    
    	if(flag3==1)
			{
				 if(Alarm[0]==0)
				 {
						if((0<time_buf1[4])&&(time_buf1[4]<12)||
							(0==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==12)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((12<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==12)&&(time_buf1[5]>=Alarm[3])||
						  (time_buf1[4]==0 )&&(time_buf1[5]<Alarm[1]) )
							  Lamp_flag1=0;

         }
				 else if(Alarm[0]==1)
				 {
						if((1<time_buf1[4])&&(time_buf1[4]<13)||
							(1==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==13)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((13<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==0 )&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==13)&&(time_buf1[5]>=Alarm[3])||
              ((time_buf1[4]==1)&&(time_buf1[5]<Alarm[1])))
							  
						     Lamp_flag1=0;
         }
				 else if(Alarm[0]==2)
				 {
						if((2<time_buf1[4])&&(time_buf1[4]<14)||
							(2==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==14)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((14<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]>=0 )&&(time_buf1[4]<2)||
						  (time_buf1[4]==14)&&(time_buf1[5]>=Alarm[3])||
						  (time_buf1[4]==2 )&&(time_buf1[5]<Alarm[1]) )
							  
						   Lamp_flag1=0;

         }
				  else if(Alarm[0]==3)
				 {
						if((3<time_buf1[4])&&(time_buf1[4]<15)||
							(3==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==15)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((15<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==15)&&(time_buf1[5]>=Alarm[3])||
						  (time_buf1[4]==3 )&&(time_buf1[5]<Alarm[1])||
						  (0<time_buf1[4])&&(time_buf1[4]<3))
							 
    						Lamp_flag1=0;

         }
				   else if(Alarm[0]==4)
				 {
						if((4<time_buf1[4])&&(time_buf1[4]<16)||
							(4==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==15)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((16<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&(&time_buf1[5]<=59)||
						  (0<time_buf1[4])&&(time_buf1[4]<4)||
						  (time_buf1[4]==4 )&&(time_buf1[5]<Alarm[1])||
						  (time_buf1[4]==16)&&(time_buf1[5]>=Alarm[3]) )
							  Lamp_flag1=0;

         }
				   else if(Alarm[0]==5)
				 {
						if((5<time_buf1[4])&&(time_buf1[4]<17)||
							(5==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==17)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((17<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==17)&&(time_buf1[5]>=Alarm[3])||
						  (time_buf1[4]==5 )&&(time_buf1[5]<Alarm[1]) || 
              (0<time_buf1[4])&&(time_buf1[4]<5))
							  
						     Lamp_flag1=0;

         }
				 
				  else if(Alarm[0]==6)
				 {
						if((6<time_buf1[4])&&(time_buf1[4]<18)||
							(6==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==18)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((18<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==18)&&(time_buf1[5]>=Alarm[3])||
          		(time_buf1[4]==6)&&(time_buf1[5]<Alarm[1])||
     					(0<time_buf1[4])&&(time_buf1[4]<6))
							  Lamp_flag1=0;

         }
				  else if(Alarm[0]==7)
				 {
						if((7<time_buf1[4])&&(time_buf1[4]<19)||
							(7==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==19)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((19<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==19)&&(time_buf1[5]>=Alarm[3])||
   						(time_buf1[4]==7)&&(time_buf1[5]<Alarm[1])||
     					(0<time_buf1[4])&&(time_buf1[4]<7))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==8)
				 {
						if((8<time_buf1[4])&&(time_buf1[4]<20)||
							(8==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==20)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((20<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==20)&&(time_buf1[5]>=Alarm[3])||
    					(time_buf1[4]==7)&&(time_buf1[5]<Alarm[1])||
     					(0<time_buf1[4])&&(time_buf1[4]<7) )
							  
						    Lamp_flag1=0;

         }
				 else if(Alarm[0]==9)
				 {
						if((9<time_buf1[4])&&(time_buf1[4]<21)||
							(9==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==21)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((21<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==21)&&(time_buf1[5]>=Alarm[3])||
						  (time_buf1[4]==9)&&(time_buf1[5]<Alarm[1])||
     					(0<time_buf1[4])&&(time_buf1[4]<9))
							  
						   Lamp_flag1=0;

         }
				 else if(Alarm[0]==10)
				 {
						if((10<time_buf1[4])&&(time_buf1[4]<22)||
							(10==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==22)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((22<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==22)&&(time_buf1[5]>=Alarm[3])||
              (time_buf1[4]==10)&&(time_buf1[5]<Alarm[1])||
     					(0<time_buf1[4])&&(time_buf1[4]<10))
							  Lamp_flag1=0;

         }
				 
				 else if(Alarm[0]==11)
				 {
						if((11<time_buf1[4])&&(time_buf1[4]<23)||
							(11==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==23)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						
						if((time_buf1[4]==23)&&(time_buf1[5]>=Alarm[3])||
						  (time_buf1[4]>=0)&&(time_buf1[4]<11)||
           		(time_buf1[4]==11)&&(time_buf1[5]<Alarm[1]))
							  Lamp_flag1=0;

         }
				 
				 else if(Alarm[0]==12)
				 {
						if((12<time_buf1[4])&&(time_buf1[4]<23)||
							(12==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==0)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						
						if(((time_buf1[4]==0)&&(time_buf1[5]>=Alarm[3]))||
						   (0<time_buf1[4])&&(time_buf1[4]<12)||
						   (time_buf1[4]==12)&&(time_buf1[5]<Alarm[1])
						  )
							  Lamp_flag1=0;

         }
				 
				 else  if(Alarm[0]==13)
				 {
           if((13<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					   ((time_buf1[4]==0 )&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (1==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
					 {
							Lamp_flag1=1;	
           }
					 if((1<time_buf1[4])&&(time_buf1[4]<13)||
						  (1==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
					    (13==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
					 {
							Lamp_flag1=0;	
           }	 
         }
				 else if(Alarm[0]==14)
				 {
						if((14<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<2)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (2==time_buf1[4])&&(time_buf1[5]<Alarm[3]) )
        
						  Lamp_flag1=1;	
						
						if(((2<time_buf1[4])&&(time_buf1[4]<14))||
							(2==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (14==time_buf1[4])&&(time_buf1[5]<Alarm[1]) )
						{
							Lamp_flag1=0;	
            }

         }
				 else if(Alarm[0]==15)
				 {
						if((15<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<3)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (3==time_buf1[4])&&(time_buf1[5]<Alarm[3]) )
						
              Lamp_flag1=1;	
						
            if(((3<time_buf1[4])&&(time_buf1[4]<15))||
							(3==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (15==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }	 
				else if(Alarm[0]==16)
				{
						if((16<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<4)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (4==time_buf1[4])&&(time_buf1[5]<Alarm[3]) )
             
						Lamp_flag1=1;	
						
            if((4<time_buf1[4])&&(time_buf1[4]<16)||
							(4==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (16==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
				else if(Alarm[0]==17)
				{
						if((17<time_buf1[4])&&(time_buf1[4]<23)||
						  ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<5)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  ( 5==time_buf1[4])&&(time_buf1[5]<Alarm[3]) )
              Lamp_flag1=1;	
						
            if((5<time_buf1[4])&&(time_buf1[4]<17)||
							 (5==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						   (17==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
				else if(Alarm[0]==18)
				{
						if((18<time_buf1[4])&&(time_buf1[4]<23)||
						  ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<6)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (6==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
             
						Lamp_flag1=1;	
						
            if((6<time_buf1[4])&&(time_buf1[4]<18)||
							(6==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (18==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
				else if(Alarm[0]==19)
				{
						if((19<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<7)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (7==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
              
						 Lamp_flag1=1;	
						
            if((7<time_buf1[4])&&(time_buf1[4]<19)||
							(7==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (19==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
        else if(Alarm[0]==20)
				{
						if((20<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<8)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (8==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
             
 						 Lamp_flag1=1;	
						
            if((8<time_buf1[4])&&(time_buf1[4]<20)||
							(8==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (20==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
				else if(Alarm[0]==21)
				{
						if((21<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<9)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (9==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
              Lamp_flag1=1;	
            if((9<time_buf1[4])&&(time_buf1[4]<21)||
							(9==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (21==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
        else if(Alarm[0]==22)
				{
						if((22<time_buf1[4])&&(time_buf1[4]<23)||
						 ((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
					    (0<time_buf1[4])&&(time_buf1[4]<10)||
						  (0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59)||
						  (10==time_buf1[4])&&(time_buf1[5]<Alarm[3])  )
              Lamp_flag1=1;	
            if((10<time_buf1[4])&&(time_buf1[4]<22)||
							(10==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (22==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
				else if(Alarm[0]==23)
				{
						if(((time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59))||
							  ((0==time_buf1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<=59))||
							   (0<time_buf1[4])&&(time_buf1[4]<11)||
						    (11==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						  
						Lamp_flag1=1;	
						
					  if((11<time_buf1[4])&&(time_buf1[4]<23)||
							(10==time_buf1[4])&&(time_buf1[5]>=Alarm[3])||
						  (23==time_buf1[4])&&(time_buf1[5]<Alarm[1]))
						{
							Lamp_flag1=0;	
            }
        }
				
			}
			 if(flag3==2)
			{
				 if(Alarm[0]==0)
				 {
						if((0<time_buf1[4])&&(time_buf1[4]<18)||
							(0==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==18)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((18<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (0==time_buf1[4])&&(time_buf1[5]<Alarm[1])||
						  (time_buf1[4]==18)&&(time_buf1[5]>=Alarm[3]))
							  
						    Lamp_flag1=0;

         }
				 
				 else if(Alarm[0]==1)
				 {
						if((1<time_buf1[4])&&(time_buf1[4]<19)||
							(1==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==19)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((19<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (time_buf1[4]==19)&&(time_buf1[5]>=Alarm[3])||
						  (1==time_buf1[4])&&(time_buf1[5]<Alarm[1])||
						  ((time_buf1[4]==0)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)))
							  
						    Lamp_flag1=0;

         }
				 else if(Alarm[0]==2)
				 {
						if((2<time_buf1[4])&&(time_buf1[4]<20)||
							(2==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==20)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((20<time_buf1[4])&&(time_buf1[4]<23)||
							((0<=time_buf1[4])&&(time_buf1[4]<2))||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  (2==time_buf1[4])&&(time_buf1[5]<Alarm[1])||
						  (time_buf1[4]==20)&&(time_buf1[5]>=Alarm[3]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==3)
				 {
						if((3<time_buf1[4])&&(time_buf1[4]<21)||
							(3==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==21)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((21<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  ((0<=time_buf1[4])&&(time_buf1[4]<3))||
						  (3==time_buf1[4])&&(time_buf1[5]<Alarm[1])||
						  (time_buf1[4]==21)&&(time_buf1[5]>=Alarm[3]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==4)
				 {
						if((4<time_buf1[4])&&(time_buf1[4]<22)||
							(4==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==22)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((22<time_buf1[4])&&(time_buf1[4]<23)||
							(time_buf1[4]==23)&&(0<=time_buf1[5])&&(time_buf1[5]<=59)||
						  ((0<=time_buf1[4])&&(time_buf1[4]<4))||
						  (time_buf1[4]==22)&&(time_buf1[5]>=Alarm[3]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==5)
				 {
						if((5<time_buf1[4])&&(time_buf1[4]<23)||
							(5==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==23)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((0<=time_buf1[4])&&(time_buf1[4]<5)||
               (5==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||							
						   (time_buf1[4]==23)&&(time_buf1[5]>=Alarm[3]))
							  
						    Lamp_flag1=0;

         }
				 else if(Alarm[0]==6)
				 {
						if((6<time_buf1[4])&&(time_buf1[4]<23)||
							(6==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						  (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<59)||
						  (time_buf1[4]==0)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((0<time_buf1[4])&&(time_buf1[4]<6)||							 
						   (6==time_buf1[4])&&(time_buf1[5]<Alarm[1])||
						   (time_buf1[4]==0 )&&(time_buf1[5]>Alarm[3]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==7)
				 {
						if((7<time_buf1[4])&&(time_buf1[4]<23)||
							 (7==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==1)&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((1<time_buf1[4])&&(time_buf1[4]<7)||							 
						   (time_buf1[4]==1)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==7)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				   else if(Alarm[0]==8)
				 {
						if((8<time_buf1[4] )&&(time_buf1[4]<23)||
							 (8==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==1 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==2 )&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((2<time_buf1[4])&&(time_buf1[4]<8)||							 
						   (time_buf1[4]==2)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==8)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				   else if(Alarm[0]==9)
				 {
						if((9<time_buf1[4])&&(time_buf1[4]<23)||
							 (9==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<3)||
						   (3==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((3<time_buf1[4])&&(time_buf1[4]<9)||							 
						   (time_buf1[4]==3)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==9)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				 else if(Alarm[0]==10)
				 {
						if((10<time_buf1[4])&&(time_buf1[4]<23)||
							 (10==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<4)||
						   (4==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((4<time_buf1[4])&&(time_buf1[4]<10)||							 
						   (time_buf1[4]==4)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==10)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				 else if(Alarm[0]==11)
				 {
						if((11<time_buf1[4])&&(time_buf1[4]<23)||
							 (11==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<5)||
						   (5==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((5<time_buf1[4])&&(time_buf1[4]<11)||							 
						   (time_buf1[4]==5)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==11)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				 else if(Alarm[0]==12)
				 {
						if((12<time_buf1[4])&&(time_buf1[4]<23)||
							 (12==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<6)||
						   (6==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((6<time_buf1[4])&&(time_buf1[4]<12)||							 
						   (time_buf1[4]==6)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==12)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==13)
				 {
						if((13<time_buf1[4])&&(time_buf1[4]<23)||
							 (13==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<7)||
						   (7==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((7<time_buf1[4])&&(time_buf1[4]<13)||							 
						   (time_buf1[4]==7)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==13)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==14)
				 {
						if((14<time_buf1[4])&&(time_buf1[4]<23)||
							 (14==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<8)||
						   (8==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((8<time_buf1[4])&&(time_buf1[4]<14)||							 
						   (time_buf1[4]==8)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==14)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==15)
				 {
						if((15<time_buf1[4])&&(time_buf1[4]<23)||
							 (15==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<9)||
						   (9==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((9<time_buf1[4])&&(time_buf1[4]<15)||							 
						   (time_buf1[4]==9)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==15)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				 
				  else if(Alarm[0]==16)
				 {
						if((16<time_buf1[4])&&(time_buf1[4]<23)||
							 (16==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<10)||
						   (10==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((10<time_buf1[4])&&(time_buf1[4]<16)||							 
						   (time_buf1[4]==10)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==16)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==17)
				 {
						if((17<time_buf1[4])&&(time_buf1[4]<23)||
							 (17==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<11)||
						   (11==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((11<time_buf1[4])&&(time_buf1[4]<17)||							 
						   (time_buf1[4]==11)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==17)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==18)
				 {
						if((18<time_buf1[4])&&(time_buf1[4]<23)||
							 (18==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<12)||
						   (12==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((12<time_buf1[4])&&(time_buf1[4]<18)||							 
						   (time_buf1[4]==12)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==18)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==19)
				 {
						if((19<time_buf1[4])&&(time_buf1[4]<23)||
							 (19==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<13)||
						   (13==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((13<time_buf1[4])&&(time_buf1[4]<19)||							 
						   (time_buf1[4]==13)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==19)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==20)
				 {
						if((20<time_buf1[4])&&(time_buf1[4]<23)||
							 (20==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<14)||
						   (14==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((14<time_buf1[4])&&(time_buf1[4]<20)||							 
						   (time_buf1[4]==14)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==20)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==21)
				 {
						if((21<time_buf1[4])&&(time_buf1[4]<23)||
							 (21==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<15)||
						   (15==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((15<time_buf1[4])&&(time_buf1[4]<21)||							 
						   (time_buf1[4]==15)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==21)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				 
				  else if(Alarm[0]==22)
				 {
						if((22==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==23)&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<16)||
						   (16==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((16<time_buf1[4])&&(time_buf1[4]<22)||							 
						   (time_buf1[4]==16)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==22)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

         }
				  else if(Alarm[0]==23)
				 {
						if((23==time_buf1[4])&&(time_buf1[5]>=Alarm[1])||
						   (time_buf1[4]==0 )&&(0<time_buf1[5])&&(time_buf1[5]<=59)||
						   (0<time_buf1[4])&&(time_buf1[4]<17)||
						   (17==time_buf1[4])&&(time_buf1[5]<Alarm[3]))
						{
								Lamp_flag1=1;	
            }
						if((17<time_buf1[4])&&(time_buf1[4]<23)||							 
						   (time_buf1[4]==16)&&(time_buf1[5]>=Alarm[3])||
						   (time_buf1[4]==23)&&(time_buf1[5]<Alarm[1]))
							  
						    Lamp_flag1=0;

        }
      }
			 if(flag3==3)
			{
            Lamp_flag1=1;
      }

}


void Pump_Modechange()
{
#if 0
	 
    
	   IRcvStr(0xae,210, &flag3,1);            //24c02读出数据
		 IRcvStr(0xae,211, &flag4,1); 
	   Pum_Mode();
	//12hour  X1
	   if((flag3==1)&&(flag4==1))
			{
         if((time_buf1[4]==Alarm_Pour1[0])&&(0<=time_buf1[5])&&(time_buf1[5]<15))
				 {
						Pump1_flag=1;
					 
         }
				 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					   (time_buf1[4]<Alarm_Pour1[0])||(time_buf1[4]>Alarm_Pour1[2]))
				 {
						Pump1_flag=0;
         }
				 
      }
	//18hour  X1		
			 if((flag3==2)&&(flag4==1))	
		 {
				if((time_buf1[4]==Alarm_Pour1[0])&&(0<=time_buf1[5])&&(time_buf1[5]<15))
				 {
						Pump1_flag=1;
					 
         }
				 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					   (time_buf1[4]<Alarm_Pour1[0])||(time_buf1[4]>Alarm_Pour1[2]))
				 {
						Pump1_flag=0;
         }

     }	
		 
 //24hour  X1
		 
			if((flag3==3)&&(flag4==1))
			{
         if((time_buf1[4]==Alarm_Pour1[0])&&(0<=time_buf1[5])&&(time_buf1[5]<15))
				 {
						Pump1_flag=1;
					 
         }
				 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					   (time_buf1[4]<Alarm_Pour1[0])||(time_buf1[4]>Alarm_Pour1[2]))
				 {
						Pump1_flag=0;
         }
				 
      }
		 
	
	
			 if((flag3==1)&&(flag4==2))
			 {
				 if(((time_buf1[4]==Alarm_Pour1[0])&&(time_buf1[5]>=0)&&(time_buf1[5]<15))||
						((time_buf1[4]==Alarm_Pour1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)))
					{
								Pump1_flag=1;	  
				  }
				 if((Alarm_Pour1[0]>=0)&&(Alarm_Pour1[0]<=17))
				 {		
						 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
								((time_buf1[4]>Alarm_Pour1[2])&&(time_buf1[4]<Alarm_Pour1[4]))||(time_buf1[4]<Alarm_Pour1[0])||
								((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
								(time_buf1[4]>Alarm_Pour1[6]))
						 {
								Pump1_flag=0;
						 }
				 }
				 
				 
				  if(Alarm_Pour1[0]==18)
					{
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>0)&&(time_buf1[4]<18))||((time_buf1[4]>18)&&(time_buf1[4]<=23)))//||(time_buf1[4]>0))
             {
								Pump1_flag=0;	 
             }
          }
					
					 if(Alarm_Pour1[0]==19)
					{
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>1)&&(time_buf1[4]<19))||((time_buf1[4]>19)&&(time_buf1[4]<=23))||(time_buf1[4]==0))
             {
								Pump1_flag=0;	 
             }
          }
					
					 if(Alarm_Pour1[0]==20)
					{
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>2)&&(time_buf1[4]<20))||((time_buf1[4]>20)&&(time_buf1[4]<=23))||
						     (time_buf1[4]>=0)&&(time_buf1[4]<2))
             {
								Pump1_flag=0;	 
             }
          }
					
					 if(Alarm_Pour1[0]==21)
					{
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>3)&&(time_buf1[4]<21))||((time_buf1[4]>21)&&(time_buf1[4]<=23))||
						     (time_buf1[4]>=0)&&(time_buf1[4]<3))
             {
								Pump1_flag=0;	 
             }
          }
					
					 if(Alarm_Pour1[0]==22)
					{
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>4)&&(time_buf1[4]<22))||(time_buf1[4]==23)||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<4)))
             {
								Pump1_flag=0;	 
             }
          }
					
					 if(Alarm_Pour1[0]==23)
					{
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     (time_buf1[4]>=0)&&(time_buf1[4]<5)||((time_buf1[4]>5)&&(time_buf1[4]<23)))
             {
								Pump1_flag=0;	 
             }
          }
					

      } 
      if((flag3==1)&&(flag4==3))
			{
				if((time_buf1[4]==Alarm_Pour1[0])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)||
				   (time_buf1[4]==Alarm_Pour1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)||
					 (time_buf1[4]==Alarm_Pour1[8])&&(time_buf1[5]>=0)&&(time_buf1[5]<15) )
           {
							Pump1_flag=1;
           }
				 if((Alarm_Pour1[0]>=0)&&(Alarm_Pour1[0]<=15))
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]>Alarm_Pour1[2])&&(time_buf1[4]<Alarm_Pour1[4]))||(time_buf1[4]<Alarm_Pour1[0])||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    (time_buf1[4]>Alarm_Pour1[6])&&(time_buf1[4]<Alarm_Pour1[8])||
					    (( time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15))||
					    (time_buf1[4]>Alarm_Pour1[10]))
           {
							Pump1_flag=0;
           }
					 
				 }
				 
				 if(Alarm_Pour1[0]==16)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>0)&&(time_buf1[4]<16))||((time_buf1[4]>16)&&(time_buf1[4]<20))||
					    ((time_buf1[4]>20)&&(time_buf1[4]<=23)))
           {
							Pump1_flag=0;
           }

         }
				 if(Alarm_Pour1[0]==17)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>1)&&(time_buf1[4]<17))||((time_buf1[4]>17)&&(time_buf1[4]<21))||
					    ((time_buf1[4]>21)&&(time_buf1[4]<=23))||(time_buf1[4]==0))
           {
							Pump1_flag=0;
           }

         }
				 if(Alarm_Pour1[0]==18)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>2)&&(time_buf1[4]<18))||((time_buf1[4]>18)&&(time_buf1[4]<22))||
					    (time_buf1[4]==23)||((time_buf1[4]>=0)&&(time_buf1[4]<2)))
           {
							Pump1_flag=0;
           }

         }
				 
				  if(Alarm_Pour1[0]==19)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>3)&&(time_buf1[4]<19))||((time_buf1[4]>19)&&(time_buf1[4]<23))||
					    ((time_buf1[4]>=0)&&(time_buf1[4]<3)))
           {
							Pump1_flag=0;
           }

         }
				 
				   if(Alarm_Pour1[0]==20)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>4)&&(time_buf1[4]<20))||((time_buf1[4]>20)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>0)&&(time_buf1[4]<4)))
           {
							Pump1_flag=0;
           }

         }
				   if(Alarm_Pour1[0]==21)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>5)&&(time_buf1[4]<21))||((time_buf1[4]>21)&&(time_buf1[4]<=23))||
					    (time_buf1[4]==0)||((time_buf1[4]>1)&&(time_buf1[4]<5)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==22)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>6)&&(time_buf1[4]<22))||(time_buf1[4]==23)||
					    ((time_buf1[4]>=0)&&(time_buf1[4]<2))||((time_buf1[4]>2)&&(time_buf1[4]<6)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==23)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>7)&&(time_buf1[4]<23))||
					    ((time_buf1[4]>=0)&&(time_buf1[4]<3))||((time_buf1[4]>3)&&(time_buf1[4]<7)))
           {
							Pump1_flag=0;
           }

         }
				 
				 

      }	
		
//LED灯工作18小时，泵工作三种模式			
    
     
    if((flag3==2)&&(flag4==2))	
		{
			if(((time_buf1[4]==Alarm_Pour1[0])&&(time_buf1[5]>=0)&&(time_buf1[5]<15))||
				     ((time_buf1[4]==Alarm_Pour1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)))
       {
							Pump1_flag=1;	  
       } 
			
			  if((Alarm_Pour1[0]>=0)&&(Alarm_Pour1[0]<=14))
				{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]>Alarm_Pour1[2])&&(time_buf1[4]<Alarm_Pour1[4]))||(time_buf1[4]<Alarm_Pour1[0])||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    (time_buf1[4]>Alarm_Pour1[6]))
           {
							Pump1_flag=0;
           }
         }
				 if(Alarm_Pour1[0]==15)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>0)&&(time_buf1[4]<15))||((time_buf1[4]>15)&&(time_buf1[4]<=23)))
             {
								Pump1_flag=0;	 
             }

         }
				 if(Alarm_Pour1[0]==16)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>1)&&(time_buf1[4]<16))||((time_buf1[4]>16)&&(time_buf1[4]<=23))||(time_buf1[4]==0))
             {
								Pump1_flag=0;	 
             }

         }
				 	if(Alarm_Pour1[0]==17)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>2)&&(time_buf1[4]<17))||((time_buf1[4]>17)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<2)))
             {
								Pump1_flag=0;	 
             }

         }
				 if(Alarm_Pour1[0]==18)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>3)&&(time_buf1[4]<18))||((time_buf1[4]>18)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<3)))
             {
								Pump1_flag=0;	 
             }

         }
				  if(Alarm_Pour1[0]==19)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>4)&&(time_buf1[4]<19))||((time_buf1[4]>19)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<4)))
             {
								Pump1_flag=0;	 
             }

         }
				 if(Alarm_Pour1[0]==20)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>5)&&(time_buf1[4]<20))||((time_buf1[4]>20)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<5)))
             {
								Pump1_flag=0;	 
             }

         }
				 if(Alarm_Pour1[0]==21)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>6)&&(time_buf1[4]<21))||((time_buf1[4]>21)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<6)))
             {
								Pump1_flag=0;	 
             }

         }
				 if(Alarm_Pour1[0]==22)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>7)&&(time_buf1[4]<22))||(time_buf1[4]==23)||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<7)))
             {
								Pump1_flag=0;	 
             }

         }
				 if(Alarm_Pour1[0]==23)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>8)&&(time_buf1[4]<23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<7)))
             {
								Pump1_flag=0;	 
             }

         }
				 
				 
    }	

   	if((flag3==2)&&(flag4==3))	
		{
			
			if((time_buf1[4]==Alarm_Pour1[0])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)||
				     (time_buf1[4]==Alarm_Pour1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)||
					   (time_buf1[4]==Alarm_Pour1[8])&&(time_buf1[5]>=0)&&(time_buf1[5]<15) )
      {
							Pump1_flag=1;
      }

			
			 if((Alarm_Pour1[0]>=0)&&(Alarm_Pour1[0]<=11))
			 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]>Alarm_Pour1[2])&&(time_buf1[4]<Alarm_Pour1[4]))||(time_buf1[4]<Alarm_Pour1[0])||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    (time_buf1[4]>Alarm_Pour1[6])&&(time_buf1[4]<Alarm_Pour1[8])||
					    (( time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15))||
					    (time_buf1[4]>Alarm_Pour1[10]))
           {
							Pump1_flag=0;
           }
				}
			if(Alarm_Pour1[0]==12)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>0)&&(time_buf1[4]<12))||((time_buf1[4]>12)&&(time_buf1[4]<18))||
					    ((time_buf1[4]>18)&&(time_buf1[4]<=23)))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==13)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>1)&&(time_buf1[4]<13))||((time_buf1[4]>13)&&(time_buf1[4]<19))||
					    ((time_buf1[4]>19)&&(time_buf1[4]<=23))||(time_buf1[4]==0))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==14)
			{

					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>2)&&(time_buf1[4]<14))||((time_buf1[4]>14)&&(time_buf1[4]<20))||
					    ((time_buf1[4]>20)&&(time_buf1[4]<=23))||((time_buf1[4]>=0)&&(time_buf1[4]<2)))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==15)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>3)&&(time_buf1[4]<15))||((time_buf1[4]>15)&&(time_buf1[4]<21))||
					    ((time_buf1[4]>21)&&(time_buf1[4]<=23))||((time_buf1[4]>=0)&&(time_buf1[4]<3)))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==16)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>4)&&(time_buf1[4]<16))||((time_buf1[4]>16)&&(time_buf1[4]<22))||
					    (time_buf1[4]==23)||((time_buf1[4]>=0)&&(time_buf1[4]<4)))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==17)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>5)&&(time_buf1[4]<17))||((time_buf1[4]>17)&&(time_buf1[4]<23))||
					    ((time_buf1[4]>=0)&&(time_buf1[4]<5)))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==18)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>6)&&(time_buf1[4]<18))||((time_buf1[4]>18)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>0)&&(time_buf1[4]<6)))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==19)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>7)&&(time_buf1[4]<19))||((time_buf1[4]>19)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>1)&&(time_buf1[4]<7))||(time_buf1[4]==0))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==20)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>8)&&(time_buf1[4]<20))||((time_buf1[4]>20)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>2)&&(time_buf1[4]<8))||((time_buf1[4]>=0)&&(time_buf1[4]<2)))
           {
							Pump1_flag=0;
           }
      }
			if(Alarm_Pour1[0]==21)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>9)&&(time_buf1[4]<21))||((time_buf1[4]>21)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>3)&&(time_buf1[4]<9))||((time_buf1[4]>=0)&&(time_buf1[4]<3)))
           {
							Pump1_flag=0;
           }
      }
				if(Alarm_Pour1[0]==22)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>10)&&(time_buf1[4]<22))||((time_buf1[4]==23))||
					    ((time_buf1[4]>4)&&(time_buf1[4]<10))||((time_buf1[4]>=0)&&(time_buf1[4]<4)))
           {
							Pump1_flag=0;
           }
      }
				if(Alarm_Pour1[0]==23)
			{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>11)&&(time_buf1[4]<23))||
					    ((time_buf1[4]>5)&&(time_buf1[4]<11))||((time_buf1[4]>=0)&&(time_buf1[4]<5)))
           {
							Pump1_flag=0;
           }
      }
    }
		
//灯工作24小时，泵工作的三种模式	
		
		//#2	
			if((flag3==3)&&(flag4==2))
			{
				if(((time_buf1[4]==Alarm_Pour1[0])&&(time_buf1[5]>=0)&&(time_buf1[5]<15))||
				     ((time_buf1[4]==Alarm_Pour1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)))
        {
							Pump1_flag=1;	  
        }
				if((Alarm_Pour1[0]>=0)&&(Alarm_Pour1[0]<=11))
				{	  
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]>Alarm_Pour1[2])&&(time_buf1[4]<Alarm_Pour1[4]))||(time_buf1[4]<Alarm_Pour1[0])||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    (time_buf1[4]>Alarm_Pour1[6]))
           {
							Pump1_flag=0;
           }
				 }
				 if(Alarm_Pour1[0]==12)
				 {

             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>0)&&(time_buf1[4]<12))||((time_buf1[4]>12)&&(time_buf1[4]<=23)))
             {
								Pump1_flag=0;	 
             }
         }
         if(Alarm_Pour1[0]==13)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>1)&&(time_buf1[4]<13))||((time_buf1[4]>13)&&(time_buf1[4]<=23))||
						      (time_buf1[4]==0))
             {
								Pump1_flag=0;	 
             }
         }
				 if(Alarm_Pour1[0]==14)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>2)&&(time_buf1[4]<14))||((time_buf1[4]>14)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<2)))
             {
								Pump1_flag=0;	 
             }
         }
				  if(Alarm_Pour1[0]==15)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>3)&&(time_buf1[4]<15))||((time_buf1[4]>15)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<3)))
             {
								Pump1_flag=0;	 
             }
         }
				  if(Alarm_Pour1[0]==16)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>4)&&(time_buf1[4]<16))||((time_buf1[4]>16)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<4)))
             {
								Pump1_flag=0;	 
             }
         }
				  if(Alarm_Pour1[0]==17)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>5)&&(time_buf1[4]<17))||((time_buf1[4]>17)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<5)))
             {
								Pump1_flag=0;	 
             }
         }
				 if(Alarm_Pour1[0]==18)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>6)&&(time_buf1[4]<18))||((time_buf1[4]>18)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<6)))
             {
								Pump1_flag=0;	 
             }
         }
				  if(Alarm_Pour1[0]==19)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>7)&&(time_buf1[4]<19))||((time_buf1[4]>19)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<7)))
             {
								Pump1_flag=0;	 
             }
         }
				 if(Alarm_Pour1[0]==20)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>8)&&(time_buf1[4]<20))||((time_buf1[4]>20)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<8)))
             {
								Pump1_flag=0;	 
             }
         }
				 if(Alarm_Pour1[0]==21)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>9)&&(time_buf1[4]<21))||((time_buf1[4]>21)&&(time_buf1[4]<=23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<9)))
             {
								Pump1_flag=0;	 
             }
         }
				  if(Alarm_Pour1[0]==22)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>10)&&(time_buf1[4]<22))||(time_buf1[4]==23)||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<10)))
             {
								Pump1_flag=0;	 
             }
         }
				  if(Alarm_Pour1[0]==23)
				 {
             if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
							   ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						     ((time_buf1[4]>11)&&(time_buf1[4]<23))||
						     ((time_buf1[4]>=0)&&(time_buf1[4]<11)))
             {
								Pump1_flag=0;	 
             }
         }
     } 
		// #3
      if((flag3==3)&&(flag4==3))
			{
				
				if((time_buf1[4]==Alarm_Pour1[0])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)||
				     (time_buf1[4]==Alarm_Pour1[4])&&(time_buf1[5]>=0)&&(time_buf1[5]<15)||
					   (time_buf1[4]==Alarm_Pour1[8])&&(time_buf1[5]>=0)&&(time_buf1[5]<15) )
           {
							Pump1_flag=1;
           }
				
				
				if((Alarm_Pour1[0]>=0)&&(Alarm_Pour1[0]<=7))
				{
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]>Alarm_Pour1[2])&&(time_buf1[4]<Alarm_Pour1[4]))||(time_buf1[4]<Alarm_Pour1[0])||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    (time_buf1[4]>Alarm_Pour1[6])&&(time_buf1[4]<Alarm_Pour1[8])||
					    (( time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15))||
					    (time_buf1[4]>Alarm_Pour1[10]))
           {
							Pump1_flag=0;
           }
				 }
				 if(Alarm_Pour1[0]==8)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>0)&&(time_buf1[4]<8))||((time_buf1[4]>8)&&(time_buf1[4]<16))||
					    ((time_buf1[4]>16)&&(time_buf1[4]<=23)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==9)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>1)&&(time_buf1[4]<9))||((time_buf1[4]>9)&&(time_buf1[4]<17))||
					    ((time_buf1[4]>17)&&(time_buf1[4]<=23))||(time_buf1[4]==0))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==10)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>2)&&(time_buf1[4]<10))||((time_buf1[4]>10)&&(time_buf1[4]<18))||
					    ((time_buf1[4]>18)&&(time_buf1[4]<=23))||((time_buf1[4]>=0)&&(time_buf1[4]<2)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==11)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>3)&&(time_buf1[4]<11))||((time_buf1[4]>11)&&(time_buf1[4]<19))||
					    ((time_buf1[4]>19)&&(time_buf1[4]<=23))||((time_buf1[4]>=0)&&(time_buf1[4]<3)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==12)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>4)&&(time_buf1[4]<12))||((time_buf1[4]>12)&&(time_buf1[4]<20))||
					    ((time_buf1[4]>20)&&(time_buf1[4]<=23))||((time_buf1[4]>=0)&&(time_buf1[4]<4)))
           {
							Pump1_flag=0;
           }

         }
         if(Alarm_Pour1[0]==13)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>5)&&(time_buf1[4]<13))||((time_buf1[4]>13)&&(time_buf1[4]<21))||
					    ((time_buf1[4]>21)&&(time_buf1[4]<=23))||((time_buf1[4]>=0)&&(time_buf1[4]<5)))
           {
							Pump1_flag=0;
           }

         }
				 if(Alarm_Pour1[0]==14)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>6)&&(time_buf1[4]<14))||((time_buf1[4]>14)&&(time_buf1[4]<22))||
					    (time_buf1[4]==23)||((time_buf1[4]>=0)&&(time_buf1[4]<6)))
           {
							Pump1_flag=0;
           }

         }
				 if(Alarm_Pour1[0]==15)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>7)&&(time_buf1[4]<15))||((time_buf1[4]>15)&&(time_buf1[4]<23))||
					    ((time_buf1[4]>=0)&&(time_buf1[4]<7)))
           {
							Pump1_flag=0;
           }

         }
				 if(Alarm_Pour1[0]==16)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>8)&&(time_buf1[4]<16))||((time_buf1[4]>16)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>0)&&(time_buf1[4]<8)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==17)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>9)&&(time_buf1[4]<17))||((time_buf1[4]>17)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>1)&&(time_buf1[4]<9))||(time_buf1[4]==0))
           {
							Pump1_flag=0;
           }

         }
				 if(Alarm_Pour1[0]==18)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>10)&&(time_buf1[4]<18))||((time_buf1[4]>18)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>2)&&(time_buf1[4]<10))||((time_buf1[4]>=0)&&(time_buf1[4]<2)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==19)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>11)&&(time_buf1[4]<19))||((time_buf1[4]>19)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>3)&&(time_buf1[4]<11))||((time_buf1[4]>=0)&&(time_buf1[4]<3)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==20)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>12)&&(time_buf1[4]<20))||((time_buf1[4]>20)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>4)&&(time_buf1[4]<12))||((time_buf1[4]>=0)&&(time_buf1[4]<4)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==21)
				 {
					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>13)&&(time_buf1[4]<21))||((time_buf1[4]>21)&&(time_buf1[4]<=23))||
					    ((time_buf1[4]>5)&&(time_buf1[4]<13))||((time_buf1[4]>=0)&&(time_buf1[4]<5)))
           {
							Pump1_flag=0;
           }

         }
				 if(Alarm_Pour1[0]==22)
				 {

					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>14)&&(time_buf1[4]<22))||(time_buf1[4]==23)||
					    ((time_buf1[4]>6)&&(time_buf1[4]<14))||((time_buf1[4]>=0)&&(time_buf1[4]<6)))
           {
							Pump1_flag=0;
           }

         }
				  if(Alarm_Pour1[0]==23)
				 {

					 if(((time_buf1[4]==Alarm_Pour1[2])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
						  ((time_buf1[4]==Alarm_Pour1[6])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]==Alarm_Pour1[10])&&(time_buf1[5]>=15)&&(time_buf1[5]<=59))||
					    ((time_buf1[4]>15)&&(time_buf1[4]<23))||
					    ((time_buf1[4]>7)&&(time_buf1[4]<15))||((time_buf1[4]>=0)&&(time_buf1[4]<7)))
           {
							Pump1_flag=0;
           }

         }
				 
		  }
#endif
}

