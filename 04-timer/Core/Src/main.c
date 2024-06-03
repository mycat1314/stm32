#include "main.h"
#include "gpio.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "exti.h"
#include "btimer.h"
#include "gtimer.h"
#include "oled.h"
#include "timerCapture.h"

extern u8  TIM4CH1_CAPTURE_STA;	//输入捕获状态
extern u32	TIM4CH1_CAPTURE_VAL;	//输入捕获值(TIM4是32位)

int main(void) {

    HAL_Init();
    // 初始化系统时钟
    Stm32_Clock_Init(336, 8, 2, 7);

    MX_GPIO_Init();

    // 初始化延时函数
    delay_init(168);

    // 初始化uart
    uart_init(115200);

    // 初始化 led
    LED_Init();

    // 初始化key
    key_init();

    // 初始化外部中断
    //exti_init();

    //TIM3_Init(5000-1, 8400-1);

    //gtim_timx_pwm_init(500-1, 84-1);

    OLED_Init();					//初始化OLED


    Tim4_Init(0XFFFF,84-1); // 以1MHZ的频率计算

//    oled_show_chinese(0,0,0);
//    oled_show_chinese(0,5,1);




//    printf("%d\r\n",0);
//    printf("hello\r\n");
    printf("你好\r\n");

    long long temp=0;
    while (1) {
//        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
//        delay_ms(500);
//        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
//        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
//        delay_ms(500);

//        if (KEY_Scan(0))
//        {
//            HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
//        }
//        else
//        {
//            delay_ms(10);
//        }

        delay_ms(10);
//        GPIO_PinState pin = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12);
//        if (pin == GPIO_PIN_SET)
//        {
//            printf("高电平");
//        }
        if (TIM4CH1_CAPTURE_STA&0x80) // 成功捕获到有一次高电平
        {
            temp = TIM4CH1_CAPTURE_STA&0X3F; // 捕获的次数
            temp *= 0XFFFF; // 溢出时间总和
            temp += TIM4CH1_CAPTURE_VAL;  // 定时器没中断结束的取值
            printf("HIGH:%lld us \r\n", temp);
            TIM4CH1_CAPTURE_STA = 0; // 开启下一次捕获
        }






    }
}




