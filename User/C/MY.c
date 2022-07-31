#include "MY.h"
MY_Struct MY = {2, 0, 2, 10670, 0};
Key_Struct Key_AMP = {GPIOA, GPIO_PIN_4, 0, 0, 0};
Key_Struct Key_FUNCTION = {GPIOA, GPIO_PIN_7, 0, 0, 0};
Key_Struct Key_DETERMINE = {GPIOA, GPIO_PIN_8, 0, 0, 0};
Key_Struct Key_EC11 = {GPIOB, GPIO_PIN_4, 0, 0, 0};

uint8_t Key_SW = 0;             //按键功能是否打开
int F_A_Freq_js = 0;            //频率闪烁计时
int F_A_Freq_ins = 0;           //频率闪烁状态
uint8_t radio_station_Len = 10; //电台数量

/**
 * @brief 功放开关
 * @param bool:0 关闭   1 打开
 * @return 无
 * @author HZ12138
 * @date 2022-07-27 10:23:38
 */
void AMP_EN(uint8_t bool)
{
    if (bool == 0)
        HAL_GPIO_WritePin(AMP_EN_GPIO_Port, AMP_EN_Pin, GPIO_PIN_RESET);
    else if (bool == 1)

        HAL_GPIO_WritePin(AMP_EN_GPIO_Port, AMP_EN_Pin, GPIO_PIN_SET);
}
/**
 * @brief 键盘扫描
 * @param Key:键盘结构体地址
 * @param Down:按键按下的回调函数
 * @param Up:按键弹起的回调函数
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 15:46:00
 */
void Key_Scan(Key_Struct *Key, void (*Down)(void), void (*Up)(void))
{
    if (HAL_GPIO_ReadPin(Key->GPIOx, Key->GPIO_Pin) == GPIO_PIN_RESET && Key->ins == 0)
    {
        Key->i++;
        if (Key->i > 10)
        {
            Key->i = 0;
            Key->ins = 1;
            Key->bool = 1;
            Down();
        }
    }
    else if (HAL_GPIO_ReadPin(Key->GPIOx, Key->GPIO_Pin) == GPIO_PIN_SET && Key->ins == 1)
    {
        Key->i++;
        if (Key->i > 10)
        {
            Key->i = 0;
            Key->ins = 0;
            Key->bool = 0;
            Up();
        }
    }
}
/**
 * @brief 检测是否是电台并更改指示灯
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 15:49:55
 */
void check_R_D(void)
{
    if (RDA5807M_Radio_Instructions() == 1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    }
}
/**
 * @brief 功放开关
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 15:50:28
 */
void AMP_Change(void)
{
    if (HAL_GPIO_ReadPin(AMP_EN_GPIO_Port, AMP_EN_Pin) == GPIO_PIN_RESET)
        HAL_GPIO_WritePin(AMP_EN_GPIO_Port, AMP_EN_Pin, GPIO_PIN_SET);
    else if (HAL_GPIO_ReadPin(AMP_EN_GPIO_Port, AMP_EN_Pin) == GPIO_PIN_SET)
        HAL_GPIO_WritePin(AMP_EN_GPIO_Port, AMP_EN_Pin, GPIO_PIN_RESET);
}
/**
 * @brief 功能选择改变函数（按键触发）
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 15:50:59
 */
void Function_Change(void)
{
    //改变指示的值
    MY.function_ins++;
    if (MY.function_ins > 2)
        MY.function_ins = 0;

    //根据功能更新需要的数据
    if (MY.function_ins == 0)
    {
        MY.volume = RDA5807M_Read_Reg(0x05) & 0xf;
    }
    else if (MY.function_ins == 2)
    {
        MY.Freq_ins = 0;
        MY.Freq = RDA5807M_Read_Freq();
    }

    check_R_D();
}
/**
 * @brief 空函数
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 15:51:19
 */
void air(void)
{
}
/**
 * @brief 功能选择实现函数（定时器扫描）
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 15:55:51
 */
void Function_Action(void)
{
    if (MY.function_ins == 0)
    { //音量

        Tube_Val[0] = Tube_Lest[MY.volume / 10 % 10];
        Tube_Val[1] = Tube_Lest[MY.volume % 10];
        for (int i = 2; i <= 7; i++)
            Tube_Val[i] = Tube_Bleak;
        Tube_Spot_P = 0xff;
    }
    else if (MY.function_ins == 1)
    { //电台
        for (int i = 0; i <= 1; i++)
            Tube_Val[i] = Tube_Bleak;

        Tube_Val[2] = Tube_Lest[MY.radio_station_num / 10 % 10];
        Tube_Val[3] = Tube_Lest[MY.radio_station_num % 10];

        MY.Freq = RDA5807M_RadioStadion_Freq[MY.radio_station_num];

        Tube_Val[4] = Tube_Lest[MY.Freq / 10000 % 10];
        Tube_Val[5] = Tube_Lest[MY.Freq / 1000 % 10];
        Tube_Val[6] = Tube_Lest[MY.Freq / 100 % 10];
        Tube_Val[7] = Tube_Lest[MY.Freq / 10 % 10];
        Tube_Spot_P = 6;
    }
    else if (MY.function_ins == 2)
    { //频率
        //闪烁计时
        F_A_Freq_js++;
        if (F_A_Freq_js > 250)
        {
            if (F_A_Freq_ins == 0)
                F_A_Freq_ins = 1;
            else if (F_A_Freq_ins == 1)
                F_A_Freq_ins = 0;
            F_A_Freq_js = 0;
        }
        //前面变黑
        for (int i = 0; i <= 3; i++)
            Tube_Val[i] = Tube_Bleak;
        //更新数据到数码管
        Tube_Val[4] = Tube_Lest[MY.Freq / 10000 % 10];
        Tube_Val[5] = Tube_Lest[MY.Freq / 1000 % 10];
        Tube_Val[6] = Tube_Lest[MY.Freq / 100 % 10];
        Tube_Val[7] = Tube_Lest[MY.Freq / 10 % 10];
        //闪烁实现
        if (F_A_Freq_ins == 1)
        {
            Tube_Val[7 - MY.Freq_ins] = Tube_Bleak;
        }
        //小数点
        Tube_Spot_P = 6;
    }
}
/**
 * @brief 旋转编码器实现
 * @param bool:0增加 1减少
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 16:12:23
 */
void EC11_Achive(uint8_t bool)
{
    if (MY.function_ins == 0)
    {                       //音量
        int zj = MY.volume; //避免无负数问题

        if (bool == 0)
            zj++;
        else if (bool == 1)
            zj--;

        if (zj >= 15)
            zj = 15;
        else if (zj <= 0)
            zj = 0;
        MY.volume = zj;
    }
    else if (MY.function_ins == 1)
    {                                  //电台
        int zj = MY.radio_station_num; //避免无负数问题

        if (bool == 0)
            zj++;
        else if (bool == 1)
            zj--;

        if (zj <= 0)
            zj = 0;
        if (zj >= radio_station_Len)
            zj = radio_station_Len;
        MY.radio_station_num = zj;
        MY.Freq = RDA5807M_RadioStadion_Freq[MY.radio_station_num];
    }
    else if (MY.function_ins == 2)
    { //频率
        int zj = 1;

        F_A_Freq_ins = 0; //开始时从最低位置开始
        F_A_Freq_js = 0;  //更改后从亮开始闪烁
                          //跟据选择增加频率
        if (bool == 0)
            zj = 1;
        else if (bool == 1)
            zj = -1;

        if (MY.Freq_ins == 0)
            MY.Freq += zj * 10;
        else if (MY.Freq_ins == 1)
            MY.Freq += zj * 100;
        else if (MY.Freq_ins == 2)
            MY.Freq += zj * 1000;
        else if (MY.Freq_ins == 3)
            MY.Freq += zj * 10000;
        //范围限制
        if (MY.Freq >= 10800)
            MY.Freq = 10800;
        else if (MY.Freq <= 7600)
            MY.Freq = 7600;
    }
}
/**
 * @brief 旋转编码器增加
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 16:06:14
 */
void EC11_Change_Up(void)
{
    EC11_Achive(0);
}
/**
 * @brief 旋转编码器减少
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 16:08:29
 */
void EC11_Change_Down(void)
{
    EC11_Achive(1);
}
/**
 * @brief 旋转编码器按键（改变频率的设定值）
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 16:13:47
 */
void EC11_Change(void)
{
    MY.Freq_ins++;
    if (MY.Freq_ins > 3)
        MY.Freq_ins = 0;
}
/**
 * @brief 将设置好的功能发送给芯片（确定按键）
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-29 16:18:32
 */
void DETERMINE(void)
{
    if (MY.function_ins == 0)
    { //音量
        if (MY.volume == 0)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            RDA5807M_SetMutea(1);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            RDA5807M_SetMutea(0);
            RDA5807M_Set_Volume(MY.volume);
        }
    }
    else if (MY.function_ins == 1)
    { //电台
        RDA5807M_Set_Freq(MY.Freq);
    }
    else if (MY.function_ins == 2)
    { //频率
        RDA5807M_Set_Freq(MY.Freq);
    }
}
