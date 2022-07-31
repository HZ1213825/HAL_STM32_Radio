#ifndef _MY_H_
#define _MY_H_
#include "main.h"
#include "RDA5807M.h"
#include "Print.h"
#include "Rotary_Encoder.h"
#include "TUBE.h"
typedef struct
{
    uint8_t function_ins;
    uint8_t volume;
    uint8_t radio_station_num;
    uint16_t Freq;
    uint8_t Freq_ins;
} MY_Struct;

typedef struct
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    uint8_t ins;
    uint8_t bool;
    uint8_t i;
} Key_Struct;

extern MY_Struct MY;
extern Key_Struct Key_AMP;
extern Key_Struct Key_FUNCTION;
extern Key_Struct Key_DETERMINE;
extern Key_Struct Key_EC11;
extern uint8_t Key_SW;
extern uint8_t radio_station_Len;

void AMP_EN(uint8_t bool);
void Key_Scan(Key_Struct *Key, void (*Down)(void), void (*Up)(void));
void check_R_D(void);
void AMP_Change(void);
void Function_Change(void);
void air(void);
void Function_Action(void);
void EC11_Change_Up(void);
void EC11_Change_Down(void);
void EC11_Change(void);
void DETERMINE(void);
#endif
