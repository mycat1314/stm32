//
// Created by 86156 on 2024/5/14.
//
#include "sys.h"

/**
 * 初始化系统时钟
 * @param plln
 * @param pllm
 * @param pllp
 * @param pllq
 */
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure;
    RCC_ClkInitTypeDef RCC_ClkInitStructure;

    // 使能 PWR 时钟
    __HAL_RCC_PWR_CLK_ENABLE();

    // 设置调压器输出电压级别，以便在器件未以最大频率工作时使性能与功耗实现平衡, 设置调压器输出电压级别为 1
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStructure.HSEState = RCC_HSE_ON;
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL.PLLM = pllm;
    RCC_OscInitStructure.PLL.PLLN = plln;
    RCC_OscInitStructure.PLL.PLLP = pllp;
    RCC_OscInitStructure.PLL.PLLQ = pllq;

    // 配置系统时钟源
    ret = HAL_RCC_OscConfig(&RCC_OscInitStructure);

    if (ret != HAL_OK) while (1);

    // 选中 PLLCLK 作为系统时钟源并且配置 HCLK(AHB1) PCLK1(APB1), PCLK2(APB2)
    RCC_ClkInitStructure.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV2;

    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_5);
    if (ret != HAL_OK) while (1);

    // STM32F405x/407x/415x/417x Z版本的器件支持预取功能
    if (HAL_GetREVID() == 0x1001)
    {
        // 使能 FLASH 预取
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }



}


//GPIO通用设置
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,位设置,每个位代表一个IO,第0位代表Px0,第1位代表Px1,依次类推.比如0X0101,代表同时设置Px0和Px8.
//MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
//OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
//OSPEED:0~3;输出速度设置,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh.
//PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
//注意:在输入模式(普通输入/模拟输入)下,OTYPE和OSPEED参数无效!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{
    u32 pinpos=0,pos=0,curpin=0;
    for(pinpos=0;pinpos<16;pinpos++)
    {
        pos=1<<pinpos;	//一个个位检查
        curpin=BITx&pos;//检查引脚是否要设置
        if(curpin==pos)	//需要设置
        {
            GPIOx->MODER&=~(3<<(pinpos*2));	//先清除原来的设置
            GPIOx->MODER|=MODE<<(pinpos*2);	//设置新的模式
            if((MODE==0X01)||(MODE==0X02))	//如果是输出模式/复用功能模式
            {
                GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//清除原来的设置
                GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//设置新的速度值
                GPIOx->OTYPER&=~(1<<pinpos) ;		//清除原来的设置
                GPIOx->OTYPER|=OTYPE<<pinpos;		//设置新的输出模式
            }
            GPIOx->PUPDR&=~(3<<(pinpos*2));	//先清除原来的设置
            GPIOx->PUPDR|=PUPD<<(pinpos*2);	//设置新的上下拉
        }
    }
}