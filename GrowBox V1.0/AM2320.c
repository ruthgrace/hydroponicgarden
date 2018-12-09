#include "AM2320.h"
#include <reg52.h>

sbit Sensor_SDA = P2^7;

unsigned char Sensor_Data[5]={0x00,0x00,0x00,0x00,0x00};
unsigned char Sensor_Check;		  //У���

unsigned char Sensor_AnswerFlag;  //�յ���ʼ��־λ
unsigned char Sensor_ErrorFlag;   //��ȡ�����������־
unsigned int  Sys_CNT;
unsigned int  Tmp,Tmp1;
unsigned int  Tmp2;
unsigned char *String;

//�ַ�������
#define S_Temp "Temp:"
#define S_RH   "RH:"
#define S_CRCT "Check: True"
#define S_CRCF "Check: Wrong"
#define S_Data "Data: "
#define S_NotS "Sensor Not Connected"
unsigned char a[4];
unsigned char b[4];


/********************************************\
|* ���ܣ� ��ʱ	����Ϊ12Mʱ				    *|
|*  t = 1 Ϊ 20us  Ȼ��ɱ�����10us����		*|
\********************************************/
void Delay_N10us(unsigned char t)
{
    while(t--)
   {
   	 _nop_();
   }
}
/********************************************\
|* ���ܣ� ��ʱ	����Ϊ12Mʱ					*|
|* ��ʱ��Լ 1ms			    				*|
\********************************************/ 
void Delay_N1ms(unsigned int t)
{
  unsigned int i;
  unsigned int j;
  for(j=t;j>0;j--)
     for(i=124;i>0;i--);  //��ʱ��Լ 1ms
}

void Clear_Data (void)
{
	int i;
	for(i=0;i<5;i++)
	{
	   Sensor_Data[i] = 0x00;
	}//������������
}

void UART_PutStringAndNum(unsigned int num,unsigned int num1)
  {
//		unsigned char i;
		a[3] = '0'+num%10;
		a[2] = '.';
		a[1] = '0'+num/10%10;
		a[0] = '0'+num/100%10;
		
		b[3] = '0'+num1%10;           //Temperature
		b[2] = '.';
		b[1] = '0'+num1/10%10;
		b[0] = '0'+num1/100%10;
		

  }  

/********************************************\
|* ���ܣ� ���ڷ��ʹ��������ݺ��� 			*|
\********************************************/
void UARTSend_Nbyte(void)
  {
	   if(Sensor_AnswerFlag == 1)
	   {
	      Sensor_Check = Sensor_Data[0]+Sensor_Data[1]+Sensor_Data[2]+Sensor_Data[3];
		  //У��ɹ�
		  if(Sensor_Check ==Sensor_Data[4])	
		  {
		     String = S_RH;//"RH:";	   
	         Tmp = Sensor_Data[0]*256+Sensor_Data[1];	      
	       	 Tmp1 = Sensor_Data[2]*256+Sensor_Data[3];//���¶�ת��Ϊʮ���Ʋ�����10��
				   Tmp2 = Tmp1*9/5+320;              //�������¶�ת��Ϊ�����¶�
	         UART_PutStringAndNum(Tmp,Tmp2);  //�H=Tmp1*9/5+32;

		  }
			else	//У��ʧ�� ���϶�������
		  {

				  Clear_Data ();
		  }
		}// ������δ����

}  


/********************************************\
|* ���ܣ� �����������͵ĵ����ֽ�	        *|
\********************************************/
unsigned char Read_SensorData(void)
  {
	unsigned char i,cnt;
	unsigned char buffer,tmp;
	buffer = 0;
	for(i=0;i<8;i++)
	{
		cnt=0;
		while(!Sensor_SDA)	//����ϴε͵�ƽ�Ƿ����
		{
		  if(++cnt >= 300)
		   {
			  break;
		   }
		}
		//��ʱMin=26us Max50us ��������"0" �ĸߵ�ƽ
		Delay_N10us(2);	 //��ʱ30us   
		
		//�жϴ�������������λ
		tmp =0;
		if(Sensor_SDA)	 
		{
		  tmp = 1;
		}  
		cnt =0;
		while(Sensor_SDA)		//�ȴ��ߵ�ƽ ����
		{
		   	if(++cnt >= 200)
			{
			  break;
			}
		}
		buffer <<=1;
		buffer |= tmp;	
	}
	return buffer;
  }

/********************************************\
|* ���ܣ� ��������              	        *|
\********************************************/
unsigned char Read_Sensor(void)
  {
	unsigned char i;
	//��������(Min=800US Max=20Ms) 
    Sensor_SDA = 0;
	Delay_N1ms(2);  //��ʱ2Ms
	  
	//�ͷ����� ��ʱ(Min=30us Max=50us)
	Sensor_SDA = 1; 	
	Delay_N10us(1);//��ʱ30us
	//������Ϊ���� �жϴ�������Ӧ�ź� 
	Sensor_SDA = 1; 
     	  
	Sensor_AnswerFlag = 0;  // ��������Ӧ��־	 

	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	if(Sensor_SDA ==0)
	{
	   Sensor_AnswerFlag = 1;//�յ���ʼ�ź�
	   Sys_CNT = 0;
	   //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	   while((!Sensor_SDA))
	   {
	     if(++Sys_CNT>300) //��ֹ������ѭ��
		 {
		   Sensor_ErrorFlag = 1;
		   return 0;
		  } 
	    }
	    Sys_CNT = 0;
	    //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	    while((Sensor_SDA))
	    {
	       if(++Sys_CNT>300) //��ֹ������ѭ��
		   {
		     Sensor_ErrorFlag = 1;
		     return 0;
		   } 
	    } 		 
	    // ���ݽ���	������������40λ���� 
	    // ��5���ֽ� ��λ����  5���ֽڷֱ�Ϊʪ�ȸ�λ ʪ�ȵ�λ �¶ȸ�λ �¶ȵ�λ У���
	    // У���Ϊ��ʪ�ȸ�λ+ʪ�ȵ�λ+�¶ȸ�λ+�¶ȵ�λ
	    for(i=0;i<5;i++)
	    {
	      Sensor_Data[i] = Read_SensorData();
	    }
	  }
	  else
	  {
	    Sensor_AnswerFlag = 0;	  // δ�յ���������Ӧ	
	  }
	  return 1;
  }    

void AM2320_Init()
{
		Clear_Data();       // ����յ�����
		Read_Sensor();  
    UARTSend_Nbyte();	  // ��ȡ����������
}