#include "main.h"
#include "gpio.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

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

    printf("%d\r\n",0);
    printf("hello\r\n");
    printf("你好\r\n");

    while (1) {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
        delay_ms(500);
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
        delay_ms(500);
    }
}




