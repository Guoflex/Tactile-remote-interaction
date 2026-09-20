/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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
#define HV2801_1_DATA_Pin GPIO_PIN_4
#define HV2801_1_DATA_GPIO_Port GPIOA
#define HV2801_1_CS_Pin GPIO_PIN_5
#define HV2801_1_CS_GPIO_Port GPIOA
#define HV2801_1_CLK_Pin GPIO_PIN_6
#define HV2801_1_CLK_GPIO_Port GPIOA
#define HV2801_1_CLR_Pin GPIO_PIN_7
#define HV2801_1_CLR_GPIO_Port GPIOA
#define HV2801_2_DATA_Pin GPIO_PIN_4
#define HV2801_2_DATA_GPIO_Port GPIOC
#define HV2801_2_CS_Pin GPIO_PIN_5
#define HV2801_2_CS_GPIO_Port GPIOC
#define HV2801_2_CLK_Pin GPIO_PIN_0
#define HV2801_2_CLK_GPIO_Port GPIOB
#define HV2801_2_CLR_Pin GPIO_PIN_1
#define HV2801_2_CLR_GPIO_Port GPIOB
#define HV2801_3_DATA_Pin GPIO_PIN_2
#define HV2801_3_DATA_GPIO_Port GPIOB
#define HV2801_3_CS_Pin GPIO_PIN_7
#define HV2801_3_CS_GPIO_Port GPIOE
#define HV2801_3_CLK_Pin GPIO_PIN_8
#define HV2801_3_CLK_GPIO_Port GPIOE
#define HV2801_3_CLR_Pin GPIO_PIN_9
#define HV2801_3_CLR_GPIO_Port GPIOE
#define DAC80502_DATA_Pin GPIO_PIN_10
#define DAC80502_DATA_GPIO_Port GPIOE
#define DAC80502_SYNC_Pin GPIO_PIN_11
#define DAC80502_SYNC_GPIO_Port GPIOE
#define DAC80502_CLK_Pin GPIO_PIN_12
#define DAC80502_CLK_GPIO_Port GPIOE
#define PCAP_SCL_Pin GPIO_PIN_10
#define PCAP_SCL_GPIO_Port GPIOB
#define PCAP_SDA_Pin GPIO_PIN_11
#define PCAP_SDA_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_12
#define LED_B_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_13
#define LED_R_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_14
#define LED_G_GPIO_Port GPIOB
#define LED_R_9_Pin GPIO_PIN_8
#define LED_R_9_GPIO_Port GPIOC
#define LED_R_8_Pin GPIO_PIN_9
#define LED_R_8_GPIO_Port GPIOC
#define LED_R_7_Pin GPIO_PIN_8
#define LED_R_7_GPIO_Port GPIOA
#define LED_R_6_Pin GPIO_PIN_15
#define LED_R_6_GPIO_Port GPIOA
#define LED_R_5_Pin GPIO_PIN_10
#define LED_R_5_GPIO_Port GPIOC
#define LED_R_4_Pin GPIO_PIN_11
#define LED_R_4_GPIO_Port GPIOC
#define LED_R_3_Pin GPIO_PIN_12
#define LED_R_3_GPIO_Port GPIOC
#define LED_R_2_Pin GPIO_PIN_0
#define LED_R_2_GPIO_Port GPIOD
#define LED_R_1_Pin GPIO_PIN_1
#define LED_R_1_GPIO_Port GPIOD
#define LED_R_0_Pin GPIO_PIN_2
#define LED_R_0_GPIO_Port GPIOD
#define LED_B_9_Pin GPIO_PIN_3
#define LED_B_9_GPIO_Port GPIOD
#define LED_B_8_Pin GPIO_PIN_4
#define LED_B_8_GPIO_Port GPIOD
#define LED_B_7_Pin GPIO_PIN_5
#define LED_B_7_GPIO_Port GPIOD
#define LED_B_6_Pin GPIO_PIN_6
#define LED_B_6_GPIO_Port GPIOD
#define LED_B_5_Pin GPIO_PIN_7
#define LED_B_5_GPIO_Port GPIOD
#define LED_B_4_Pin GPIO_PIN_3
#define LED_B_4_GPIO_Port GPIOB
#define LED_B_3_Pin GPIO_PIN_4
#define LED_B_3_GPIO_Port GPIOB
#define LED_B_2_Pin GPIO_PIN_5
#define LED_B_2_GPIO_Port GPIOB
#define LED_B_1_Pin GPIO_PIN_6
#define LED_B_1_GPIO_Port GPIOB
#define LED_B_0_Pin GPIO_PIN_7
#define LED_B_0_GPIO_Port GPIOB
#define TEST1_Pin GPIO_PIN_8
#define TEST1_GPIO_Port GPIOB
#define TEST2_Pin GPIO_PIN_9
#define TEST2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern volatile uint8_t tim7_is_up;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
