#ifndef __OLED_H
#define __OLED_H
#include "sys.h"
#include "stdlib.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//OLED ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


//OLEDģʽ����
//0: 4�ߴ���ģʽ  ��ģ���BS1��BS2����GND��
//1: ����8080ģʽ ��ģ���BS1��BS2����VCC��
#define OLED_MODE 	0

//-----------------OLED�˿ڶ���----------------
#define OLED_CS 	PBout(7)
#define OLED_RST  	PGout(15)
#define OLED_RS 	PDout(6)
#define OLED_WR 	PAout(4)
#define OLED_RD 	PDout(7)

//ʹ��4�ߴ��нӿ�ʱʹ��
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB9����ģʽ
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB9���ģʽ
#define OLED_SCLK 	PBout(6)
#define OLED_SDIN 	PBout(7)

#define READ2_SDA   PBin(7)

#define OLED_CMD  	0		//д����
#define OLED_DATA 	1		//д����
//OLED�����ú���
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
void OLED_I2C_SendByte(uint8_t Byte);
void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t Data);
void OLED_Refresh_Gram(void);

void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
u8 OLED_Wait_Ack(void);
void OLED_NAck(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void oled_show_chinese(int x0,int y0,int no);
#endif




