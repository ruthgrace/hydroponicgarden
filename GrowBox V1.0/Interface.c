#include "Interface.h"

extern unsigned char code pic0[];
extern unsigned char Alarm[5];

extern unsigned char Alarm_Pour1[16];


extern unsigned char Alarm_Temp[5];

float  temperature;

extern unsigned char a[4];
extern unsigned char b[4];
extern unsigned char t[4];
unsigned char temp[16];//������ʾ������ʱ�洢����


unsigned int   flag1=1; //�˵�ѡ���־
unsigned int   flag2=1; //��������λѡ���־
unsigned char  flag3=2; //
unsigned char  flag4=1; //
unsigned int   flag5=1; //
unsigned int   flag6=1; //���ȡ��á��ƹ���ѡ������־
unsigned char  flag7=3; //���ȹ�����ʽѡ��Ĭ�ϴ��������У�
unsigned int   flag8=1; // ��ǰʱ�����ñ�־

extern unsigned char Alarmflag; //���ӱ�����־
extern unsigned char Alarmswitch;

void move_selection(int flag)
{
	switch(flag)
	{
	  case 1:   LCD_PutString(7,2," *");
		        	LCD_PutString(7,3,"  ");
							LCD_PutString(7,4,"  "); 
							break;
	  case 2:   LCD_PutString(7,2,"  ");
							LCD_PutString(7,3," *");
							LCD_PutString(7,4,"  "); 
							break;
	  case 3:   LCD_PutString(7,2,"  ");
							LCD_PutString(7,3,"  ");
							LCD_PutString(7,4," *"); 
							break;
  }
}

void interface_display(int x) 
{
      switch(x)
	  {	 //��������ʾ
	     case MainInterface:  
		          //DelayMs(20);
		          sprintf(temp,"%02d-%02d-20%02d  ",(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[1]);//������
              LCD_Display_Array(0,0,temp,12);
	
	               switch(time_buf1[7])
	               {
										case 1:   LCD_PutString(6,1,"Mon ");
														  break;
										case 2:   LCD_PutString(6,1,"Tue ");
															break;
										case 3:   LCD_PutString(6,1,"Wed ");
															break;
										case 4:   LCD_PutString(6,1,"Thu ");
															break;
										case 5:   LCD_PutString(6,1,"Fri ");
															break;
										case 6:   LCD_PutString(6,1,"Sat ");
															break;
										case 7:   LCD_PutString(6,1,"Sun ");
															break;
	               }
	              
/*							huatu(0,num1632[time_buf1[4]/10]);
								huatu(1,num1632[time_buf1[4]%10]);
								huatu(2,num1632[10]);
								huatu(3,num1632[time_buf1[5]/10]);
								huatu(4,num1632[time_buf1[5]%10]);
								huatu(5,num1632[10]);
								huatu(6,num1632[time_buf1[6]/10]);
								huatu(7,num1632[time_buf1[6]%10]);
*/							
              sprintf(temp,"    %02d:%02d:%02d    ",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//ʱ����
              LCD_Display_Array(1,0,temp,16);
														 
							LCD_PutString(0,4,"WaterTemp:");
	            LCD_Display_Array(3,5,t,4);
              DisplayCGRAM(7,3);
								 
							LCD_PutString(0,3,"H:");
							LCD_Display_Array(2,1,a,4);
							LCD_PutString(3,3,"% ");
								

              LCD_PutString(4,3,"T:");
							LCD_Display_Array(2,5,b,4);
							DisplayCGRAM(7,4);

								break;
/**********************************���˵�������ʾ********************************/        
	case  Mainmenu:  
		//DelayMs(20);
		LCD_PutString(0,1,"   Main Menu    ");
		LCD_PutString(0,2,"Date/Time Set ");
	  LCD_PutString(0,3,"Schedule      ");
	  LCD_PutString(0,4,"LED/Pump/Fan  ");
		move_selection(flag1);
		break;
/****************************ʱ����������ѡ����ʾ����*********************************/
	case  Date_Time_Setting:
		//DelayMs(20);
		LCD_PutString(0,1," Date Time Set  ");
		LCD_PutString(0,2,"Date Setting  ");
		LCD_PutString(0,3,"Time Setting  ");
		LCD_PutString(0,4,"              ");	
	  move_selection(flag1);
		break;	
								
/****************************�Ʊ�ģʽ����ѡ�����*******************************************/		
	case  Lamp_Pump_Fan_Setting:
		//DelayMs(20);
		LCD_PutString(0,1,"LED Pump Fan Set");
		LCD_PutString(0,2,"LED  Mode     "); 
		LCD_PutString(0,3,"Pump Mode     ");
		LCD_PutString(0,4,"Fan  Mode     ");
		move_selection(flag6);
		break;
/******************************��ʼʱ������ý���***********************************/
			case  AlarmSetInterface:  
		          //DelayMs(20);
				  
				         switch(flag5)
	               {
	                case 1:   LCD_PutString(0,1,"  Start Time    ");
		                        LCD_PutString(0,2,"       :        ");
				                    LCD_PutString(0,3,"    --          ");
		                        break;
									case 2:   LCD_PutString(0,1,"  Start Time    ");
														LCD_PutString(0,2,"       :        ");
														LCD_PutString(0,3,"        --      ");
														break;

		           }
				  sprintf(temp,"%02d",(int)Alarm[0]);//��
                  LCD_Display_Array(1,2,temp,2);
				  sprintf(temp,"%02d",(int)Alarm[1]);//��
                  LCD_Display_Array(1,4,temp,2);

 
		          break;
							 						
/*******************************���뵽�������ý���**********************************/			 
		case  DateSetInterface:  
		              //DelayMs(20);
				          switch(flag2)
	               {
										case 1:   LCD_PutString(0,1,"  Date Setting  ");
									            LCD_PutString(0,2,"  20  --  --    ");
															LCD_PutString(0,3,"    --          ");
															break;
										case 2:   LCD_PutString(0,1,"  Date Setting  ");
															LCD_PutString(0,2,"  20  --  --    ");
															LCD_PutString(0,3,"        --      ");
															break;
										case 3:   LCD_PutString(0,1,"  Date Setting  ");
															LCD_PutString(0,2,"  20  --  --    ");
															LCD_PutString(0,3,"            --  ");
															break;
		             }
									sprintf(temp,"%02d",(int)time_buf1[1]);//��
													LCD_Display_Array(1,2,temp,2);
									sprintf(temp,"%02d",(int)time_buf1[2]);//��
													LCD_Display_Array(1,4,temp,2);
									sprintf(temp,"%02d",(int)time_buf1[3]);//��
													LCD_Display_Array(1,6,temp,2);
				  
										break;
/**************************************���뵽ʱ�����ý���**********************************/
	   	case  TimeSetInterface:  
									//DelayMs(20);
									switch(flag8)
	               {
	                case 1:   LCD_PutString(0,1," Time Setting  ");
														LCD_PutString(0,2,"      :        ");
														LCD_PutString(0,3,"    --         ");
														break;
									case 2:	 LCD_PutString(0,1," Time Setting    ");
														LCD_PutString(0,2,"      :         ");
														LCD_PutString(0,3,"        --      ");
														break;

		           }
								sprintf(temp,"%02d",(int)time_buf1[4]);//��
												LCD_Display_Array(1,2,temp,2);
								sprintf(temp,"%02d",(int)time_buf1[5]);//��
												LCD_Display_Array(1,4,temp,2);

				  
		             break;

							 
/**************************************�����ģʽѡ�����**********************************/							 
	case  Lamp_Mode :
		//DelayMs(20);
		LCD_PutString(0,1,"LED Mode Select ");
		LCD_PutString(0,2,"Mode1   12hr  ");
		LCD_PutString(0,3,"Mode2   18hr  ");
		LCD_PutString(0,4,"Mode3   24hr  ");
		move_selection(flag3);
		break;
/************************************�����ģʽѡ�����************************************/
	case  Pump_Mode:
		//DelayMs(20);
		LCD_PutString(0,1,"Pump Mode Select");
		LCD_PutString(0,2,"Mode1   1X    ");
		LCD_PutString(0,3,"Mode2   2X    ");
		LCD_PutString(0,4,"Mode3   3X    ");
		move_selection(flag4);
		break;
							 
/************************************�������ģʽѡ�����************************************/
							 
	case  Fan_Mode :
		//DelayMs(20);
		LCD_PutString(0,1,"Fan Mode Select ");
		LCD_PutString(0,2,"High          ");
		LCD_PutString(0,3,"Medium        ");
		LCD_PutString(0,4,"Low           ");
		move_selection(flag7);
		break;			 
								

	  }
	  
}