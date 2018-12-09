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




sbit beep       = P3^0;    //����������˿�
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


void Sys_Start_Modechage();
void Pump_Modechange();

void Bell()
{
		beep=0;
	  DelayMs(100);
	  beep=1;
}


/*************�̵������ƺ���**************/
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
          
        //Lamp����12hr
				
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


           //Lamp����18Hr
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
						
						//Lamp����24hr
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
		  
		  Bell();
		 	
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
				  Sys_Start_Modechage();
				  Cflag3=0;
       } 
				
			 if(Cflag4==1)    //��ģʽ�仯��־
			 {

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

//ϵͳʱ��ı俪ʼʱ��LEDģʽ�ı����LED�ĵ�ǰ״̬
void Sys_Start_Modechage()
{
	 IRcvStr(0xae,80,&Alarm[0],1);         //��24c02�������� 
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
	 
    
	   IRcvStr(0xae,210, &flag3,1);            //24c02��������
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
		
//LED�ƹ���18Сʱ���ù�������ģʽ			
    
     
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
		
//�ƹ���24Сʱ���ù���������ģʽ	
		
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

