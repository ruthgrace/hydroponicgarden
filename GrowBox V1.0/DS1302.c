
#include "ds1302.h"

unsigned char time_buf1[8] = {20,9,3,13,18,51,00,6};//��������ʱ������
unsigned char time_buf[8] ;                         //��������ʱ������
/*------------------------------------------------
           ��DS1302д��һ�ֽ�����
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	unsigned char i;
	RST_SET;	
	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;     //���λ����
	for (i = 0; i < 8; i ++) 
	{ 
		if (addr & 0x01) 
		{
			IO_SET;
	  }
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		addr = addr >> 1;
		}
	
	//д�����ݣ�d
	for (i = 0; i < 8; i ++) 
	{
		if (d & 0x01) 
		{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
			SCK_SET;
			SCK_CLR;
			d = d >> 1;
	}
	RST_CLR;					//ֹͣDS1302����
}
/*------------------------------------------------
           ��DS1302����һ�ֽ�����
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char i;
	unsigned char temp;
	RST_SET;	

	//д��Ŀ���ַ��addr
	addr = addr | 0x01;//���λ�ø�
	for (i = 0; i < 8; i ++) 
	{
	     
		if (addr & 0x01) 
		{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		addr = addr >> 1;
	}
	
	//������ݣ�temp
	for (i = 0; i < 8; i ++) 
	{
		temp = temp >> 1;
		if (IO_R) 
		{
			temp |= 0x80;
		}
		else 
		{
			temp &= 0x7F;
		}
				SCK_SET;
				SCK_CLR;
	}
	
	RST_CLR;	//ֹͣDS1302����
	return temp;
}

/*------------------------------------------------
           ��DS1302д��ʱ������
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
     
    unsigned char i,tmp;
	  for(i=0;i<8;i++)
	  {                  //BCD����
			tmp=time_buf1[i]/10;
			time_buf[i]=time_buf1[i]%10;
			time_buf[i]=time_buf[i]+tmp*16;
	  }
	Ds1302_Write_Byte(ds1302_control_add,0x00);			  //�ر�д���� 
	Ds1302_Write_Byte(ds1302_sec_add,0x80);				    //��ͣ 
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//������ 
	Ds1302_Write_Byte(ds1302_year_add,time_buf[1]);		//�� 
	Ds1302_Write_Byte(ds1302_month_add,time_buf[2]);	//�� 
	Ds1302_Write_Byte(ds1302_date_add,time_buf[3]);		//�� 
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//�� 
	Ds1302_Write_Byte(ds1302_hr_add,time_buf[4]);		  //ʱ 
	Ds1302_Write_Byte(ds1302_min_add,time_buf[5]);		//��
	Ds1302_Write_Byte(ds1302_sec_add,time_buf[6]);		//��
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//�� 
	Ds1302_Write_Byte(ds1302_control_add,0x80);			  //��д���� 
}

/*------------------------------------------------
           ��DS1302����ʱ������
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
  unsigned char i,tmp;
	
	time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		  //�� 
	time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		  //�� 
	time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		  //�� 
	time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		    //ʱ 
	time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		    //�� 
	time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//�� 
	time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		    //�� 


	for(i=0;i<8;i++)
	{   //BCD����
			tmp=time_buf[i]/16;
			time_buf1[i]=time_buf[i]%16;
			time_buf1[i]=time_buf1[i]+tmp*10;
	}
}

/*************************�������ڼ�************************/
//------------------------------------------------------------------------------ 
//�������ƣ�Get_Day 
//�������ܣ������������������ 
//����������year --- �� 
// month --- �� 
// date --- �� 
//�������أ�day --- ���� 
//------------------------------------------------------------------------------ 
//���㹫ʽʹ�������Ĳ��գ�Zeller����ʽ����: 
//w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1 
//��ʽ�еķ��ź������£� 
//w�����ڣ� 
//c������-1�� 
//y���꣨��λ������ 
//m���£�m���ڵ���3��С�ڵ���14�����ڲ��չ�ʽ�У� 
// ĳ���1��2��Ҫ������һ��� 13��14�������㣬 
// ����2003��1��1��Ҫ����2002���13��1�������㣩�� 
//d���գ�[ ]����ȡ������ֻҪ�������֡� 

//C����������һ��y����ݺ���λ��M���·ݣ�d�������� 
//1�º�2��Ҫ����һ���13�º� 14�����㣬��ʱC��y������һ ��ȡֵ�� 
//------------------------------------------------------------------------------ 
unsigned char Get_Day(unsigned char year,unsigned char month,unsigned char date) 
{ 
		unsigned char day; 
		unsigned int temp; 
		unsigned char century; 
////		year  = Hex_To_Decimal(year); 
////		month = Hex_To_Decimal(month); 
////		date  = Hex_To_Decimal(date); 

		century = 20; 
		if(month < 3) 
		{ 
			month += 12; 
			if(year == 0) 
			{ 
				year = 99; 
				century --; 
			} 
			else year --; 
		} 
			temp = (month + 1) * 26; 
			temp = temp / 10; 
			temp += year; 
			temp += (year >> 2); 
			temp += (century >> 2); 
			temp -= (century << 1); 
			temp += date; 
			temp --; 
			day = temp % 7; 
			if(day == 0) 
			  day = 7; 
			return day; 
} 

/*------------------------------------------------
                DS1302��ʼ��
------------------------------------------------*/
void Ds1302_Init(void)
{
	RST_CLR;			//RST���õ�
	SCK_CLR;			//SCK���õ�
 // Ds1302_Write_Byte(ds1302_sec_add,0x00);	
 
}
