#include "gpio.h"
void GPIO_Init(){
 GPIO_InitTypeDef  key={0};	
	__HAL_RCC_GPIOB_CLK_ENABLE();
 __HAL_RCC_GPIOF_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
	key.Pin=GPIO_PIN_0|GPIO_PIN_1;
	key.Mode=GPIO_MODE_INPUT;
	key.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOF,&key);
	
	key.Pin=GPIO_PIN_3;
	key.Mode=GPIO_MODE_INPUT;
	key.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB,&key);
}