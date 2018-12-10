#include "key.h"
#include "delay.h"

unsigned char Lamp_flag=1;
unsigned char Lamp_flag2;
unsigned char Confirm=0;
unsigned char Save_Data=0;
unsigned char year;
unsigned char temp8;
unsigned char flagc=0;
unsigned char CTimeflag=0;
unsigned char Cflag3=0;
unsigned char Cflag4=0;

unsigned char  Fan_flag=0;
unsigned char Alarm_Temp[5];
unsigned char timebuf1_Temp[8];

unsigned char S_flag=1;     //��ʼʱ�䲻ȷ�ϱ���ԭʼֵ��־
unsigned char C_flag=1;     //ϵͳʱ���޸Ĳ����汣��ԭʼֵ��־λ
unsigned char D_flag=1;      //ϵͳ�����޸Ĳ�ȷ�ϱ�־


/*------------------------------------------------
            ����ɨ�躯��������ɨ���ֵ
------------------------------------------------*/

unsigned char KeyScan(void)
{
    unsigned char i;
	  unsigned int  key_press_num=0;
    
    HC165_CK = 1;
    HC165_nPL = 0;        //HC165������
    HC165_nPL = 1;        //��ͣHC165������
    temp8 = 0;
    if(HC165_OUT == 1) temp8 |= 0x01;
    for(i = 0;i < 7;i++)
    {    
        HC165_CK = 0;  
        HC165_CK = 1;  
       temp8 <<= 1;
        if(HC165_OUT == 1) 
        {
            temp8 |= 0x01;
        }
    }
    HC165_CK = 0;
 
		  if(!Power) 
      {
          DelayMs(10);
          if(!Power)
					{
            while(!Power)
						{
								key_press_num++;
                DelayMs(10);           //10x200=2000ms=2s 
							  if(key_press_num==200)
								break;
						} 
          }						
		      if(key_press_num==200) //��Լ2s
					{									 
						 return 9;
          }	
				  if(key_press_num<200)
		 		 {
						 return 10;
         } 
                							
            key_press_num=0;
           
     }
      
	    else if(temp8==0xfe)  { return 5; DelayMs(10);}   //Fan   5   1
			else if(temp8==0xfd)  { return 4; DelayMs(10);}   //lamp  4   2
			else if(temp8==0xfb)	{ return 6; DelayMs(10);}   //Pump1 6   3
			else if(temp8==0xf7)  { return 7; DelayMs(10);}   //Pump2 7   4
			else if(temp8==0xef)  { return 3; DelayMs(10);}   //UP    3   5
			else if(temp8==0xdf)	{ return 8; DelayMs(10);}   //EXit  8   6
			else if(temp8==0xbf)  { return 1; DelayMs(10);}   //Set   1   7
			else if(temp8==0x7f)  { return 2; DelayMs(10);}   //Down  2   8
			else  return 0;
	   

}

//later, use the interface to determine the flag
int move_flag(int flag, int direction, int options)
{
	switch(direction)
	{
		case 2: // up
			flag++;
		  if (flag > options)
			{
				flag = 1;
			}
			break;
		case 3: //down
			flag--;
		  if (flag < 1)
			{
				flag = options;
			}
			break;
	}
	return flag;
}

void up_down_press(int Interface, int direction)
{
	switch(Interface)
	{
		case Mainmenu:
			flag1 = move_flag(flag1, direction, 3);
			break;
		case Pump_Mode:
			flag4 = move_flag(flag4, direction, 3);
			break;
		case Lamp_Mode:
			flag3 = move_flag(flag3, direction, 3);
			Cflag3=1;
			break;
		case Date_Time_Setting:
			flag1 = move_flag(flag1, direction, 2);
		  break;
		case Lamp_Pump_Fan_Setting:
			flag6 = move_flag(flag6, direction, 3);
			break;
		case Fan_Mode:
			flag7 = move_flag(flag7, direction, 3);
			break;
	}
}

void Keyprocessing(int x)   //����ֵ������
{
    switch(x)
	  {  
	    case 1:  if(Interface==MainInterface)
		           {
									Interface=Mainmenu;
							 }
						 
								else if(Interface==Mainmenu)
		           {
								  switch(flag1)
								  {
											case 1:  Interface=Date_Time_Setting;    break;
										
											case 2:  Interface=AlarmSetInterface;    break;
										
										  case 3:  Interface=Lamp_Pump_Fan_Setting;break;

									}
							 }
							 else if(Interface==Date_Time_Setting)
							 {
									switch(flag1)
								  {
											case 1: Interface=DateSetInterface;
										          timebuf1_Temp[1]=time_buf1[1];
								              timebuf1_Temp[2]=time_buf1[2];
								              timebuf1_Temp[3]=time_buf1[3];
										          break;
										
										  case 2: Interface=TimeSetInterface;
										          timebuf1_Temp[4]=time_buf1[4];
									            timebuf1_Temp[5]=time_buf1[5];
										          break;
									}
									
               }
							 	 else if(Interface==DateSetInterface)
		          {
								 flag2++;
								 if(flag2==4)
								 {
									 flag2=1;
								   Interface=Date_Time_Setting;
								 }

									timebuf1_Temp[1]=time_buf1[1];
								  timebuf1_Temp[2]=time_buf1[2];
								  timebuf1_Temp[3]=time_buf1[3];
															 
              }
							else if(Interface==TimeSetInterface)
							{
                  flag8++;
								  if(flag8==3)
									{
										flag8=1;
										Interface=Date_Time_Setting;
                  }
//									C_flag=1;
									
									timebuf1_Temp[4]=time_buf1[4];
									timebuf1_Temp[5]=time_buf1[5];
              }
							else if(Interface==AlarmSetInterface)
							{
								 
								 flag5++;
								 if(flag5==3)
								 {
										flag5=1;
									  Interface=Mainmenu;
                 }

//							   S_flag=1;
								 IRcvStr(0xae,80,&Alarm[0],1);         //��24c02�������� 
		             IRcvStr(0xae,81,&Alarm[1],1); 
								 
							    Alarm_Temp[0]=Alarm[0];
								  Alarm_Temp[1]=Alarm[1];
								 								 
              }
							
							 else if(Interface==Lamp_Pump_Fan_Setting)
							 {	
									switch(flag6)
								  {
											case 1: Interface=Lamp_Mode;break;
										
										  case 2: Interface=Pump_Mode;break;
										  
										  case 3: Interface=Fan_Mode; break;
									}

               }

							 						

/******************************����Lamp��ʼʱ�䲢�˳���LAMP��Pumpѡ�����******************************/				 

							 else if(Interface==Lamp_Mode)
							 {
								 
								  LED_Mode(flag3);
								  
								  ISendStr(0xae,82,&Alarm[2],1); //д��24c02
									ISendStr(0xae,83,&Alarm[3],1); //д��24c02
								  ISendStr(0xae,210,&flag3,1);   //д��24c02 
								  
								  DelayMs(2);
								  Interface=Lamp_Pump_Fan_Setting;
							 
								 
	            }
	
/********************************ѡ�񲢱���PUMP1�Ĺ�����ʽ***********************************/
							else if(Interface==Pump_Mode)
							{
						    	
								Pum_Mode();	
 						  	Cflag4=1;								
						    ISendStr(0xae,8,&Alarm_Pour1[0],1);  //д��24c02
							  ISendStr(0xae,16,&Alarm_Pour1[1],1); //д��24c02
							  ISendStr(0xae,24,&Alarm_Pour1[2],1); //д��24c02
							  ISendStr(0xae,32,&Alarm_Pour1[3],1); //д��24c02
							  ISendStr(0xae,40,&Alarm_Pour1[4],1); //д��24c02
							  ISendStr(0xae,48,&Alarm_Pour1[5],1); //д��24c02 
							  ISendStr(0xae,56,&Alarm_Pour1[6],1); //д��24c02
							  ISendStr(0xae,128,&Alarm_Pour1[7],1); //д��24c02
							  ISendStr(0xae,136,&Alarm_Pour1[8],1); //д��24c02
							  ISendStr(0xae,144,&Alarm_Pour1[9],1); //д��24c02
							  ISendStr(0xae,152,&Alarm_Pour1[10],1); //д��24c02
							  ISendStr(0xae,160,&Alarm_Pour1[11],1); //д��24c02 
//							  ISendStr(0xae,168,&Alarm_Pour1[12],1); //д��24c02
//							  ISendStr(0xae,176,&Alarm_Pour1[13],1); //д��24c02
//							  ISendStr(0xae,200,&Alarm_Pour1[14],1); //д��24c02
//							  ISendStr(0xae,208,&Alarm_Pour1[15],1); //д��24c02 
							 ISendStr(0xae,211,&flag4,1); //д��24c02

							  
							  DelayMs(2);
								Interface=Lamp_Pump_Fan_Setting;
							    
	       }
					else if(Interface==Fan_Mode)
					{
							ISendStr(0xae,248,&flag7,1); //д��24c02      ������ȵ�ת�ٱ�־λ
							Interface=Lamp_Pump_Fan_Setting;
					}
											
		          break;
/*************************************�Ӽ�����λ������**********************************/							 
		 case 2:
			 switch(Interface)
			 {
				 case DateSetInterface:
					 switch(flag2)
					 {
							case 1:   if(time_buf1[1]<0)
												 time_buf1[1]=95;
												else time_buf1[1]--;
													
												break;
							case 2:   time_buf1[2]--;
												if(time_buf1[2]<1)
													time_buf1[2]=12;
												break;
							case 3:  if(time_buf1[3]>=1)
											 time_buf1[3]--;
											 if(time_buf1[3]<1)
												{
													if((time_buf1[2]==1)||(time_buf1[2]==3)||(time_buf1[2]==5)||(time_buf1[2]==7)||(time_buf1[2]==8)||(time_buf1[2]==10)||(time_buf1[2]==12))
														time_buf1[3]=31;
													if((time_buf1[2]==4)||(time_buf1[2]==6)||(time_buf1[2]==9)||(time_buf1[2]==11))
														time_buf1[3]=30;
													if(time_buf1[2]==2)
													{
															year=2000+time_buf1[1];
															
															if(year%4==0&&year%100!=0||year%400==0) //����
															{
																	time_buf1[3]=29;
															}
															else time_buf1[3]=28;
													}
												}
												break;
					 }
					 time_buf1[7]= Get_Day(time_buf1[1],time_buf1[2],time_buf1[3]);
						Ds1302_Write_Time();
				   break;
         case TimeSetInterface:
					 switch(flag8)
				 {
						case 1:   if(time_buf1[4]==0)
												time_buf1[4]=23; 
											else time_buf1[4]--;
											CTimeflag=1;
											break;
						case 2:  
											if(time_buf1[5]==0)
												time_buf1[5]=59;
											else time_buf1[5]--;
											CTimeflag=1;
											break;

					 }
					 Ds1302_Write_Time();
					 break;
			   case AlarmSetInterface:
					switch(flag5)
					{
							case 1:  if(Alarm[0]==0)
												Alarm[0]=23;
												else Alarm[0]--; 
												break;
							case 2:   if(Alarm[1]==0)
												Alarm[1]=59;
												else Alarm[1]--;
												break;	
					}
						ISendStr(0xae,80,&Alarm[0],1); //д��24c02
						ISendStr(0xae,81,&Alarm[1],1); //д��24c02
				   break;
				 default:
					 up_down_press(Interface, x);
					 break;
		 }
		 break;
							        
/*************************************��������λ������**********************************/							 						
		 case 3:  if(Interface==Mainmenu) //||Interface==Pump_Mode||Interface==Mainmenu)
							{
								flag1 = move_flag(flag1, x, 3);
              }
							 else if(Interface==Pump_Mode)
							{
								flag4 = move_flag(flag4, x, 3);									
              }
							 else if(Interface==Lamp_Mode)
							{
								flag3 = move_flag(flag3, x, 3);
								Cflag3=1;
              }
							
							else if(Interface==Lamp_Pump_Fan_Setting)
							{
								flag6 = move_flag(flag6, x, 3);
              }
							 else if(Interface==Fan_Mode)
							{
								flag7 = move_flag(flag7, x, 3);
              }
							
							else if(Interface==Date_Time_Setting)
		          {
								flag1 = move_flag(flag1, x, 2);
              }
							

							else if(Interface==DateSetInterface)
							{							
									 switch(flag2)
									 {
											case 1:   time_buf1[1]++;
										            if(time_buf1[1]==96)
																	time_buf1[1]=0;
																break;
											case 2:   time_buf1[2]++;
										            if(time_buf1[2]>12)
																	time_buf1[2]=1;
																break;
										
	                      
										  case 3:		
                              	if((time_buf1[2] == 1)||(time_buf1[2] == 3)||(time_buf1[2] == 5)||(time_buf1[2] == 7)||(time_buf1[2] == 8)||(time_buf1[2] == 10)||(time_buf1[2] == 12))
																{
																	  time_buf1[3]++;
																	  if(time_buf1[3]>=32)
																			time_buf1[3]=1;
                                }
                                if(time_buf1[2]==2)
																{
																	  time_buf1[3]++;
																	  year=time_buf1[1]+2000;
																		if(year%4==0&&year%100!=0||year%400==0)
																		{
                                       if(time_buf1[3]>29)
																				time_buf1[3]=1;
                                    }
																		else if(time_buf1[3]>28)
																			
																			   time_buf1[3]=1; 

                                }	
                               if((time_buf1[2] == 4)||(time_buf1[2] == 6)||(time_buf1[2] == 9)||(time_buf1[2] ==11))
																{
																	  if(time_buf1[3]<31)
																	  time_buf1[3]++;
																	  
																	  if(time_buf1[3]==31)
																			time_buf1[3]=1;
                                }
                            																
																break;
									           
								  }       
									 time_buf1[7]= Get_Day(time_buf1[1],time_buf1[2],time_buf1[3]);  //���ڼ���
									 Ds1302_Write_Time();
									  
								}		
								
								else if(Interface==TimeSetInterface)
		           {
										   switch(flag8)
										 {
												case 1:   
												          time_buf1[4]++;
											            if(time_buf1[4]>23)
																		time_buf1[4]=0;
																	CTimeflag=1;
																	break;
												
												case 2:   time_buf1[5]++;
																	if(time_buf1[5]==60)
																	time_buf1[5]=0;
																	CTimeflag=1;
																	break;
											
										 }
										 
										 Ds1302_Write_Time();
								
							 }

							 
							else if(Interface==AlarmSetInterface)
		          {

									switch(flag5)
											{
													case 1:   
													          Alarm[0]++;
																		if(Alarm[0]>=24)
																		Alarm[0]=0;
																		break;
													case 2:   Alarm[1]++;
																	  if(Alarm[1]>=60)
																	  Alarm[1]=0;
																	  break;
												    											          
											}

										
										ISendStr(0xae,80,&Alarm[0],1); //д��24c02
									  ISendStr(0xae,81,&Alarm[1],1); //д��24c02
							 
							}
							 

							
		          break;
				
		  case 4:   Lamp_flag1=~Lamp_flag1;  break;   //
				
		  case 5:   Fan_flag=~Fan_flag;      break;
		
		  case 6:   timeflag=~timeflag;      break;     // Pump1_flag=~Pump1_flag;
				
		  case 7:   Pump2_flag=~Pump2_flag;  break;
								 			 
    	case 8:    if(Interface==Mainmenu||Date_Time_Setting||Lamp_Pump_Fan_Setting||DateSetInterface||TimeSetInterface||AlarmSetInterface||Lamp_Mode||Pump_Mode)
			           {
									  if(Interface==AlarmSetInterface)
										{
										
												Alarm[0]	= Alarm_Temp[0];
												Alarm[1]  = Alarm_Temp[1];

											
										   ISendStr(0xae,80,&Alarm[0],1); //д��24c02
									     ISendStr(0xae,81,&Alarm[1],1); //д��24c02

                    }
										if(Interface==DateSetInterface)
										{
											 D_flag=1;

											 time_buf1[1]=timebuf1_Temp[1];
								       time_buf1[2]=timebuf1_Temp[2];
								       time_buf1[3]=timebuf1_Temp[3];

								
                       time_buf1[7]= Get_Day(time_buf1[1],time_buf1[2],time_buf1[3]);  //���ڼ���											 
											 Ds1302_Write_Time();
									      	
										}
										if(Interface==TimeSetInterface)
										{

											  CTimeflag=1;

										    time_buf1[4] = timebuf1_Temp[4];
										    time_buf1[5] = timebuf1_Temp[5];

											
											 	Ds1302_Write_Time();		
										}
                     Interface=MainInterface;
									   flag1=1;
									   flag2=1;
                 }
								 break;
								 
      case 9:      TTT=~TTT;      break;    //   Power_C=~Power_C ; Power_flag=~Power_flag;  Power_flag=~Power_flag; TTT=~TTT;
	               

								 
			case 10:    LCD_flag=1;      break;
			
			
	  }
}

void LED_Mode(unsigned char flag10)
{
			IRcvStr(0xae,80,&Alarm[0],1);       //��24c02������ʼʱ��(�Ƶ�һ�ε���ʱ��)������ֵ�����������Ϩ���ʱ��
	    IRcvStr(0xae,81,&Alarm[1],1); 
			switch(flag10)
			{
										case 1:  Alarm[2]=Alarm[0]+12;  //12hour�Ľ���ʱ��
									           if(Alarm[2]>=24)
														 {
															 Alarm[2]-=24;
														   flagc=1;
														 }
														 Alarm[3]=Alarm[1];
														 Lamp_flag=1;

														 break;
										case 2:  Alarm[2]=Alarm[0]+18;  //18hour����ʱ��
									           if(Alarm[2]>=24)
														 {
															 Alarm[2]-=24;
															 flagc=1;
														 }
														 Alarm[3]=Alarm[1];
   													 Lamp_flag=2;												 
														 break;
														 
										case 3:  Alarm[2]=Alarm[0]+23; //24hour����ʱ��
									           if(Alarm[2]>=24)
														 {
															 Alarm[2]-=24;
                                flagc=1;															 
														 }
														 Alarm[3]=Alarm[1]+59;
														 Lamp_flag=3; 
											
		}
		  ISendStr(0xae,208,&Lamp_flag,1); //д��24c02
}


//ˮ����ͣʱ�����㺯��
void Pum_Mode()
{
	 
  	IRcvStr(0xae,80,&Alarm[0],1);    //��24c02������ʼʱ��(�Ƶ�һ�ε���ʱ��)������ֵ�����������Ϩ���ʱ��
    IRcvStr(0xae,210,&flag3,  1);  
	
	   switch(flag3)
	   {
	        case 1:   switch(flag4)
										{
											//LAMP��12hourģʽ��ѡ��PUMP1����һ��(ģʽ1)
											case 1:  Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ
										           if(Alarm_Pour1[0]>=24)
																 Alarm_Pour1[0]-= 24;

														   Alarm_Pour1[1]=0;              //�������ķֺ���ȫ����Ϊ0�����ڿ��Ƿ�
										          
															 Alarm_Pour1[3]= Alarm_Pour1[1]+15;  //Alarm_Pour1[3]= Alarm_Pour1[1]+15;    ��һ�ν���ʱ���

															 Alarm_Pour1[2]=Alarm_Pour1[0]; //�������������Ϊ���Ƿ֣���һ�ν�����ʱ�������Ƿֵ�����
															 break;
											
											
											
											//LAMP��12hourģʽ��ѡ��PUMP1��������(ģʽ2)
											 case 2:  Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ
										            if(Alarm_Pour1[0]>=24)
																  Alarm_Pour1[0]-=24;
															 
															 Alarm_Pour1[1]=0;               //�������ķֺ���ȫ����Ϊ0�����ڿ��Ƿ�
															 
										           Alarm_Pour1[3]= Alarm_Pour1[1]+15;  //Alarm_Pour1[3]= Alarm_Pour1[1]+15; ��һ�ν���ʱ���

															 Alarm_Pour1[2]=Alarm_Pour1[0];         //�������������Ϊ���Ƿ֣���һ�ν�����ʱ�������Ƿֵ�����
															 
															 
															 Alarm_Pour1[4]=Alarm_Pour1[2]+6; //�ڶ��ο�ʼʱ��ʱ
															 if(Alarm_Pour1[4]>=24)
																  Alarm_Pour1[4]-=24;
															 Alarm_Pour1[5]=0;                 //�ڶ��ο�ʼ��ʱ���ȫ��Ϊ0
															 
															  Alarm_Pour1[7]=Alarm_Pour1[5]+15;   //Alarm_Pour1[7]=Alarm_Pour1[5]+15; �ڶ��ν���ʱ���

															 Alarm_Pour1[6]=Alarm_Pour1[4];//�ڶ��ν���ʱ
															  break;
															 
										//LAMP��12hourģʽ��ѡ��PUMP1��������(ģʽ3)					 
										case 3:    Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ
										           if(Alarm_Pour1[0]>=24)
																Alarm_Pour1[0]-=24;

															  Alarm_Pour1[1]=0;               //��һ�ο�ʼ��ʱ���
															 
										           Alarm_Pour1[3]=Alarm_Pour1[1]+15;   //Alarm_Pour1[3]=Alarm_Pour1[1]+15; ��һ�ν���ʱ���

															 Alarm_Pour1[2]=Alarm_Pour1[0];       //��һ�ν���ʱ��ʱ,�ֲ��ٿ���
															 
															 Alarm_Pour1[4]=Alarm_Pour1[2]+4;     //�ڶ��ο�ʼʱ��ʱ
															 if(Alarm_Pour1[4]>=24)
																  Alarm_Pour1[4]-=24;
															 Alarm_Pour1[5]=0;                     //�ڶ��ο�ʼʱ���,�����Ƿ�
															 
															 
															  Alarm_Pour1[7]=Alarm_Pour1[5]+15;         //Alarm_Pour1[7]=Alarm_Pour1[5]+15; �ڶ��ν���ʱ���
											           Alarm_Pour1[6]=Alarm_Pour1[4];       //�ڶ��ν���ʱ��ʱ,�����Ƿ�

															 
															 Alarm_Pour1[8]=Alarm_Pour1[6]+4;  //�����ο�ʼʱ��ʱ
															 if(Alarm_Pour1[8]>=24)
																   Alarm_Pour1[8]-=24;
															 Alarm_Pour1[9]=0;                  //�����ο�ʼʱ���,�����Ƿ�
															 
															 
															 Alarm_Pour1[11]= Alarm_Pour1[9]+15; //Alarm_Pour1[11]= Alarm_Pour1[9]+15; �����ν���ʱ���
															 
															 Alarm_Pour1[10]=Alarm_Pour1[8]; //�����ν���ʱ��ʱ�������Ƿ�
															 break;
									 };
							 
                   break;
									 
					case 2:
						       switch(flag4)
									{
										//Lamp��18Сʱ����ģʽ�±ÿ�һ��
										case 1:   Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ
										          if(Alarm_Pour1[0]>=24)
																 Alarm_Pour1[0]-=24;

															Alarm_Pour1[1]=0;               //��һ�ο�ʼ��ʱ��ֲ����Ƿ֣���ȡ0
															 
										          Alarm_Pour1[3]=Alarm_Pour1[1]+15;   // Alarm_Pour1[3]=Alarm_Pour1[1]+15; ��һ�ν���ʱ���
										          Alarm_Pour1[2]=Alarm_Pour1[0]; //��һ�ν���ʱ��ʱ�������Ƿ�
										          break;
										 
										
										
										//Lamp��18Сʱ����ģʽ�±ÿ�����
											case 2:  Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ
										           if(Alarm_Pour1[0]>=24)
																Alarm_Pour1[0]-=24;

															  Alarm_Pour1[1]=0;      //��һ�ο�ʼ��ʱ��֣������Ƿ�
															  
										           Alarm_Pour1[3]=Alarm_Pour1[1]+15;   //Alarm_Pour1[3]=Alarm_Pour1[1]+15; ��һ�ν���ʱ���

															 Alarm_Pour1[2]=Alarm_Pour1[0]; //��һ�ν���ʱ��ʱ�������Ƿ�
															 
															 
															 Alarm_Pour1[4]=Alarm_Pour1[2]+9; //�ڶ��ο�ʼʱ��ʱ
															 if(Alarm_Pour1[4]>=24)
																  Alarm_Pour1[4]-=24;

															 Alarm_Pour1[5]=0;                //�ڶ��ο�ʼʱ���,�����ǿ�ʼʱ��ķ���
															 
															  Alarm_Pour1[7]=Alarm_Pour1[5]+15;   //Alarm_Pour1[7]=Alarm_Pour1[5]+15;  �ڶ��ν���ʱ���

															 Alarm_Pour1[6]=Alarm_Pour1[4];//�ڶ��ν���ʱ��ʱ�������Ƿֵ�
															  break;
															 
										//Lamp��18Сʱ����ģʽ�±ÿ�����					 
										case 3:    Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ
										           if(Alarm_Pour1[0]>=24)
																Alarm_Pour1[0]-=24;

															 Alarm_Pour1[1]=0;               //��һ�ο�ʼ��ʱ���
															 
										           Alarm_Pour1[3]=Alarm_Pour1[1]+15;   //Alarm_Pour1[3]=Alarm_Pour1[1]+15; ��һ�ν���ʱ���
															 Alarm_Pour1[2]=Alarm_Pour1[0] ;      //��һ�ν���ʱ��ʱ�������Ƿ�
															 
															 
															 Alarm_Pour1[4]=Alarm_Pour1[2]+6; //�ڶ��ο�ʼʱ��ʱ
															 if(Alarm_Pour1[4]>=24)
																Alarm_Pour1[4]-=24;
															 
																 Alarm_Pour1[5]=0;   //�ڶ��ο�ʼʱ��֣������Ƿ�
															 
															  Alarm_Pour1[7]=Alarm_Pour1[5]+15;   //Alarm_Pour1[7]=Alarm_Pour1[5]+15;�ڶ��ν���ʱ���

															 Alarm_Pour1[6]=Alarm_Pour1[4]; //�ڶ��ν���ʱ��ʱ
															 
															 Alarm_Pour1[8]=Alarm_Pour1[6]+6;    //�����ο�ʼʱ��ʱ
															 if(Alarm_Pour1[8]>=24)
																  Alarm_Pour1[8]-=24;

															  Alarm_Pour1[9]=0;
															 
															 
															 Alarm_Pour1[11]= Alarm_Pour1[9]+15; //Alarm_Pour1[11]= Alarm_Pour1[9]+15;�����ν���ʱ���

															 Alarm_Pour1[10]=Alarm_Pour1[8];            //�����ν���ʱ��ʱ,�����Ƿ�
															 
															 break;
															 

									};
							
							   break;
                      						
					case 3:  switch(flag4)
									{
										 //Lamp������24Сʱģʽ�±ÿ�һ��
										 case 1:  Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ?
										           if(Alarm_Pour1[0]>=24)
																Alarm_Pour1[0]-=24;

															  Alarm_Pour1[1]=0;      //��һ�ο�ʼ��ʱ��֣������Ƿ�
															 
										           Alarm_Pour1[3]=Alarm_Pour1[1]+15;   //Alarm_Pour1[3]=Alarm_Pour1[1]+15; ��һ�ν���ʱ���

															 Alarm_Pour1[2]=Alarm_Pour1[0] ;//��һ�ν���ʱ��ʱ
															 break;
										
										 //Lamp������24Сʱģʽ�±ÿ�����
											case 2:  Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ
										           if(Alarm_Pour1[0]>=24)
																Alarm_Pour1[0]-=24;

															 Alarm_Pour1[1]=0;      //��һ�ο�ʼ��ʱ���
															 
															 
										           Alarm_Pour1[3]=Alarm_Pour1[1]+15;   //Alarm_Pour1[3]=Alarm_Pour1[1]+15; ��һ�ν���ʱ���

															  Alarm_Pour1[2]=Alarm_Pour1[0] ;       //��һ�ν���ʱ��ʱ�������Ƿ�
															 
															 Alarm_Pour1[4]=Alarm_Pour1[2]+12; //�ڶ��ο�ʼʱ��ʱ
															 if(Alarm_Pour1[4]>=24)
																 Alarm_Pour1[4]-=24;
															 Alarm_Pour1[5]=0;                //�ڶ��ο�ʼʱ��֣������Ƿ���
															 
															 Alarm_Pour1[7]=Alarm_Pour1[5]+15;   //Alarm_Pour1[7]=Alarm_Pour1[5]+15;  �ڶ��ν���ʱ���
															 Alarm_Pour1[6]=Alarm_Pour1[4];       //�ڶ��ν���ʱ��ʱ�������Ƿ�
															  break;
															 
															 
										case 3:    Alarm_Pour1[0]=Alarm[0]+1;     //��һ�ο�ʼ����ʱ��ʱ?
										           if(Alarm_Pour1[0]>=24)
																Alarm_Pour1[0]-=24;
															 Alarm_Pour1[1]=0;               //��һ�ο�ʼ��ʱ��֣������Ƿ���
															 
										           Alarm_Pour1[3]=Alarm_Pour1[1]+15;   //Alarm_Pour1[3]=Alarm_Pour1[1]+15; ��һ�ν���ʱ���
															 Alarm_Pour1[2]=Alarm_Pour1[0] ;        //��һ�ν���ʱ��ʱ�������Ƿ�
															 
															   
															 Alarm_Pour1[4]=Alarm_Pour1[0]+8; //�ڶ��ο�ʼʱ��ʱ
															 if(Alarm_Pour1[4]>=24)
																   Alarm_Pour1[4]-=24;
															  Alarm_Pour1[5]=0;  //�ڶ��ο�ʼʱ��֣������Ƿ�
															 
															  Alarm_Pour1[7]=Alarm_Pour1[5]+15;   // Alarm_Pour1[7]=Alarm_Pour1[5]+15; �ڶ��ν���ʱ���
															 Alarm_Pour1[6]=Alarm_Pour1[4];        //�ڶ��ν���ʱ��ʱ�������Ƿ�
															 
															 
															 Alarm_Pour1[8]=Alarm_Pour1[6]+8;  //�����ο�ʼʱ��ʱ
															 if(Alarm_Pour1[8]>=24)
																 Alarm_Pour1[8]-=24;
															 Alarm_Pour1[9]=0;   //�����ο�ʼʱ���
															 
															 Alarm_Pour1[11]= Alarm_Pour1[9]+15; //Alarm_Pour1[11]= Alarm_Pour1[9]+15;  �����ν���ʱ���

															 Alarm_Pour1[10]=Alarm_Pour1[8]; //�����ν���ʱ��ʱ��������
															 
															 break;
															 

									};
									break;
				}
				
				      
               
}
						       
					
