#include "air001xx_it.h"
#include "air001xx_hal.h"

extern SPI_HandleTypeDef Spi1Handle; 
void NMI_Handler(void)
{
}
void Error_Handler(void){




}
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
/**
  * @brief This function handles SPI1 global interrupt.
  */
//spi1ÖÐ¶Ï´¥·¢º¯Êý
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&Spi1Handle);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}
