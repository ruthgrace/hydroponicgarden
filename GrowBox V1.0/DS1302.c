
#include "ds1302.h"

unsigned char time_buf1[8] = {20,9,3,13,18,51,00,6};//空年月日时分秒周
unsigned char time_buf[8] ;                         //空年月日时分秒周
/*------------------------------------------------
           向DS1302写入一字节数据
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	unsigned char i;
	RST_SET;	
	
	//写入目标地址：addr
	addr = addr & 0xFE;     //最低位置零
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
	
	//写入数据：d
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
	RST_CLR;					//停止DS1302总线
}
/*------------------------------------------------
           从DS1302读出一字节数据
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char i;
	unsigned char temp;
	RST_SET;	

	//写入目标地址：addr
	addr = addr | 0x01;//最低位置高
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
	
	//输出数据：temp
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
	
	RST_CLR;	//停止DS1302总线
	return temp;
}

/*------------------------------------------------
           向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(void) 
{
     
    unsigned char i,tmp;
	  for(i=0;i<8;i++)
	  {                  //BCD处理
			tmp=time_buf1[i]/10;
			time_buf[i]=time_buf1[i]%10;
			time_buf[i]=time_buf[i]+tmp*16;
	  }
	Ds1302_Write_Byte(ds1302_control_add,0x00);			  //关闭写保护 
	Ds1302_Write_Byte(ds1302_sec_add,0x80);				    //暂停 
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//涓流充电 
	Ds1302_Write_Byte(ds1302_year_add,time_buf[1]);		//年 
	Ds1302_Write_Byte(ds1302_month_add,time_buf[2]);	//月 
	Ds1302_Write_Byte(ds1302_date_add,time_buf[3]);		//日 
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//周 
	Ds1302_Write_Byte(ds1302_hr_add,time_buf[4]);		  //时 
	Ds1302_Write_Byte(ds1302_min_add,time_buf[5]);		//分
	Ds1302_Write_Byte(ds1302_sec_add,time_buf[6]);		//秒
	Ds1302_Write_Byte(ds1302_day_add,time_buf[7]);		//周 
	Ds1302_Write_Byte(ds1302_control_add,0x80);			  //打开写保护 
}

/*------------------------------------------------
           从DS1302读出时钟数据
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
  unsigned char i,tmp;
	
	time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		  //年 
	time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		  //月 
	time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		  //日 
	time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		    //时 
	time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		    //分 
	time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;//秒 
	time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		    //周 


	for(i=0;i<8;i++)
	{   //BCD处理
			tmp=time_buf[i]/16;
			time_buf1[i]=time_buf[i]%16;
			time_buf1[i]=time_buf1[i]+tmp*10;
	}
}

/*************************计算星期几************************/
//------------------------------------------------------------------------------ 
//函数名称：Get_Day 
//函数功能：根据年月日算出星期 
//函数参数：year --- 年 
// month --- 月 
// date --- 日 
//函数返回：day --- 星期 
//------------------------------------------------------------------------------ 
//计算公式使用著名的蔡勒（Zeller）公式。即: 
//w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1 
//公式中的符号含义如下， 
//w：星期； 
//c：世纪-1； 
//y：年（两位数）； 
//m：月（m大于等于3，小于等于14，即在蔡勒公式中， 
// 某年的1、2月要看作上一年的 13、14月来计算， 
// 比如2003年1月1日要看作2002年的13月1日来计算）； 
//d：日；[ ]代表取整，即只要整数部分。 

//C是世纪数减一，y是年份后两位，M是月份，d是日数。 
//1月和2月要按上一年的13月和 14月来算，这时C和y均按上一 年取值。 
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
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void)
{
	RST_CLR;			//RST脚置低
	SCK_CLR;			//SCK脚置低
 // Ds1302_Write_Byte(ds1302_sec_add,0x00);	
 
}
