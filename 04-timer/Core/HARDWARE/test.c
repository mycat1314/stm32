//
// Created by 86156 on 2024/6/2.
//
#include "test.h"
#include "oled.h"
#include "stdio.h"
#include "key.h"
#include "gtimer.h"

int16_t ledpwnval = 0;
uint8_t dir = 1;
uint8_t is_start = 0;
uint16_t max_val = 500;
uint8_t is_max_speed = 0;
uint8_t  is_high_speed = 0;
uint8_t  is_low_speed = 0;
void test_tim_pwm(void )
{
    OLED_ShowString(18,0,"^_^ Show Info ^_^",12);

    OLED_ShowString(0,14,"Running Status:",12);

    OLED_ShowString(90,14,is_start == 1 ? "Open" : "Close",12);

    OLED_ShowString(0, 28, "Speed Ratio:", 12 );

    char* str;
    float result = (1 - ((float)ledpwnval / max_val)) * 100; // 计算结果

    sprintf(str, "%.0f%%", result); // 将结果转换为字符串，并添加百分号
    OLED_ShowString(90, 28, str, 12);

    OLED_ShowString(0,42,"Is Max Speed:",12);
    OLED_ShowString(90,42,is_max_speed == 1 ? "Yes" : "No",12);

    OLED_ShowString(0,52,"Turn Speed:",12);
    int num = (is_high_speed ||  is_low_speed) ? is_low_speed ? -50 : +50 : 0;
    char turnSpeedStr[20];;
    sprintf(turnSpeedStr, "%d", num); // 将 num 转换为字符串
    OLED_ShowString(90,52,turnSpeedStr,12);




    OLED_Refresh_Gram();//更新显示到OLED

    u8 key = KEY_Scan(0);
    if (key == KEY0_PRES) {
        is_max_speed =  !is_max_speed;
        OLED_Clear();
    } else if (key == KEY1_PRES) {
        OLED_Clear();
        is_low_speed = 1;
        ledpwnval += 50;
        if (ledpwnval >= max_val) {
            ledpwnval = max_val;
        }
    } else if (key == KEY2_PRES) {
        OLED_Clear();
        is_start = !is_start;
    } else if (key == WKUP_PRES) {
        OLED_Clear();
        is_high_speed = 1;
        ledpwnval -= 50;
        if (ledpwnval <= 0) {
            ledpwnval = 0;
        }
    }

    if (is_start) {
        if (is_max_speed) {
            ledpwnval = 0;
        } else {
            if (!(is_low_speed) && !(is_high_speed)) {
                if (dir) ledpwnval+=20;
                else ledpwnval-=20;
            }

        }

        if (ledpwnval > max_val) dir=0;
        if (ledpwnval <= 0) dir=1;

        //__HAL_TIM_SET_COMPARE(&timHandleTypeDef, TIM_CHANNEL_1, ledpwnval);

        TIM_SetTIM14Compare1(ledpwnval);	//修改比较值，修改占空比
    } else {
        is_start = 0;
        is_high_speed = 0;
        is_low_speed = 0;
        is_max_speed = 0;
        ledpwnval = max_val;
        TIM_SetTIM14Compare1(ledpwnval);	//修改比较值，修改占空比

    }
}

