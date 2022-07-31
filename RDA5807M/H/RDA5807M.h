#ifndef _RDA5807M_H_
#define _RDA5807M_H_

#include "main.h"
/*
需要：
1.I2C
    I2C
    (默认设置)
    标准模式
    时钟频率100kHz
    地址长度7bit
    不用填写设备地址
*/
// #define RDA6807M_I2C_Handle hi2c1
#define RDA5807M_N 20

// extern I2C_HandleTypeDef hi2c1;
extern uint16_t RDA5807M_RadioStadion_Freq[RDA5807M_N];

#define RDA6807M_Group_SCL GPIOB // I2C的时钟GPIO组号
#define RDA6807M_SCL GPIO_PIN_6  // I2C时钟的GPIO端口号

#define RDA6807M_Group_SDA GPIOB // I2C的数据GPIO组号
#define RDA6807M_SDA GPIO_PIN_7  // I2C数据的GPIO端口号

#define RDA6807M_Write_SCL(x) HAL_GPIO_WritePin(RDA6807M_Group_SCL, RDA6807M_SCL, x)
#define RDA6807M_Write_SDA(x) HAL_GPIO_WritePin(RDA6807M_Group_SDA, RDA6807M_SDA, x)

#define RDA6807M_Read_SCL() HAL_GPIO_ReadPin(RDA6807M_Group_SCL, RDA6807M_SCL)
#define RDA6807M_Read_SDA() HAL_GPIO_ReadPin(RDA6807M_Group_SDA, RDA6807M_SDA)

/** @defgroup 频率段选择组
 * @{
 */
#define RDA6807M_Freq_Range_87_108 1
#define RDA6807M_Freq_Range_76_91 2
#define RDA6807M_Freq_Range_76_108 3
#define RDA6807M_Freq_Range_65_76 4
#define RDA6807M_Freq_Range_50_76 5
/**
 * @}
 */

/** @defgroup 频率间隔选择组
 * @{
 */
#define RDA6807M_Freq_Space_100kHz 1
#define RDA6807M_Freq_Space_200kHz 2
#define RDA6807M_Freq_Space_50KHz 3
#define RDA6807M_Freq_Space_25KHz 4
/**
 * @}
 */
void RDA5807M_Write_Reg(uint8_t Address, uint16_t Data);
uint16_t RDA5807M_Read_Reg(uint8_t Address);
void RDA5807M_init(void);
uint16_t RDA5807M_FreqToChan(uint16_t Freq);
uint16_t RDA5807M_ChanToFreq(uint16_t Chan);
uint16_t RDA5807M_Read_Freq(void);
void RDA5807M_Set_Freq(uint16_t Freq);
void RDA5807M_Search_Freq_TurnUp(void);
uint8_t RDA5807M_Radio_Instructions(void);
uint16_t RDA5807M_Search_ALL_Freq(void);
void RDA5807M_Set_Volume(uint8_t Val);
void RDA5807M_SetMutea(uint8_t Bool);
void RDA5807M_Set_Output_Idle(uint8_t Bool);
uint8_t RDA5807M_Read_Signal_Intensity(void);
uint16_t RDA5807M_Read_ID(void);
#endif
