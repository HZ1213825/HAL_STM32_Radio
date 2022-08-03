/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "RDA5807M.h"
#include "Print.h"
#include "MY.h"
#include "Rotary_Encoder.h"
#include "TUBE.h"
#include "IR_NEC.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_0)
  {
    EC11_Decode(EC11_Change_Up, EC11_Change_Down);
  }
  else if (GPIO_Pin == GPIO_PIN_3)
  {
    IR_NEC_Read_Decode(air);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2)
  {

    if (Key_SW)
    {
      Function_Action();
      Key_Scan(&Key_AMP, AMP_Change, air);
      Key_Scan(&Key_FUNCTION, Function_Change, air);
      Key_Scan(&Key_EC11, EC11_Change, air);
      Key_Scan(&Key_DETERMINE, DETERMINE, air);
    }
    Tube_Send_Scan(Tube_Val, Tube_Spot_P);
  }
  else if (htim == &htim3)
  {
    if (Key_SW)
      check_R_D();
  }
}
uint32_t tohex(uint8_t *Val)
{
  uint32_t zj = 0;
  for (int i = 0; i < 4; i++)
  {
    zj <<= 8;
    zj |= Val[i];
  }
  return zj;
}
void IR_Right()
{
  int ins = 0;
  if (MY.function_ins == 0)
  {
    MY.function_ins = 1;
  }
  else if (MY.function_ins == 1)
  {
    MY.function_ins = 2;
    MY.Freq_ins = 3;
  }
  else if (MY.function_ins == 2)
  {
    ins = MY.Freq_ins;
    ins--;
    if (ins < 0)
    {
      ins = 0;
      MY.function_ins = 0;
    }
    MY.Freq_ins = ins;
  }
}
void IR_Left()
{
  if (MY.function_ins == 0)
  {
    MY.function_ins = 2;
  }
  else if (MY.function_ins == 1)
  {
    MY.function_ins = 0;
    MY.Freq_ins = 0;
  }
  else if (MY.function_ins == 2)
  {
    MY.Freq_ins++;
    if (MY.Freq_ins > 3)
    {
      MY.Freq_ins = 3;
      MY.function_ins = 1;
    }
  }
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t zj = 0;
  uint8_t volume_zj = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  // RDA5807M_init();
  // HAL_Delay(500);
  // RDA5807M_Set_Volume(2);
  // HAL_Delay(50);
  // radio_station_Len = RDA5807M_Search_ALL_Freq() - 1;
  // RDA5807M_Set_Freq(10670);

  Key_SW = 1;
  AMP_EN(1);
  check_R_D();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (IR_NEC_Read_OK)
    {

      zj = tohex(IR_NEC_Read_Dat);
      if (zj == 0x00FF5AA5) //右
        IR_Right();
      else if (zj == 0x00FF10EF) //左
        IR_Left();
      else if (zj == 0x00FF18E7) //上
        EC11_Change_Up();
      else if (zj == 0x00FF4AB5) //下
        EC11_Change_Down();
      else if (zj == 0x00FF38C7) // OK
        DETERMINE();
      else if (zj == 0x00FF6897) //功放
        AMP_Change();
      else if (zj == 0x00FF6897) //功放
        AMP_Change();
      else if (zj == 0x00FFB04F)
      {

        if (MY.volume == 0)
          MY.volume = volume_zj;
        else
        {
          MY.volume = 0;
          volume_zj = MY.volume;
        }
        DETERMINE();
      }
      printf("%08X\r\n", zj);
      HAL_Delay(100);
      IR_NEC_Read_OK = 0;
    }
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
