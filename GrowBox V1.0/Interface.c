#include "Interface.h"

extern unsigned char code pic0[];
extern unsigned char Alarm[5];

extern unsigned char Alarm_Pour1[16];


extern unsigned char Alarm_Temp[5];

float  temperature;

extern unsigned char a[4];
extern unsigned char b[4];
extern unsigned char t[4];
unsigned char temp[16];//定义显示区域临时存储数组


unsigned int   flag1=1; //菜单选择标志
unsigned int   flag2=1; //日期设置位选择标志
unsigned char  flag3=2; //
unsigned char  flag4=1; //
unsigned int   flag5=1; //
unsigned int   flag6=1; //风扇、泵、灯工作选择界面标志
unsigned char  flag7=3; //风扇工作方式选择（默认从中速运行）
unsigned int   flag8=1; // 当前时间设置标志

extern unsigned char Alarmflag; //闹钟报警标志
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
	  {	 //主界面显示
	     case MainInterface:  
		          //DelayMs(20);
		          sprintf(temp,"%02d-%02d-20%02d  ",(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[1]);//年月日
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
              sprintf(temp,"    %02d:%02d:%02d    ",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//时分秒
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
/**********************************主菜单界面显示********************************/        
	case  Mainmenu:  
		//DelayMs(20);
		LCD_PutString(0,1,"   Main Menu    ");
		LCD_PutString(0,2,"Date/Time Set ");
	  LCD_PutString(0,3,"Schedule      ");
	  LCD_PutString(0,4,"LED/Pump/Fan  ");
		move_selection(flag1);
		break;
/****************************时间日期设置选择显示界面*********************************/
	case  Date_Time_Setting:
		//DelayMs(20);
		LCD_PutString(0,1," Date Time Set  ");
		LCD_PutString(0,2,"Date Setting  ");
		LCD_PutString(0,3,"Time Setting  ");
		LCD_PutString(0,4,"              ");	
	  move_selection(flag1);
		break;	
								
/****************************灯泵模式设置选择界面*******************************************/		
	case  Lamp_Pump_Fan_Setting:
		//DelayMs(20);
		LCD_PutString(0,1,"LED Pump Fan Set");
		LCD_PutString(0,2,"LED  Mode     "); 
		LCD_PutString(0,3,"Pump Mode     ");
		LCD_PutString(0,4,"Fan  Mode     ");
		move_selection(flag6);
		break;
/******************************开始时间点设置界面***********************************/
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
				  sprintf(temp,"%02d",(int)Alarm[0]);//年
                  LCD_Display_Array(1,2,temp,2);
				  sprintf(temp,"%02d",(int)Alarm[1]);//年
                  LCD_Display_Array(1,4,temp,2);

 
		          break;
							 						
/*******************************进入到日期设置界面**********************************/			 
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
									sprintf(temp,"%02d",(int)time_buf1[1]);//年
													LCD_Display_Array(1,2,temp,2);
									sprintf(temp,"%02d",(int)time_buf1[2]);//年
													LCD_Display_Array(1,4,temp,2);
									sprintf(temp,"%02d",(int)time_buf1[3]);//年
													LCD_Display_Array(1,6,temp,2);
				  
										break;
/**************************************进入到时间设置界面**********************************/
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
								sprintf(temp,"%02d",(int)time_buf1[4]);//年
												LCD_Display_Array(1,2,temp,2);
								sprintf(temp,"%02d",(int)time_buf1[5]);//年
												LCD_Display_Array(1,4,temp,2);

				  
		             break;

							 
/**************************************进入灯模式选择界面**********************************/							 
	case  Lamp_Mode :
		//DelayMs(20);
		LCD_PutString(0,1,"LED Mode Select ");
		LCD_PutString(0,2,"Mode1   12hr  ");
		LCD_PutString(0,3,"Mode2   18hr  ");
		LCD_PutString(0,4,"Mode3   24hr  ");
		move_selection(flag3);
		break;
/************************************进入泵模式选择界面************************************/
	case  Pump_Mode:
		//DelayMs(20);
		LCD_PutString(0,1,"Pump Mode Select");
		LCD_PutString(0,2,"Mode1   1X    ");
		LCD_PutString(0,3,"Mode2   2X    ");
		LCD_PutString(0,4,"Mode3   3X    ");
		move_selection(flag4);
		break;
							 
/************************************进入风扇模式选择界面************************************/
							 
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