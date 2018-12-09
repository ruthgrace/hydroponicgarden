#include "AM2320.h"
#include <reg52.h>

sbit Sensor_SDA = P2^7;

unsigned char Sensor_Data[5]={0x00,0x00,0x00,0x00,0x00};
unsigned char Sensor_Check;		  //校验和

unsigned char Sensor_AnswerFlag;  //收到起始标志位
unsigned char Sensor_ErrorFlag;   //读取传感器错误标志
unsigned int  Sys_CNT;
unsigned int  Tmp,Tmp1;
unsigned int  Tmp2;
unsigned char *String;

//字符串定义
#define S_Temp "Temp:"
#define S_RH   "RH:"
#define S_CRCT "Check: True"
#define S_CRCF "Check: Wrong"
#define S_Data "Data: "
#define S_NotS "Sensor Not Connected"
unsigned char a[4];
unsigned char b[4];


/********************************************\
|* 功能： 延时	晶振为12M时				    *|
|*  t = 1 为 20us  然后成倍增加10us左右		*|
\********************************************/
void Delay_N10us(unsigned char t)
{
    while(t--)
   {
   	 _nop_();
   }
}
/********************************************\
|* 功能： 延时	晶振为12M时					*|
|* 延时大约 1ms			    				*|
\********************************************/ 
void Delay_N1ms(unsigned int t)
{
  unsigned int i;
  unsigned int j;
  for(j=t;j>0;j--)
     for(i=124;i>0;i--);  //延时大约 1ms
}

void Clear_Data (void)
{
	int i;
	for(i=0;i<5;i++)
	{
	   Sensor_Data[i] = 0x00;
	}//接收数据清零
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
|* 功能： 串口发送传感器数据函数 			*|
\********************************************/
void UARTSend_Nbyte(void)
  {
	   if(Sensor_AnswerFlag == 1)
	   {
	      Sensor_Check = Sensor_Data[0]+Sensor_Data[1]+Sensor_Data[2]+Sensor_Data[3];
		  //校验成功
		  if(Sensor_Check ==Sensor_Data[4])	
		  {
		     String = S_RH;//"RH:";	   
	         Tmp = Sensor_Data[0]*256+Sensor_Data[1];	      
	       	 Tmp1 = Sensor_Data[2]*256+Sensor_Data[3];//将温度转换为十进制并扩大10倍
				   Tmp2 = Tmp1*9/5+320;              //将摄氏温度转化为华氏温度
	         UART_PutStringAndNum(Tmp,Tmp2);  //℉=Tmp1*9/5+32;

		  }
			else	//校验失败 送上读到数据
		  {

				  Clear_Data ();
		  }
		}// 传感器未连接

}  


/********************************************\
|* 功能： 读传感器发送的单个字节	        *|
\********************************************/
unsigned char Read_SensorData(void)
  {
	unsigned char i,cnt;
	unsigned char buffer,tmp;
	buffer = 0;
	for(i=0;i<8;i++)
	{
		cnt=0;
		while(!Sensor_SDA)	//检测上次低电平是否结束
		{
		  if(++cnt >= 300)
		   {
			  break;
		   }
		}
		//延时Min=26us Max50us 跳过数据"0" 的高电平
		Delay_N10us(2);	 //延时30us   
		
		//判断传感器发送数据位
		tmp =0;
		if(Sensor_SDA)	 
		{
		  tmp = 1;
		}  
		cnt =0;
		while(Sensor_SDA)		//等待高电平 结束
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
|* 功能： 读传感器              	        *|
\********************************************/
unsigned char Read_Sensor(void)
  {
	unsigned char i;
	//主机拉低(Min=800US Max=20Ms) 
    Sensor_SDA = 0;
	Delay_N1ms(2);  //延时2Ms
	  
	//释放总线 延时(Min=30us Max=50us)
	Sensor_SDA = 1; 	
	Delay_N10us(1);//延时30us
	//主机设为输入 判断传感器响应信号 
	Sensor_SDA = 1; 
     	  
	Sensor_AnswerFlag = 0;  // 传感器响应标志	 

	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	if(Sensor_SDA ==0)
	{
	   Sensor_AnswerFlag = 1;//收到起始信号
	   Sys_CNT = 0;
	   //判断从机是否发出 80us 的低电平响应信号是否结束	 
	   while((!Sensor_SDA))
	   {
	     if(++Sys_CNT>300) //防止进入死循环
		 {
		   Sensor_ErrorFlag = 1;
		   return 0;
		  } 
	    }
	    Sys_CNT = 0;
	    //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	    while((Sensor_SDA))
	    {
	       if(++Sys_CNT>300) //防止进入死循环
		   {
		     Sensor_ErrorFlag = 1;
		     return 0;
		   } 
	    } 		 
	    // 数据接收	传感器共发送40位数据 
	    // 即5个字节 高位先送  5个字节分别为湿度高位 湿度低位 温度高位 温度低位 校验和
	    // 校验和为：湿度高位+湿度低位+温度高位+温度低位
	    for(i=0;i<5;i++)
	    {
	      Sensor_Data[i] = Read_SensorData();
	    }
	  }
	  else
	  {
	    Sensor_AnswerFlag = 0;	  // 未收到传感器响应	
	  }
	  return 1;
  }    

void AM2320_Init()
{
		Clear_Data();       // 清除收到数据
		Read_Sensor();  
    UARTSend_Nbyte();	  // 读取传感器数据
}