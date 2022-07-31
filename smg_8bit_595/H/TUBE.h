#ifndef _TUBE_H
#define _TUBE_H
#include "main.h"

//更改端口时需要更改
#define Tube_SCLK_GPIOx GPIOA
#define Tube_SCLK GPIO_PIN_0

#define Tube_RCLK_GPIOx GPIOA
#define Tube_RCLK GPIO_PIN_1

#define Tube_DIO_GPIOx GPIOA
#define Tube_DIO GPIO_PIN_2

//数码管的值
typedef enum
{
	Tube_0 = 0xC0,
	Tube_1 = 0xF9,
	Tube_2 = 0xA4,
	Tube_3 = 0xB0,
	Tube_4 = 0x99,
	Tube_5 = 0x92,
	Tube_6 = 0x82,
	Tube_7 = 0xF8,
	Tube_8 = 0x80,
	Tube_9 = 0x90,
	Tube_A = 0x88,
	Tube_b = 0x83,
	Tube_C = 0xC6,
	Tube_d = 0xA1,
	Tube_E = 0x86,
	Tube_F = 0x8E,
	Tube_Line = 0xbf,  //显示-
	Tube_Bleak = 0xff, //全不显示
	Tube_Spot = 0x7F,  //显示小数点
	Tube_White = 0x00  //全都显示
} Tube_Val_Typedef;

extern Tube_Val_Typedef Tube_Val[8];
extern const Tube_Val_Typedef Tube_Lest[16];
extern uint8_t Tube_Spot_P;

void Time_Init(void);
void Tube_Out_Send(uint8_t X);
void Tube_Show(void);
void Tube_Out(uint8_t Digit, Tube_Val_Typedef Data, FunctionalState Spot);
void Time_Init(void);
void Tube_Clear(void);
void Tube_Init(void);
void Tube_Send_8Bit(Tube_Val_Typedef *Data, uint8_t Spot_P);
void Tube_Send_Scan(Tube_Val_Typedef *Data, uint8_t Spot_P);
//更改定时器时需要更改，如出现重复定义请删除另一个

#endif
