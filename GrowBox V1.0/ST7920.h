#ifndef __ST7920_H__
#define __ST7920_H__

#include <reg52.h>
#include<intrins.h>

sbit RS = P1^0;
sbit RW = P1^1;
sbit E  = P1^2;

#define DataPort P0        //MCU P0<------> LCM

void Init_ST7920(void);
void CGRAM(unsigned char x);
void DisplayCGRAM(unsigned char x,unsigned char y);
void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s);
void ClrScreen(void);
void LCD_PutGraphic(unsigned char code *img);
void SetGraphicMode();
void SetNormalMode();
void huatu(unsigned char x,unsigned char code *img);
//void write1632GDRAM(unsigned char x,unsigned char y,unsigned char *bmp);
void LCD_Display_Array( unsigned char x,unsigned char y,unsigned char *Array,unsigned char Lenth );
//void setrowbkcolor(unsigned char row,unsigned char bkcor);
void Write_Data(unsigned char Data);
void Write_Cmd(unsigned char Cmd);


#endif