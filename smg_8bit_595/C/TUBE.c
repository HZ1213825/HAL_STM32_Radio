#include "TUBE.h"

// 8个数码管的值 从左到右
Tube_Val_Typedef Tube_Val[8] = {Tube_Line, Tube_Line, Tube_Line, Tube_Line, Tube_Line, Tube_Line, Tube_Line, Tube_Line};

//数值列表分别是0-f
const Tube_Val_Typedef Tube_Lest[16] = {Tube_0, Tube_1, Tube_2, Tube_3, Tube_4, Tube_5, Tube_6, Tube_7, Tube_8, Tube_9, Tube_A, Tube_b, Tube_C, Tube_d, Tube_E, Tube_F};

//小数点的位置 从左到右数
uint8_t Tube_Spot_P = 0xff;

void Tube_Delay(void)
{
	int i = 0xF;
	while (i--)
		;
}
//发送数据
void Tube_Out_Send(uint8_t X)
{
	uint8_t i;
	//从高到低发送数据
	for (i = 8; i >= 1; i--)
	{
		if (X & 0x80)
		{
			HAL_GPIO_WritePin(Tube_DIO_GPIOx, Tube_DIO, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(Tube_DIO_GPIOx, Tube_DIO, GPIO_PIN_RESET);
		}
		X <<= 1;
		//必须加延迟
		Tube_Delay();
		//在SCLK中产生上升沿 表示一位数据送入
		HAL_GPIO_WritePin(Tube_SCLK_GPIOx, Tube_SCLK, GPIO_PIN_RESET);
		Tube_Delay();
		HAL_GPIO_WritePin(Tube_SCLK_GPIOx, Tube_SCLK, GPIO_PIN_SET);
		Tube_Delay();
	}
}

//显示
void Tube_Show(void)
{
	//在RCLK产生上升沿 锁存显示
	HAL_GPIO_WritePin(Tube_RCLK_GPIOx, Tube_RCLK, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Tube_RCLK_GPIOx, Tube_RCLK, GPIO_PIN_SET);
}

//发送一位数码管的数据 Digit:从左向右数的数据位数 Data:数据 Spot:是否显示小数点
void Tube_Out(uint8_t Digit, Tube_Val_Typedef Data, FunctionalState Spot)
{
	//先发送要显示的值
	if (Spot == ENABLE)
	{
		Tube_Out_Send(0x7F & Data);
	}
	else
	{
		Tube_Out_Send(Data);
	}

	//后发送显示的位置
	Tube_Out_Send(0x80 >> Digit);
	Tube_Show();
}

// Data:数据数组0位显示在最左边 Spot_P:为从左向右数点的位置
void Tube_Send_8Bit(Tube_Val_Typedef *Data, uint8_t Spot_P)
{
	for (int i = 0; i < 8; i++)
	{
		if (i == Spot_P)
		{
			Tube_Out(i, Data[i], ENABLE);
		}
		else
		{
			Tube_Out(i, Data[i], DISABLE);
		}
	}

	//解决扫描时最后一位高亮问题
	Tube_Out(7, Tube_Bleak, DISABLE);
	Tube_Show();
}
//定时器扫描用
void Tube_Send_Scan(Tube_Val_Typedef *Data, uint8_t Spot_P)
{
	static char i = 0;
	if (i == 7 - Spot_P)
	{
		Tube_Out(i, Data[7 - i], ENABLE);
	}
	else
	{
		Tube_Out(i, Data[7 - i], DISABLE);
	}
	i++;
	if (i > 7)
	{
		i = 0;
	}
	Tube_Show();
}

// 8个数码管全不点亮
void Tube_Clear(void)
{
	for (int i = 0; i < 8; i++)
	{
		Tube_Val[i] = Tube_Bleak;
	}
}
