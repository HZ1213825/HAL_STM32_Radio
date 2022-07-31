#include "IR_NEC.h"
u8 IR_NEC_Read_ins = 0;      //指示解码类型 引导码/数据码
u8 IR_NEC_Read_Decode_i = 0; //解码循环变量
u8 IR_NEC_Read_Decode_j = 0; //解码循环变量
u8 IR_NEC_Read_OK = 0;       //解码成功标志
u32 IR_NEC_Read_Time = 0;    //下降沿时间差
u8 IR_NEC_Read_zj = 0;
u8 IR_NEC_Send_Square = 0; //方波
u8 IR_NEC_Read_Dat[N] = {0};
u8 IR_NEC_Read_Dat2[N] = {0};

// NEC解码函数，外部中断下降沿调用
void IR_NEC_Read_Decode(void (*val)(void))
{
    if (IR_NEC_Read_ins == 0) //检测初始低电平
    {
        IR_NEC_Read_ins = 1;
        HAL_TIM_Base_Start(&IR_NEC_Read_TIM);
        __HAL_TIM_SetCounter(&IR_NEC_Read_TIM, 0);
    }
    else if (IR_NEC_Read_ins == 1) //判断初始低电平到第二个低电平时间
    {
        IR_NEC_Read_Time = __HAL_TIM_GetCounter(&IR_NEC_Read_TIM);
        if (IR_NEC_Read_Time > 13500 - 1000 && IR_NEC_Read_Time < 13500 + 1000) // 13.5ms左右 引导码
        {

            IR_NEC_Read_ins = 2;
            __HAL_TIM_SetCounter(&IR_NEC_Read_TIM, 0);
            IR_NEC_Read_OK = 0;
            for (int i = 0; i < N; i++)
                IR_NEC_Read_Dat[i] = 0;
            IR_NEC_Read_zj = 0;
        }
        else if (IR_NEC_Read_Time > 11250 - 1000 && IR_NEC_Read_Time < 11250 + 1000) // 11.25ms左右 重复码
        {
            IR_NEC_Read_ins = 0;
            IR_NEC_Read_OK = 2;
            for (int i = 0; i < N; i++)
                IR_NEC_Read_Dat[i] = IR_NEC_Read_Dat2[i];

            __HAL_TIM_SetCounter(&IR_NEC_Read_TIM, 0);
        }
        else //超时或时间过短 复位
        {
            IR_NEC_Read_ins = 0;
        }
    }
    else if (IR_NEC_Read_ins == 2) //开始解码
    {
        IR_NEC_Read_Time = __HAL_TIM_GetCounter(&IR_NEC_Read_TIM);
        if (IR_NEC_Read_Time > 1120 - 500 && IR_NEC_Read_Time < 1120 + 500) // 1.12ms 写入0
        {
            IR_NEC_Read_zj <<= 1;   //向左移位
            IR_NEC_Read_zj &= 0xfe; //最低位置零
            IR_NEC_Read_Decode_i++;
        }
        else if (IR_NEC_Read_Time > 2250 - 500 && IR_NEC_Read_Time < 2250 + 500) // 2.25ms 写入1
        {
            IR_NEC_Read_zj <<= 1;
            IR_NEC_Read_zj |= 0x01;
            IR_NEC_Read_Decode_i++;
        }
        else //出错复位
        {
            IR_NEC_Read_ins = 0;
            IR_NEC_Read_Decode_i = 0;
            IR_NEC_Read_Decode_j = 0;
            IR_NEC_Read_zj = 0;
        }

        if (IR_NEC_Read_Decode_i >= 8) // uchar每位写入数据
        {
            IR_NEC_Read_Decode_i = 0;
            IR_NEC_Read_Dat[IR_NEC_Read_Decode_j] = IR_NEC_Read_zj;
            IR_NEC_Read_Decode_j++;
            IR_NEC_Read_zj = 0;
        }
        if (IR_NEC_Read_Decode_j >= N) //数据数组的不同位写入数据
        {
            IR_NEC_Read_Decode_i = 0;
            IR_NEC_Read_Decode_j = 0;
            IR_NEC_Read_ins = 0;
            IR_NEC_Read_OK = 1;
            IR_NEC_Read_zj = 0;
            for (int i = 0; i < N; i++)
                IR_NEC_Read_Dat2[i] = IR_NEC_Read_Dat[i];
            HAL_TIM_Base_Stop(&IR_NEC_Read_TIM);
            val();
        }

        __HAL_TIM_SetCounter(&IR_NEC_Read_TIM, 0);
    }
}
