/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, HV2801_1_DATA_Pin|HV2801_1_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, HV2801_1_CS_Pin|HV2801_1_CLR_Pin|LED_R_7_Pin|LED_R_6_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HV2801_2_DATA_GPIO_Port, HV2801_2_DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, HV2801_2_CS_Pin|LED_R_9_Pin|LED_R_8_Pin|LED_R_5_Pin
                          |LED_R_4_Pin|LED_R_3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, HV2801_2_CLK_Pin|HV2801_3_DATA_Pin|TEST1_Pin|TEST2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, HV2801_2_CLR_Pin|LED_B_Pin|LED_R_Pin|LED_G_Pin
                          |LED_B_4_Pin|LED_B_3_Pin|LED_B_2_Pin|LED_B_1_Pin
                          |LED_B_0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, HV2801_3_CS_Pin|HV2801_3_CLR_Pin|DAC80502_DATA_Pin|DAC80502_SYNC_Pin
                          |DAC80502_CLK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HV2801_3_CLK_GPIO_Port, HV2801_3_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED_R_2_Pin|LED_R_1_Pin|LED_R_0_Pin|LED_B_9_Pin
                          |LED_B_8_Pin|LED_B_7_Pin|LED_B_6_Pin|LED_B_5_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : HV2801_1_DATA_Pin HV2801_1_CS_Pin HV2801_1_CLK_Pin HV2801_1_CLR_Pin */
  GPIO_InitStruct.Pin = HV2801_1_DATA_Pin|HV2801_1_CS_Pin|HV2801_1_CLK_Pin|HV2801_1_CLR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : HV2801_2_DATA_Pin HV2801_2_CS_Pin */
  GPIO_InitStruct.Pin = HV2801_2_DATA_Pin|HV2801_2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : HV2801_2_CLK_Pin HV2801_2_CLR_Pin HV2801_3_DATA_Pin */
  GPIO_InitStruct.Pin = HV2801_2_CLK_Pin|HV2801_2_CLR_Pin|HV2801_3_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : HV2801_3_CS_Pin HV2801_3_CLK_Pin HV2801_3_CLR_Pin DAC80502_DATA_Pin
                           DAC80502_SYNC_Pin DAC80502_CLK_Pin */
  GPIO_InitStruct.Pin = HV2801_3_CS_Pin|HV2801_3_CLK_Pin|HV2801_3_CLR_Pin|DAC80502_DATA_Pin
                          |DAC80502_SYNC_Pin|DAC80502_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_B_Pin LED_R_Pin LED_G_Pin LED_B_4_Pin
                           LED_B_3_Pin LED_B_2_Pin LED_B_1_Pin LED_B_0_Pin
                           TEST1_Pin TEST2_Pin */
  GPIO_InitStruct.Pin = LED_B_Pin|LED_R_Pin|LED_G_Pin|LED_B_4_Pin
                          |LED_B_3_Pin|LED_B_2_Pin|LED_B_1_Pin|LED_B_0_Pin
                          |TEST1_Pin|TEST2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_R_9_Pin LED_R_8_Pin LED_R_5_Pin LED_R_4_Pin
                           LED_R_3_Pin */
  GPIO_InitStruct.Pin = LED_R_9_Pin|LED_R_8_Pin|LED_R_5_Pin|LED_R_4_Pin
                          |LED_R_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_R_7_Pin LED_R_6_Pin */
  GPIO_InitStruct.Pin = LED_R_7_Pin|LED_R_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_R_2_Pin LED_R_1_Pin LED_R_0_Pin LED_B_9_Pin
                           LED_B_8_Pin LED_B_7_Pin LED_B_6_Pin LED_B_5_Pin */
  GPIO_InitStruct.Pin = LED_R_2_Pin|LED_R_1_Pin|LED_R_0_Pin|LED_B_9_Pin
                          |LED_B_8_Pin|LED_B_7_Pin|LED_B_6_Pin|LED_B_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
