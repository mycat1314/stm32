//
// Created by 86156 on 2024/5/29.
//
#include "exti.h"
#include "delay.h"

void exti_init(void)
{

    GPIO_InitTypeDef gpioInitTypeDef;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();



    gpioInitTypeDef.Pin = GPIO_PIN_0; // PA0
    gpioInitTypeDef.Mode = GPIO_MODE_IT_RISING; // 上升岩触发
    gpioInitTypeDef.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &gpioInitTypeDef);


    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);



}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);
    if (GPIO_Pin == GPIO_PIN_0) {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1) {
            HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
        }
    }
}