#include "main.h"
#include "gpio.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

int main(void) {

    HAL_Init();
    // ��ʼ��ϵͳʱ��
    Stm32_Clock_Init(336, 8, 2, 7);

    MX_GPIO_Init();

    // ��ʼ����ʱ����
    delay_init(168);

    // ��ʼ��uart
    uart_init(115200);

    // ��ʼ�� led
    LED_Init();

    printf("%d\r\n",0);
    printf("hello\r\n");
    printf("���\r\n");

    while (1) {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
        delay_ms(500);
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
        delay_ms(500);
    }
}




