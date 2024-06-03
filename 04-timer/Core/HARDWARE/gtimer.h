//
// Created by 86156 on 2024/6/1.
//

#ifndef INC_02_USART_GTIMER_H
#define INC_02_USART_GTIMER_H

#include "sys.h"
extern TIM_HandleTypeDef g_timer_pwm_handle;

void gtim_timx_pwm_init(uint16_t arr, uint16_t psc);
void TIM_SetTIM14Compare1(u32 compare);
#endif //INC_02_USART_GTIMER_H
