//
// Created by 86156 on 2024/5/14.
//

#ifndef INC_01_LED_DELAY_H
#define INC_01_LED_DELAY_H
#include "sys.h"
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
#endif //INC_01_LED_DELAY_H
