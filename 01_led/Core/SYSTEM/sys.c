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