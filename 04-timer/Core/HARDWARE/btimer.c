//
// Created by 86156 on 2024/5/31.
//
#include "btimer.h"
#include "led.h"
// 定时器句柄
TIM_HandleTypeDef TIM3_Handler;

/**
 *
 * @param arr 自动装置值
 * @param psc 时钟预分频数
 */
void TIM3_Init(u16 arr, u16 psc) {
    TIM3_Handler.Instance = TIM3; // 通用定时器3
    TIM3_Handler.Init.Prescaler=psc; // 分配系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP; // 向上计数器
    TIM3_Handler.Init.Period = arr; // 自动装置值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; // 时钟分频因子

    HAL_TIM_Base_Init(&TIM3_Handler);

    HAL_TIM_Base_Start_IT(&TIM3_Handler); // 使能定时器3和定时器3更新中断：TIM_IT_UPDATE

}

// 定时器底层驱动，开启定时器，设置中断优先级
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE(); // 使能定时器3
        HAL_NVIC_SetPriority(TIM3_IRQn, 1, 3); // 设置中断优先级，抢占优先级1，子优先级3
        HAL_NVIC_EnableIRQ(TIM3_IRQn); // 开启TIM3


    }
}

void TIM3_IRQHandler(void )
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3)
//    {
//        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
//        //LED1=!LED1; // LED1 反转
//    }
//}
