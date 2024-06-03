#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//OLED 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];

//更新显存到LCD
void OLED_Refresh_Gram(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WriteCommand (0xb0+i);    //设置页地址（0~7）
        OLED_WriteCommand (0x00);      //设置显示位置—列低地址
        OLED_WriteCommand (0x10);      //设置显示位置—列高地址
        for(n=0;n<128;n++)OLED_WriteData(OLED_GRAM[n][i]);
    }
}
/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
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
  * @brief  I2C停止
  * @param  无
  * @retval 无
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
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
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
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);		//从机地址
    OLED_Wait_Ack();
    OLED_I2C_SendByte(0x00);		//写命令
    OLED_Wait_Ack();
    OLED_I2C_SendByte(Command);
    OLED_Wait_Ack();
    OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);		//从机地址
    OLED_Wait_Ack();
    OLED_I2C_SendByte(0x40);		//写数据
    OLED_Wait_Ack();
    OLED_I2C_SendByte(Data);
    OLED_Wait_Ack();
    OLED_I2C_Stop();
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
    OLED_Refresh_Gram();//更新显示
}

//坐标设置

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WriteCommand(0xb0+y);
    OLED_WriteCommand(((x&0xf0)>>4)|0x10);
    OLED_WriteCommand((x&0x0f));
}




/**
 * @brief       在指定位置显示汉字
 * @param       x   : 0~127
 * @param       y   : 0~7
 * @param       len: 汉字个数
 * @retval      无
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

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//超出范围了.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
{
    u8 x,y;
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
    }
    OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
    u8 temp,t,t1;
    u8 y0=y;
    u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
    chr=chr-' ';//得到偏移后的值
    for(t=0;t<csize;t++)
    {
        if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
        else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
        else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
        else return;								//没有的字库
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
//m^n函数
u32 mypow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
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
//显示字符串
//x,y:起点坐标
//size:字体大小
//*p:字符串起始地址
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);
        x+=size/2;
        p++;
    }


}
//初始化SSD1306
void OLED_Init(void)
{
    RCC->AHB1ENR|=1<<1;    //使能PORTB时钟
    GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//PB8/PB9设置
    OLED_SDIN=1;
    OLED_SCLK=1;

    delay_ms(100);

//    OLED_WriteCommand(0xAE);//--turn off oled panel
//    OLED_WriteCommand(0x02);//---SET low column address
//    OLED_WriteCommand(0x10);//---SET high column address
//    OLED_WriteCommand(0x40);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
//    OLED_WriteCommand(0x81);//--SET contrast control register
//    OLED_WriteCommand(0xCF); // SET SEG Output Current Brightness
//    OLED_WriteCommand(0xA1);//--SET SEG/Column Mapping     0xa0×óóò·′?? 0xa1?y3￡
//    OLED_WriteCommand(0xC8);//SET COM/Row Scan Direction   0xc0é???·′?? 0xc8?y3￡
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
//    OLED_WriteCommand(0xAE); //关闭显示
//    OLED_WriteCommand(0x00); //设置时钟分频因子,震荡频率
//    OLED_WriteCommand(0x10); //[3:0],分频因子;[7:4],震荡频率
//    OLED_WriteCommand(0x40); //设置驱动路数
//    OLED_WriteCommand(0xB0); //默认0X3F(1/64)
//    OLED_WriteCommand(0x81); //设置显示偏移
//    OLED_WriteCommand(0xFF); //默认为0
//    OLED_WriteCommand(0xA1); //设置显示开始行 [5:0],行数.
//    OLED_WriteCommand(0xA6); //电荷泵设置
//    OLED_WriteCommand(0xA8); //bit2，开启/关闭
//    OLED_WriteCommand(0x3F); //设置内存地址模式
//    OLED_WriteCommand(0xC8); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
//    OLED_WriteCommand(0xD3); //段重定义设置,bit0:0,0->0;1,0->127;   0xA0<-->0xA1
//    OLED_WriteCommand(0x00); //设置COM扫描方向; 0xC0<-->0xC1:垂直反向
//    OLED_WriteCommand(0xD5); //设置COM硬件引脚配置
//    OLED_WriteCommand(0x80); //[5:4]配置
//    OLED_WriteCommand(0xD8); //对比度设置
//    OLED_WriteCommand(0x05); //1~255;默认0X7F (亮度设置,越大越亮)
//    OLED_WriteCommand(0xD9); //设置预充电周期
//    OLED_WriteCommand(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
//    OLED_WriteCommand(0xDA); //设置VCOMH 电压倍率
//    OLED_WriteCommand(0x12); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
//    OLED_WriteCommand(0xD8); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
//    OLED_WriteCommand(0x30); //设置显示方式;bit0:1,反相显示;0,正常显示
//    OLED_WriteCommand(0x8D); //开启显示
//    OLED_WriteCommand(0x14); //开启显示
//    OLED_WriteCommand(0xAF); //开启显示


    OLED_WriteCommand(0xAE); //关闭显示
    OLED_WriteCommand(0xD5); //设置时钟分频因子,震荡频率
    OLED_WriteCommand(80);   //[3:0],分频因子;[7:4],震荡频率
    OLED_WriteCommand(0xA8); //设置驱动路数
    OLED_WriteCommand(0X3F); //默认0X3F(1/64)
    OLED_WriteCommand(0xD3); //设置显示偏移
    OLED_WriteCommand(0X00); //默认为0
    OLED_WriteCommand(0x40); //设置显示开始行 [5:0],行数.
    OLED_WriteCommand(0x8D); //电荷泵设置
    OLED_WriteCommand(0x14); //bit2，开启/关闭
    OLED_WriteCommand(0x20); //设置内存地址模式
    OLED_WriteCommand(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_WriteCommand(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WriteCommand(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    OLED_WriteCommand(0xDA); //设置COM硬件引脚配置
    OLED_WriteCommand(0x12); //[5:4]配置
    OLED_WriteCommand(0x81); //对比度设置
    OLED_WriteCommand(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
    OLED_WriteCommand(0xD9); //设置预充电周期
    OLED_WriteCommand(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WriteCommand(0xDB); //设置VCOMH 电压倍率
    OLED_WriteCommand(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WriteCommand(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WriteCommand(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_WriteCommand(0xAF); //开启显示

    OLED_Clear();				//OLED清屏
}





























