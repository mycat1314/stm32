//
// Created by 86156 on 2024/5/14.
//
#include "led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    // 开启 GPIOF 时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();

    // 设置引脚
    GPIO_Initure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    // 推挽输出
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(GPIOF, &GPIO_Initure);

    // 默认关闭LED灯
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
}
