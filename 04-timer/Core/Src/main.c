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

extern u8  TIM4CH1_CAPTURE_STA;	//���벶��״̬
extern u32	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ(TIM4��32λ)

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

    // ��ʼ��key
    key_init();

    // ��ʼ���ⲿ�ж�
    //exti_init();

    //TIM3_Init(5000-1, 8400-1);

    //gtim_timx_pwm_init(500-1, 84-1);

    OLED_Init();					//��ʼ��OLED


    Tim4_Init(0XFFFF,84-1); // ��1MHZ��Ƶ�ʼ���

//    oled_show_chinese(0,0,0);
//    oled_show_chinese(0,5,1);




//    printf("%d\r\n",0);
//    printf("hello\r\n");
    printf("���\r\n");

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
//            printf("�ߵ�ƽ");
//        }
        if (TIM4CH1_CAPTURE_STA&0x80) // �ɹ�������һ�θߵ�ƽ
        {
            temp = TIM4CH1_CAPTURE_STA&0X3F; // ����Ĵ���
            temp *= 0XFFFF; // ���ʱ���ܺ�
            temp += TIM4CH1_CAPTURE_VAL;  // ��ʱ��û�жϽ�����ȡֵ
            printf("HIGH:%lld us \r\n", temp);
            TIM4CH1_CAPTURE_STA = 0; // ������һ�β���
        }






    }
}




