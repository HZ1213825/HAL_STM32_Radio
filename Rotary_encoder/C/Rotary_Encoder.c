#include "Rotary_Encoder.h"
/**
 * @brief 旋转编码器解码函数（A相的外部中断调用）
 * @param Clockwise 顺时钟时的回调函数
 * @param Anticlockwise 逆时针时的回调函数
 * @return 无
 * @author HZ12138
 * @date 2022-07-12 15:45:59
 */
void EC11_Decode(void (*Clockwise)(void), void (*Anticlockwise)(void))
{
    // HAL_Delay(1);
    if (HAL_GPIO_ReadPin(EC11_B_GPIO_Group, EC11_B_GPIO_PIN) == GPIO_PIN_SET)
    {
        Anticlockwise();
    }
    else
    {
        Clockwise();
    }
}
