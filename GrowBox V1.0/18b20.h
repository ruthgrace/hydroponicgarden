#ifndef __18B20_H__
#define __18B20_H__

#include<reg52.h>     //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    端口定义
------------------------------------------------*/
sbit DQ=P3^7;//ds18b20 端口
/*------------------------------------------------
                  函数声明
------------------------------------------------*/
unsigned int ReadTemperature(void);
bit Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);
void tempchang();
void tempdat();



#endif
