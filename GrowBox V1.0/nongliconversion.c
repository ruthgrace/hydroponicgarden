#include"NONGLICONVERSION.H"
#include <intrins.h>
/****************************************************/
/* 获取当前日期的农历及星期*/
/****************************************************/
/*==================================================*/
/*公历年对应的农历数据,每年三字节, 格式: */
/*第一字节BIT7-4 位表示闰月月份,值为0 为无闰月, */
/*BIT3-0 对应农历第1-4 月的大小*/
/*第二字节BIT7-0 对应农历第5-12 月大小, */
/*第三字节BIT7 表示农历第13 个月大小月份对应的位*/
/*为1 表示本农历月大(30 天),为0 表示小(29 天) */
/*第三字节BIT6-5 表示春节的公历月份,BIT4-0 表示春节的公历日期*/
/*类型:Byte:unsigned char Word:unsigned int */
/*本代码中Tim[0]/Tim[1]/Tim[2]分别为当前年月日数据*/
/*其中Tim[0]Bit7 表示世纪,1 为20 世纪19xx,0 为21 世纪20xx*/
/*Tim[10]为周数据/Tim[7]/Tim[8]/Tim[9]为农历数据,根据需要自行更换*/
/*==================================================*/
/*---------------------------------公历转换阴历年份数据表--------------------------------*/
static unsigned char code year_code[597]={
0x04,0xAe,0x53,0x0A,0x57,0x48,0x55,0x26,0xBd, // 1901-1903
0x0d,0x26,0x50,0x0d,0x95,0x44,0x46,0xAA,0xB9, // 1904-1906
0x05,0x6A,0x4d,0x09,0xAd,0x42,0x24,0xAe,0xB6, // 1907-1909
0x04,0xAe,0x4A,0x6A,0x4d,0xBe,0x0A,0x4d,0x52, // 1910-1912
0x0d,0x25,0x46,0x5d,0x52,0xBA,0x0B,0x54,0x4e, // 1913-1915
0x0d,0x6A,0x43,0x29,0x6d,0x37,0x09,0x5B,0x4B, // 1916-1918
0x74,0x9B,0xC1,0x04,0x97,0x54,0x0A,0x4B,0x48, // 1919-1921
0x5B,0x25,0xBC,0x06,0xA5,0x50,0x06,0xd4,0x45, // 1922-1924
0x4A,0xdA,0xB8,0x02,0xB6,0x4d,0x09,0x57,0x42, // 1925-1927
0x24,0x97,0xB7,0x04,0x97,0x4A,0x66,0x4B,0x3e, // 1928-1930
0x0d,0x4A,0x51,0x0e,0xA5,0x46,0x56,0xd4,0xBA, // 1931-1933
0x05,0xAd,0x4e,0x02,0xB6,0x44,0x39,0x37,0x38, // 1934-1936
0x09,0x2e,0x4B,0x7C,0x96,0xBf,0x0C,0x95,0x53, // 1937-1939
0x0d,0x4A,0x48,0x6d,0xA5,0x3B,0x0B,0x55,0x4f, // 1940-1942
0x05,0x6A,0x45,0x4A,0xAd,0xB9,0x02,0x5d,0x4d, // 1943-1945
0x09,0x2d,0x42,0x2C,0x95,0xB6,0x0A,0x95,0x4A, // 1946-1948
0x7B,0x4A,0xBd,0x06,0xCA,0x51,0x0B,0x55,0x46, // 1949-1951
0x55,0x5A,0xBB,0x04,0xdA,0x4e,0x0A,0x5B,0x43, // 1952-1954
0x35,0x2B,0xB8,0x05,0x2B,0x4C,0x8A,0x95,0x3f, // 1955-1957
0x0e,0x95,0x52,0x06,0xAA,0x48,0x7A,0xd5,0x3C, // 1958-1960
0x0A,0xB5,0x4f,0x04,0xB6,0x45,0x4A,0x57,0x39, // 1961-1963
0x0A,0x57,0x4d,0x05,0x26,0x42,0x3e,0x93,0x35, // 1964-1966
0x0d,0x95,0x49,0x75,0xAA,0xBe,0x05,0x6A,0x51, // 1967-1969
0x09,0x6d,0x46,0x54,0xAe,0xBB,0x04,0xAd,0x4f, // 1970-1972
0x0A,0x4d,0x43,0x4d,0x26,0xB7,0x0d,0x25,0x4B, // 1973-1975
0x8d,0x52,0xBf,0x0B,0x54,0x52,0x0B,0x6A,0x47, // 1976-1978
0x69,0x6d,0x3C,0x09,0x5B,0x50,0x04,0x9B,0x45, // 1979-1981
0x4A,0x4B,0xB9,0x0A,0x4B,0x4d,0xAB,0x25,0xC2, // 1982-1984
0x06,0xA5,0x54,0x06,0xd4,0x49,0x6A,0xdA,0x3d, // 1985-1987
0x0A,0xB6,0x51,0x09,0x37,0x46,0x54,0x97,0xBB, // 1988-1990
0x04,0x97,0x4f,0x06,0x4B,0x44,0x36,0xA5,0x37, // 1991-1993
0x0e,0xA5,0x4A,0x86,0xB2,0xBf,0x05,0xAC,0x53, // 1994-1996
0x0A,0xB6,0x47,0x59,0x36,0xBC,0x09,0x2e,0x50, // 1997-1999
0x0C,0x96,0x45,0x4d,0x4A,0xB8,0x0d,0x4A,0x4C, // 2000-2002
0x0d,0xA5,0x41,0x25,0xAA,0xB6,0x05,0x6A,0x49, // 2003-2005
0x7A,0xAd,0xBd,0x02,0x5d,0x52,0x09,0x2d,0x47, // 2006-2008
0x5C,0x95,0xBA,0x0A,0x95,0x4e,0x0B,0x4A,0x43, // 2009-2011
0x4B,0x55,0x37,0x0A,0xd5,0x4A,0x95,0x5A,0xBf, // 2012-2014
0x04,0xBA,0x53,0x0A,0x5B,0x48,0x65,0x2B,0xBC, // 2015-2017
0x05,0x2B,0x50,0x0A,0x93,0x45,0x47,0x4A,0xB9, // 2018-2020
0x06,0xAA,0x4C,0x0A,0xd5,0x41,0x24,0xdA,0xB6, // 2021-2023
0x04,0xB6,0x4A,0x69,0x57,0x3d,0x0A,0x4e,0x51, // 2024-2026
0x0d,0x26,0x46,0x5e,0x93,0x3A,0x0d,0x53,0x4d, // 2027-2029
0x05,0xAA,0x43,0x36,0xB5,0x37,0x09,0x6d,0x4B, // 2030-2032
0xB4,0xAe,0xBf,0x04,0xAd,0x53,0x0A,0x4d,0x48, // 2033-2035
0x6d,0x25,0xBC,0x0d,0x25,0x4f,0x0d,0x52,0x44, // 2036-2038
0x5d,0xAA,0x38,0x0B,0x5A,0x4C,0x05,0x6d,0x41, // 2039-2041
0x24,0xAd,0xB6,0x04,0x9B,0x4A,0x7A,0x4B,0xBe, // 2042-2044
0x0A,0x4B,0x51,0x0A,0xA5,0x46,0x5B,0x52,0xBA, // 2045-2047
0x06,0xd2,0x4e,0x0A,0xdA,0x42,0x35,0x5B,0x37, // 2048-2050
0x09,0x37,0x4B,0x84,0x97,0xC1,0x04,0x97,0x53, // 2051-2053
0x06,0x4B,0x48,0x66,0xA5,0x3C,0x0e,0xA5,0x4f, // 2054-2056
0x06,0xB2,0x44,0x4A,0xB6,0x38,0x0A,0xAe,0x4C, // 2057-2059
0x09,0x2e,0x42,0x3C,0x97,0x35,0x0C,0x96,0x49, // 2060-2062
0x7d,0x4A,0xBd,0x0d,0x4A,0x51,0x0d,0xA5,0x45, // 2063-2065
0x55,0xAA,0xBA,0x05,0x6A,0x4e,0x0A,0x6d,0x43, // 2066-2068
0x45,0x2e,0xB7,0x05,0x2d,0x4B,0x8A,0x95,0xBf, // 2069-2071
0x0A,0x95,0x53,0x0B,0x4A,0x47,0x6B,0x55,0x3B, // 2072-2074
0x0A,0xd5,0x4f,0x05,0x5A,0x45,0x4A,0x5d,0x38, // 2075-2077
0x0A,0x5B,0x4C,0x05,0x2B,0x42,0x3A,0x93,0xB6, // 2078-2080
0x06,0x93,0x49,0x77,0x29,0xBd,0x06,0xAA,0x51, // 2081-2083
0x0A,0xd5,0x46,0x54,0xdA,0xBA,0x04,0xB6,0x4e, // 2084-2086
0x0A,0x57,0x43,0x45,0x27,0x38,0x0d,0x26,0x4A, // 2087-2089
0x8e,0x93,0x3e,0x0d,0x52,0x52,0x0d,0xAA,0x47, // 2090-2092
0x66,0xB5,0x3B,0x05,0x6d,0x4f,0x04,0xAe,0x45, // 2093-2095
0x4A,0x4e,0xB9,0x0A,0x4d,0x4C,0x0d,0x15,0x41, // 2096-2098
0x2d,0x92,0xB5, // 2099
};
//月份天数数据表
unsigned char code day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
unsigned char code day_code2[3]={0x111,0x130,0x14e};
//月修正数据表
unsigned char code table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
unsigned char c_moon,year_moon,month_moon,day_moon;
unsigned char GetMoonDay(unsigned char month_p,unsigned short table_addr)
/*读取数据表中农历月的大月或小月,如果该月大返回1,小返回0*/
{
	switch (month_p)
	{
		case 1:
			if((year_code[table_addr]&0x08)==0)		return(0);
			else 									return(1); 
		case 2:
			if((year_code[table_addr]&0x04)==0)		return(0);
			else 									return(1);
		case 3:
			if((year_code[table_addr]&0x02)==0)		return(0);
			else 									return(1);
		case 4:
			if((year_code[table_addr]&0x01)==0)		return(0);
			else 								    return(1);
		case 5:
			if((year_code[table_addr+1]&0x80)==0)	return(0);
			else 									return(1);
		case 6:
			if((year_code[table_addr+1]&0x40)==0)	return(0);
			else 									return(1);
		case 7:
			if((year_code[table_addr+1]&0x20)==0)	return(0);
			else 									return(1);
		case 8:
			if((year_code[table_addr+1]&0x10)==0)	return(0);
			else 									return(1);
		case 9:
			if((year_code[table_addr+1]&0x08)==0)	return(0);
			else 									return(1);
		case 10:
			if((year_code[table_addr+1]&0x04)==0)	return(0);
			else 									return(1);
		case 11:
			if((year_code[table_addr+1]&0x02)==0)	return(0);
			else 									return(1);
		case 12:
			if((year_code[table_addr+1]&0x01)==0)	return(0);
			else 									return(1);
		case 13:
			if((year_code[table_addr+2]&0x80)==0)	return(0);
			else 									return(1);
	}
	return(0);
}
/*--------------------------------公历转农历函数-----------------------------------------*/

void Conversion(unsigned char year,unsigned char month,unsigned char day)
{                         
	unsigned char temp1,temp2,temp3,month_p;
	unsigned int temp4,table_addr;
	bit flag2,flag_y,c;	
  	table_addr=(year+0x64-1)*0x3;  //定位数据表地址
	temp1=year_code[table_addr+2]&0x60; //取当年春节所在的公历月份
	temp1=_cror_(temp1,5);
	temp2=year_code[table_addr+2]&0x1f; //取当年春节所在的公历日
	//取当年春节所在的公历日完成 
	//计算当年春年离当年元旦的天数,春节只会在公历1月或2月 
	if(temp1==0x1) 	{ temp3=temp2-1; }  
   	else  		{ temp3=temp2+0x1f-1; }

	// 计算当年春年离当年元旦的天数完成 
	//计算公历日离当年元旦的天数,为了减少运算,用了两个表 
	//day_code1[9],day_code2[3] 
	//如果公历月在九月或前,天数会少于0xff,用表day_code1[9], 
	//在九月后,天数大于0xff,用表day_code2[3] 
	//如输入公历日为8月10日,则公历日离元旦天数为day_code1[8-1]+10-1 
	//如输入公历日为11月10日,则公历日离元旦天数为day_code2[11-10]+10-1 
	if(month<10)	{ temp4=day_code1[month-1]+day-1; }
		else     	{ temp4=day_code2[month-10]+day-1;}

//	if ((month>0x2)&&(year%0x4==0)&&(year!=0)) //此语句造成2000年3月1日—12月31日之间
												//计算农历少算一天，应删除判断语句“&&(year!=0)”
	if ((month>0x2)&&(year%0x4==0))
		temp4+=1; //如果公历月大于2月并且该年的2月为闰月,天数加1
	//计算公历日离当年元旦的天数完成 
	 
	//判断公历日在春节前还是春节后
	if (temp4>=temp3)
	{ 
		temp4-=temp3; month=0x1;
		month_p=0x1;  //month_p为月份指向,公历日在春节前或就是春节当日month_p指向首月
		flag2=GetMoonDay(month_p,table_addr); //检查该农历月为大小还是小月,大月返回1,小月返回0
		flag_y=0;
      if(flag2==0)temp1=0x1d;   //小月29天
      	else    temp1=0x1e;    //大月30天
		temp2=year_code[table_addr]&0xf0;
		temp2=_cror_(temp2,4);  //从数据表中取该年的闰月月份,如为0则该年无闰月
		while(temp4>=temp1)
		{
			temp4-=temp1; month_p+=1;
			if(month==temp2)
	    	{
				flag_y=~flag_y;
         	if(flag_y==0)month+=1;
			}
			else month+=1;
			flag2=GetMoonDay(month_p,table_addr);
         if(flag2==0)temp1=0x1d;
         	else    temp1=0x1e;
		}
		day=temp4+1;
	}
	else
	{  
		temp3-=temp4;
		if (year==0x0){year=0x63;c=1;}
			else	year-=1;
		table_addr-=0x3;
		month=0xc;
		temp2=year_code[table_addr]&0xf0;
		temp2=_cror_(temp2,4);
		if (temp2==0)month_p=0xc; 
			else	month_p=0xd; 
		flag_y=0;
		flag2=GetMoonDay(month_p,table_addr);
		if(flag2==0)temp1=0x1d;
      	else	temp1=0x1e;
		while(temp3>temp1)
		{
			temp3-=temp1;
			month_p-=1;
			if(flag_y==0)month-=1;
         if(month==temp2)flag_y=~flag_y;
         flag2=GetMoonDay(month_p,table_addr);
         if(flag2==0)temp1=0x1d;
            else	temp1=0x1e;
		}
		day=temp1-temp3+1;
	}
	
		c_moon=c;
		year_moon=year;
		month_moon=month;
		day_moon=day;

}
/**************************************************************************** 
	算法:日期+年份+所过闰年数+月较正数之和除7 的余数就是星期但如果是在 
	闰年又不到3月份上述之和要减一天再除7 
	星期日数为0 
*****************************************************************************/ 
unsigned char Conver_week(unsigned char cyear,unsigned char cmonth,unsigned char cday)
{
	unsigned char temp,week;
	cyear=100+cyear;
	temp=cyear+(cyear/4); //如果为21世纪,年份数加100
	temp=temp%7;    //为节省资源,先进行一次取余,避免数大于0xff,避免使用整型数据
	temp=temp+cday+table_week[cmonth-1]; //日期+年份+所过闰年数+月较正数之和除7
	if(cyear%4==0&&cmonth<3)
		{temp=temp-1;}//如果是在闰年又不到3月份上述之和要减一天再除7 
	week=temp%7;
	return week;
}