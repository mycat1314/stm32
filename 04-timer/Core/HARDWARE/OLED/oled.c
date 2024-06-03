#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"
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


//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];

//�����Դ浽LCD
void OLED_Refresh_Gram(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WriteCommand (0xb0+i);    //����ҳ��ַ��0~7��
        OLED_WriteCommand (0x00);      //������ʾλ�á��е͵�ַ
        OLED_WriteCommand (0x10);      //������ʾλ�á��иߵ�ַ
        for(n=0;n<128;n++)OLED_WriteData(OLED_GRAM[n][i]);
    }
}
/**
  * @brief  I2C��ʼ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Start(void)
{
    SDA2_OUT();
    OLED_SDIN=1;
    OLED_SCLK=1;
    delay_us(4);
    OLED_SDIN=0;
    delay_us(4);
    OLED_SCLK=0;
}

/**
  * @brief  I2Cֹͣ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Stop(void)
{
    SDA2_OUT();
    OLED_SCLK=0;
    OLED_SDIN=0;
    delay_us(4);
    OLED_SCLK=1;
    OLED_SDIN=1;
    delay_us(4);

}

/**
  * @brief  I2C����һ���ֽ�
  * @param  Byte Ҫ���͵�һ���ֽ�
  * @retval ��
  */
void OLED_I2C_SendByte(uint8_t txd)
{
    u8 t;
    SDA2_OUT();
    OLED_SCLK=0;
    for(t=0;t<8;t++)
    {
        OLED_SDIN=(txd&0x80)>>7;
        txd<<=1;
        delay_us(2);
        OLED_SCLK=1;
        delay_us(2);
        OLED_SCLK=0;
        delay_us(2);
    }

}

u8 OLED_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA2_IN();
    OLED_SDIN=1;delay_us(1);
    OLED_SCLK=1;delay_us(1);
    while(READ2_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            OLED_I2C_Stop();
            return 1;
        }
    }
    OLED_SCLK=0;
    return 0;
}
void OLED_ACK(void)
{
    OLED_SCLK=0;
    SDA2_OUT();
    OLED_SDIN=0;
    delay_us(2);
    OLED_SCLK=1;
    delay_us(2);
    OLED_SCLK=0;
}

void OLED_NAck(void)
{
    OLED_SCLK=0;
    SDA2_OUT();
    OLED_SDIN=1;
    delay_us(2);
    OLED_SCLK=1;
    delay_us(2);
    OLED_SCLK=0;
}

/**
  * @brief  OLEDд����
  * @param  Command Ҫд�������
  * @retval ��
  */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);		//�ӻ���ַ
    OLED_Wait_Ack();
    OLED_I2C_SendByte(0x00);		//д����
    OLED_Wait_Ack();
    OLED_I2C_SendByte(Command);
    OLED_Wait_Ack();
    OLED_I2C_Stop();
}

/**
  * @brief  OLEDд����
  * @param  Data Ҫд�������
  * @retval ��
  */
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);		//�ӻ���ַ
    OLED_Wait_Ack();
    OLED_I2C_SendByte(0x40);		//д����
    OLED_Wait_Ack();
    OLED_I2C_SendByte(Data);
    OLED_Wait_Ack();
    OLED_I2C_Stop();
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
    OLED_Refresh_Gram();//������ʾ
}

//��������

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WriteCommand(0xb0+y);
    OLED_WriteCommand(((x&0xf0)>>4)|0x10);
    OLED_WriteCommand((x&0x0f));
}




/**
 * @brief       ��ָ��λ����ʾ����
 * @param       x   : 0~127
 * @param       y   : 0~7
 * @param       len: ���ָ���
 * @retval      ��
 */
void oled_show_chinese(int x,int y,int no)
{
    uint8_t t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
    {
        OLED_WriteData(china[2*no][t]);
        adder+=1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
    {
        OLED_WriteData(china[2*no+1][t]);
        adder+=1;
    }

}

//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//������Χ��.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
{
    u8 x,y;
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
    }
    OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
    u8 temp,t,t1;
    u8 y0=y;
    u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr=chr-' ';//�õ�ƫ�ƺ��ֵ
    for(t=0;t<csize;t++)
    {
        if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
        else if(size==16)temp=asc2_1608[chr][t];	//����1608����
        else if(size==24)temp=asc2_2412[chr][t];	//����2412����
        else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}
//m^n����
u32 mypow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
                continue;
            }else enshow=1;

        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1);
    }
}
//��ʾ�ַ���
//x,y:�������
//size:�����С
//*p:�ַ�����ʼ��ַ
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);
        x+=size/2;
        p++;
    }


}
//��ʼ��SSD1306
void OLED_Init(void)
{
    RCC->AHB1ENR|=1<<1;    //ʹ��PORTBʱ��
    GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//PB8/PB9����
    OLED_SDIN=1;
    OLED_SCLK=1;

    delay_ms(100);

//    OLED_WriteCommand(0xAE);//--turn off oled panel
//    OLED_WriteCommand(0x02);//---SET low column address
//    OLED_WriteCommand(0x10);//---SET high column address
//    OLED_WriteCommand(0x40);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
//    OLED_WriteCommand(0x81);//--SET contrast control register
//    OLED_WriteCommand(0xCF); // SET SEG Output Current Brightness
//    OLED_WriteCommand(0xA1);//--SET SEG/Column Mapping     0xa0������������?? 0xa1?y3��
//    OLED_WriteCommand(0xC8);//SET COM/Row Scan Direction   0xc0��???����?? 0xc8?y3��
//    OLED_WriteCommand(0xA6);//--SET normal display
//    OLED_WriteCommand(0xA8);//--SET multiplex ratio(1 to 64)
//    OLED_WriteCommand(0x3f);//--1/64 duty
//    OLED_WriteCommand(0xD3);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
//    OLED_WriteCommand(0x00);//-not offSET
//    OLED_WriteCommand(0xd5);//--SET display clock divide ratio/oscillator frequency
//    OLED_WriteCommand(0x80);//--SET divide ratio, SET Clock as 100 Frames/Sec
//    OLED_WriteCommand(0xD9);//--SET pre-charge period
//    OLED_WriteCommand(0xF1);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
//    OLED_WriteCommand(0xDA);//--SET com pins hardware configuration
//    OLED_WriteCommand(0x12);
//    OLED_WriteCommand(0xDB);//--SET vcomh
//    OLED_WriteCommand(0x40);//SET VCOM Deselect Level
//    OLED_WriteCommand(0x20);//-SET Page Addressing Mode (0x00/0x01/0x02)
//    OLED_WriteCommand(0x02);//
//    OLED_WriteCommand(0x8D);//--SET Charge Pump enable/disable
//    OLED_WriteCommand(0x14);//--SET(0x10) disable
//    OLED_WriteCommand(0xA4);// Disable Entire Display On (0xa4/0xa5)
//    OLED_WriteCommand(0xA6);// Disable Inverse Display On (0xa6/a7)
//    OLED_WriteCommand(0xAF);//--turn on oled panel
//    OLED_WriteCommand(0xAF); /*display ON*/

//    0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
//
//            0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
//
//            0xD8, 0x30, 0x8D, 0x14, 0xAF
//    OLED_WriteCommand(0xAE); //�ر���ʾ
//    OLED_WriteCommand(0x00); //����ʱ�ӷ�Ƶ����,��Ƶ��
//    OLED_WriteCommand(0x10); //[3:0],��Ƶ����;[7:4],��Ƶ��
//    OLED_WriteCommand(0x40); //��������·��
//    OLED_WriteCommand(0xB0); //Ĭ��0X3F(1/64)
//    OLED_WriteCommand(0x81); //������ʾƫ��
//    OLED_WriteCommand(0xFF); //Ĭ��Ϊ0
//    OLED_WriteCommand(0xA1); //������ʾ��ʼ�� [5:0],����.
//    OLED_WriteCommand(0xA6); //��ɱ�����
//    OLED_WriteCommand(0xA8); //bit2������/�ر�
//    OLED_WriteCommand(0x3F); //�����ڴ��ַģʽ
//    OLED_WriteCommand(0xC8); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
//    OLED_WriteCommand(0xD3); //���ض�������,bit0:0,0->0;1,0->127;   0xA0<-->0xA1
//    OLED_WriteCommand(0x00); //����COMɨ�跽��; 0xC0<-->0xC1:��ֱ����
//    OLED_WriteCommand(0xD5); //����COMӲ����������
//    OLED_WriteCommand(0x80); //[5:4]����
//    OLED_WriteCommand(0xD8); //�Աȶ�����
//    OLED_WriteCommand(0x05); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
//    OLED_WriteCommand(0xD9); //����Ԥ�������
//    OLED_WriteCommand(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
//    OLED_WriteCommand(0xDA); //����VCOMH ��ѹ����
//    OLED_WriteCommand(0x12); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
//    OLED_WriteCommand(0xD8); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
//    OLED_WriteCommand(0x30); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
//    OLED_WriteCommand(0x8D); //������ʾ
//    OLED_WriteCommand(0x14); //������ʾ
//    OLED_WriteCommand(0xAF); //������ʾ


    OLED_WriteCommand(0xAE); //�ر���ʾ
    OLED_WriteCommand(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
    OLED_WriteCommand(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
    OLED_WriteCommand(0xA8); //��������·��
    OLED_WriteCommand(0X3F); //Ĭ��0X3F(1/64)
    OLED_WriteCommand(0xD3); //������ʾƫ��
    OLED_WriteCommand(0X00); //Ĭ��Ϊ0
    OLED_WriteCommand(0x40); //������ʾ��ʼ�� [5:0],����.
    OLED_WriteCommand(0x8D); //��ɱ�����
    OLED_WriteCommand(0x14); //bit2������/�ر�
    OLED_WriteCommand(0x20); //�����ڴ��ַģʽ
    OLED_WriteCommand(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    OLED_WriteCommand(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
    OLED_WriteCommand(0xC0); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
    OLED_WriteCommand(0xDA); //����COMӲ����������
    OLED_WriteCommand(0x12); //[5:4]����
    OLED_WriteCommand(0x81); //�Աȶ�����
    OLED_WriteCommand(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
    OLED_WriteCommand(0xD9); //����Ԥ�������
    OLED_WriteCommand(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WriteCommand(0xDB); //����VCOMH ��ѹ����
    OLED_WriteCommand(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WriteCommand(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    OLED_WriteCommand(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    OLED_WriteCommand(0xAF); //������ʾ

    OLED_Clear();				//OLED����
}





























