
#include "tim.h"
//TIM_HandleTypeDef htim3;

//void tim_init(void){
//		
//	
// __HAL_RCC_TIM3_CLK_ENABLE();
//	
// TIM_OC_InitTypeDef sConfigOC = {0};	

//	htim3.Instance=TIM3;                                          //定时器3
//	//htim3.Channel=HAL_TIM_ACTIVE_CHANNEL_3;                       //通道3
//	htim3.Init.Period=50;                                         //自动重转载值
//	htim3.Init.Prescaler=800-1;                                   //预分频值
//	htim3.Init.CounterMode=TIM_COUNTERMODE_UP;                    //想上计数
//	htim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;              //时钟不分频
//	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; //自动重装 开
//	htim3.Init.RepetitionCounter =1;                              //重复计数
//if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sConfigOC.OCMode = TIM_OCMODE_PWM1;
//  
//  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//	sConfigOC.OCNPolarity=TIM_OCNPOLARITY_HIGH;
//	sConfigOC.OCNIdleState=TIM_OCNIDLESTATE_RESET;
//	sConfigOC.OCIdleState=TIM_OCIDLESTATE_RESET;
//	
//  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN TIM2_Init 2 */

//  /* USER CODE END TIM2_Init 2 */
//  HAL_TIM_MspPostInit(&htim3);


//}
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
//{

//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(timHandle->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM2_MspPostInit 0 */

//  /* USER CODE END TIM2_MspPostInit 0 */

//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**TIM2 GPIO Configuration
//    PA0-WKUP     ------> TIM2_CH1
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_0;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//		GPIO_InitStruct.Pull=GPIO_PULLDOWN;
//		GPIO_InitStruct.Alternate=GPIO_AF1_TIM3;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  /* USER CODE BEGIN TIM2_MspPostInit 1 */

//  /* USER CODE END TIM2_MspPostInit 1 */
//  }

//}

///* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim3;
TIM_OC_InitTypeDef sConfigOC = {0};

void MX_TIM3_Init(void)
{

  htim3.Instance = TIM3;     																			//定时器TIM3
  htim3.Init.Prescaler = 48; 																			//预分频
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;                    //计数模式
  htim3.Init.Period = 200;                                        //重装载值
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;							//时钟分频
  htim3.Init.RepetitionCounter = 0;                               
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;   //自动重装载使能

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 10;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  //sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  //sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	//sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
  //sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

 // HAL_TIM_MspPostInit(&htim3);

}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{		GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM3)
  {

    __HAL_RCC_TIM3_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		 GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

}

