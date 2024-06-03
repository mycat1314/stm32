//
// Created by 86156 on 2024/6/1.
//

#include "gtimer.h"

TIM_HandleTypeDef g_timer_pwm_handle;
TIM_OC_InitTypeDef timx_oc_pwm;

/**
 * 通用定时器PWM初始化函数
 */
void gtim_timx_pwm_init(uint16_t arr, uint16_t psc)
{

    g_timer_pwm_handle.Instance = TIM5;
    g_timer_pwm_handle.Init.Prescaler = psc;
    g_timer_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timer_pwm_handle.Init.Period = arr;
    g_timer_pwm_handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&g_timer_pwm_handle);


    timx_oc_pwm.OCMode = TIM_OCMODE_PWM1;
    timx_oc_pwm.Pulse = arr / 2;
    timx_oc_pwm.OCPolarity = TIM_OCPOLARITY_LOW;
    HAL_TIM_PWM_ConfigChannel(&g_timer_pwm_handle,&timx_oc_pwm,TIM_CHANNEL_2);


    HAL_TIM_PWM_Start(&g_timer_pwm_handle, TIM_CHANNEL_2);
}

/**
 * 定时器输出 PWM MSP 初始化函数
 * @param htim
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        GPIO_InitTypeDef gpioInitTypeDef;
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_TIM5_CLK_ENABLE();

        gpioInitTypeDef.Pin = GPIO_PIN_1;
        gpioInitTypeDef.Mode = GPIO_MODE_AF_PP;
        gpioInitTypeDef.Pull = GPIO_PULLUP;
        gpioInitTypeDef.Speed = GPIO_SPEED_HIGH;
        gpioInitTypeDef.Alternate = GPIO_AF2_TIM5;
        HAL_GPIO_Init(GPIOA, &gpioInitTypeDef);

    }
}
//设置TIM通道4的占空比
//compare:比较值
void TIM_SetTIM14Compare1(u32 compare)
{
    TIM5->CCR2=compare;
}




