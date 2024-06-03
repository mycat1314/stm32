#include "main.h"
#include "gpio.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "exti.h"

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
    exti_init();

//    printf("%d\r\n",0);
//    printf("hello\r\n");
//    printf("你好\r\n");

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



    }
}




