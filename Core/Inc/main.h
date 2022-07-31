/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SMG_SCLK_Pin GPIO_PIN_0
#define SMG_SCLK_GPIO_Port GPIOA
#define SMG_RCLK_Pin GPIO_PIN_1
#define SMG_RCLK_GPIO_Port GPIOA
#define SMG_DIO_Pin GPIO_PIN_2
#define SMG_DIO_GPIO_Port GPIOA
#define AMP_EN_Pin GPIO_PIN_3
#define AMP_EN_GPIO_Port GPIOA
#define AMP_SW_Pin GPIO_PIN_4
#define AMP_SW_GPIO_Port GPIOA
#define MUTE_LED_Pin GPIO_PIN_5
#define MUTE_LED_GPIO_Port GPIOA
#define FM_TRUE_LED_Pin GPIO_PIN_6
#define FM_TRUE_LED_GPIO_Port GPIOA
#define FUNCTION_SW_Pin GPIO_PIN_7
#define FUNCTION_SW_GPIO_Port GPIOA
#define EC11_A_Pin GPIO_PIN_0
#define EC11_A_GPIO_Port GPIOB
#define EC11_A_EXTI_IRQn EXTI0_IRQn
#define EC11_B_Pin GPIO_PIN_1
#define EC11_B_GPIO_Port GPIOB
#define IR_Pin GPIO_PIN_3
#define IR_GPIO_Port GPIOB
#define IR_EXTI_IRQn EXTI3_IRQn
#define EC11_SW_Pin GPIO_PIN_4
#define EC11_SW_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
