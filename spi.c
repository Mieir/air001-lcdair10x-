#include "spi.h"
//配置spi 的工作模式
//MX_SPI1_Init()函数是由cubemx生成的spi初始化函数
void MX_SPI1_Init(void){
 Spi1Handle.Instance               = SPI1;                       /* SPI1 */
  Spi1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;    /* 2分频 */
  Spi1Handle.Init.Direction         = SPI_DIRECTION_2LINES;       /* 全双工 */
  Spi1Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* 时钟极性低 */
  Spi1Handle.Init.CLKPhase          = SPI_PHASE_1EDGE ;           /* 数据采样从第一个时钟边沿开始 */
  Spi1Handle.Init.DataSize          = SPI_DATASIZE_8BIT;          /* SPI数据长度为8bit */
  Spi1Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* 先发送MSB */
  Spi1Handle.Init.NSS               = SPI_NSS_HARD_OUTPUT;        /* NSS软件模式(硬件模式) */
  Spi1Handle.Init.Mode = SPI_MODE_MASTER;                         /* 配置为主机 */
	//spi 关闭
	HAL_SPI_DeInit(&Spi1Handle);
  /*SPI初始化*/
	//工作模式配置完成后传入HAL库spi的初始化函数进行初始化
   HAL_SPI_Init(&Spi1Handle) ;
}

//HAL_SPI_Init() 完成后会调用HAL_SPI_MspInit(如果HAL_SPI_MspInit被定义)进行spi引脚配置
void HAL_SPI_MspInit(SPI_HandleTypeDef * Spi1Handle){

  GPIO_InitTypeDef  GPIO_InitStruct;
  /* SPI1 初始化 */
  if (Spi1Handle->Instance == SPI1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();                   /* GPIOB时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();                   /* GPIOA时钟使能 */
    __HAL_RCC_SYSCFG_CLK_ENABLE();                  /* 使能SYSCFG时钟 */
    __HAL_RCC_SPI1_CLK_ENABLE();                    /* SPI1时钟使能 */
 
    /*
      PA5-SCK  (AF0)
      PA6-MISO(AF0)
      PA7-MOSI(AF0)
      PA4-NSS(AF0)
    */
		HAL_NVIC_SetPriority(SPI1_IRQn,2,0);//spi1中断配置
		HAL_NVIC_EnableIRQ(SPI1_IRQn); //spi1中断使能
  
    /*GPIO配置为SPI：SCK/MISO/MOSI*/
    GPIO_InitStruct.Pin       = GPIO_PIN_5| GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
}
	}

	
//spi 复位函数	
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /* 复位SPI外设 */
    __HAL_RCC_SPI1_FORCE_RESET();
    __HAL_RCC_SPI1_RELEASE_RESET();

    /* 关闭外设和GPIO时钟 */
    /* 取消配置SPI SCK*/
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
		HAL_NVIC_DisableIRQ(SPI1_IRQn);

  }
}
