//
// Created by 86156 on 2024/6/2.
//

#include "timerCapture.h"

TIM_HandleTypeDef tim4HandleTypeDef;




/**
 * 定时器 4 输入捕获配置
 * @param arr 自动装置值
 * @param psc 时钟预分频数
 */
void Tim4_Init(u16 arr, u16 psc)
{
    TIM_IC_InitTypeDef timIcInitTypeDef;

    // 初始化输出捕获时基参数
    tim4HandleTypeDef.Instance = TIM4;
    tim4HandleTypeDef.Init.Prescaler = psc;
    tim4HandleTypeDef.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim4HandleTypeDef.Init.Period = arr;
    tim4HandleTypeDef.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&tim4HandleTypeDef);


    timIcInitTypeDef.ICPolarity = TIM_ICPOLARITY_RISING; // 上升沿捕获
    timIcInitTypeDef.ICFilter = 0; // 配置输出滤波器，不滤波
    timIcInitTypeDef.ICPrescaler = TIM_ICPSC_DIV1; // 配置输入分频，不分频， 每个周期都捕获
    timIcInitTypeDef.ICSelection = TIM_ICSELECTION_DIRECTTI; // 映射到 TI1 上
    // 配置 TIM4 通道1
    HAL_TIM_IC_ConfigChannel(&tim4HandleTypeDef, &timIcInitTypeDef, TIM_CHANNEL_1);
    // 开启更新中断
    HAL_TIM_IC_Start_IT(&tim4HandleTypeDef,TIM_CHANNEL_1);   //开启TIM4的捕获通道1，并且开启捕获中断
    __HAL_TIM_ENABLE_IT(&tim4HandleTypeDef, TIM_IT_UPDATE);

}

/**
 * 定时器4底层驱动，时钟使能，引脚配置，中断配置
 * @param htim 定时器句柄
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        GPIO_InitTypeDef  gpioInitTypeDef;
        __HAL_RCC_GPIOD_CLK_ENABLE(); // 使能 GPIOD 时钟
        __HAL_RCC_TIM4_CLK_ENABLE(); // 使能 TIM4 时钟

        gpioInitTypeDef.Pin = GPIO_PIN_12; // PD12
        gpioInitTypeDef.Mode = GPIO_MODE_AF_PP; // 复用推挽输出
        gpioInitTypeDef.Pull = GPIO_PULLDOWN; // 下拉
        gpioInitTypeDef.Speed = GPIO_SPEED_HIGH; // 高速
        gpioInitTypeDef.Alternate = GPIO_AF2_TIM4; // PD12 复用 TIM4 通道1

        HAL_GPIO_Init(GPIOD, &gpioInitTypeDef);

        HAL_NVIC_SetPriority(TIM4_IRQn, 2 ,0); // 设置中断优先级，抢占优先级2， 响应优先级0
        HAL_NVIC_EnableIRQ(TIM4_IRQn); // 开启 TIM4 中断通道

    }
}

/**
 * 定时器4中断函数
 */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&tim4HandleTypeDef);
}


//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM4CH1_CAPTURE_STA=0;	//输入捕获状态
u32	TIM4CH1_CAPTURE_VAL;	//输入捕获值(TIM4是32位)


/**
 * 定时器更新中断，计数溢出中断回调函数，该函数在 HAL_TIM_IRQHandler 中调用
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        if ((TIM4CH1_CAPTURE_STA&0x80) == 0) // 还未成功捕获
        {
            if ((TIM4CH1_CAPTURE_STA&0x40)) // 捕获到高电平
            {
                if ((TIM4CH1_CAPTURE_STA&0x3F) == 0X3F) // 高电平时间太长
                {
                    TIM4CH1_CAPTURE_STA|=0x80; // 标记成功捕获一次
                    TIM4CH1_CAPTURE_VAL = 0XFFFF;
                } else TIM4CH1_CAPTURE_STA++;
            }
        }
    } else if (htim->Instance == TIM3)
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
        //LED1=!LED1; // LED1 反转
    }
}


/**
 * 定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler 中会被调用
 * @param htim 定时器句柄
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) // 捕获中断发生时执行
{
    if (htim->Instance == TIM4)
    {
        if ((TIM4CH1_CAPTURE_STA&0x80) == 0) // 还未成功捕获
        {
            if ((TIM4CH1_CAPTURE_STA&0x40)) // 捕获到一个下降岩
            {
                TIM4CH1_CAPTURE_STA|=0x80; // 标记成功捕获到一次高电平脉宽
                // 获取定时器4通道1的捕获值
                TIM4CH1_CAPTURE_VAL= HAL_TIM_ReadCapturedValue(&tim4HandleTypeDef,TIM_CHANNEL_1);
                // 清除原来的设置
                TIM_RESET_CAPTUREPOLARITY(&tim4HandleTypeDef,TIM_CHANNEL_1);
                // 配置TIM4通道1上升岩捕获
                TIM_SET_CAPTUREPOLARITY(&tim4HandleTypeDef, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
            } else // 还未开始的时候，第一次捕获上升岩
            {
                TIM4CH1_CAPTURE_STA = 0;
                TIM4CH1_CAPTURE_VAL = 0;
                TIM4CH1_CAPTURE_STA |= 0x40; // 编辑已经捕获到上升岩
                // 关闭定时器4
                __HAL_TIM_DISABLE(&tim4HandleTypeDef);
                __HAL_TIM_SET_COUNTER(&tim4HandleTypeDef, 0);

                // 清除原来的设置
                TIM_RESET_CAPTUREPOLARITY(&tim4HandleTypeDef,TIM_CHANNEL_1);
                // 设置为下降岩
                TIM_SET_CAPTUREPOLARITY(&tim4HandleTypeDef,TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);
                // 使能定时器4
                __HAL_TIM_ENABLE(&tim4HandleTypeDef);

            }
        }
    }
}