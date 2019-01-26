/*-----------------------------------------------
  ���ƣ�18B20�¶ȴ�����
  ��վ��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�18B20�����¶ȼ���Ӧ����������
------------------------------------------------*/
#include"delay.h"
#include"18b20.h"

float ftemp;
unsigned char t[4];
unsigned char code table[]="0123456789.";
unsigned int tt;
float ftemp;
/*------------------------------------------------
                    18b20��ʼ��
------------------------------------------------*/
bit Init_DS18B20(void)
{
	 bit dat=0;
	 DQ = 1;    //DQ��λ
	 DelayUs2x(5);   //������ʱ
	 DQ = 0;         //��Ƭ����DQ����
	 DelayUs2x(200); //��ȷ��ʱ ���� 480us С��960us
	 DelayUs2x(200);
	 DQ = 1;        //��������
	 DelayUs2x(50); //15~60us �� ����60-240us�Ĵ�������
	 dat=DQ;        //���x=0���ʼ���ɹ�, x=1���ʼ��ʧ��
	 DelayUs2x(25); //������ʱ����
	 return dat;
}

/*------------------------------------------------
                    ��ȡһ���ֽ�
------------------------------------------------*/
unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	 {
		DQ = 0; // �������ź�
		dat>>=1;
		DQ = 1; // �������ź�
		if(DQ)
		 dat|=0x80;
		DelayUs2x(25);
	 }
	 return(dat);
}
/*------------------------------------------------
                    д��һ���ֽ�
------------------------------------------------*/
void WriteOneChar(unsigned char dat)
{
	 unsigned char i=0;
	 for (i=8; i>0; i--)
	 {
			DQ = 0;
			DQ = dat&0x01;
			DelayUs2x(25);
			DQ = 1;
			dat>>=1;
	 }
		DelayUs2x(25);
}

void tempchang() //
{
  Init_DS18B20();
  DelayMs(1);
  WriteOneChar(0xCC); // ����������кŵĲ���
	WriteOneChar(0x44);
}

/*------------------------------------------------
                    ��ȡ�¶�
------------------------------------------------*/
unsigned int ReadTemperature(void)
{
		unsigned char a=0;
		unsigned int  b=0;
	
	Init_DS18B20();
  WriteOneChar(0xCC); // ����������кŵĲ���
  WriteOneChar(0x44); // �����¶�ת��
  DelayMs(10);
  Init_DS18B20();
  WriteOneChar(0xCC); //����������кŵĲ��� 
  WriteOneChar(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
  a=ReadOneChar();   //��λ
  b=ReadOneChar();   //��λ

	  tt=b;
	
		tt<<=8;
		tt=tt|a;
	  
    ftemp=tt*0.0625;      
    tt=ftemp*10+0.5;
	  

		return(tt);
}

//�¶ȴ�������ȡ���ݵĴ���
void tempdat()
{
	 unsigned int a,b,c,d,f;
	  
	 tempchang();
   f=ReadTemperature();
		 a=(1.8*f+320);
		 b=(a/100)%10;
		 c=(a/10)%10;
		 d=a%10;

		 t[0]=table[b]; 
		 DelayUs2x(25);
		 t[1]=table[c];
		 DelayUs2x(25);
		 t[2]=table[10];
		 DelayUs2x(25);
		 t[3]=table[d];
		 DelayUs2x(25);
}
