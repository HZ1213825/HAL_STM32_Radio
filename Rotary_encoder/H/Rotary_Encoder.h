#ifndef _ROTARY_ENCODER_
#define _ROTARY_ENCODER_
#include "main.h"
/*
需要：
    1.外部中断（A相）
        上升沿触发中断
    2.GPIO（B相）
        上拉输入模式
 */
//外部中断
#define EC11_A_GPIO_Group GPIOB
#define EC11_A_GPIO_PIN GPIO_PIN_0

// GPIO输入
#define EC11_B_GPIO_Group GPIOB
#define EC11_B_GPIO_PIN GPIO_PIN_1
void EC11_Decode(void (*Clockwise)(void), void (*Anticlockwise)(void));
#endif
