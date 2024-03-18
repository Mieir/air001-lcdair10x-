#include "spi.h"
//����spi �Ĺ���ģʽ
//MX_SPI1_Init()��������cubemx���ɵ�spi��ʼ������
void MX_SPI1_Init(void){
 Spi1Handle.Instance               = SPI1;                       /* SPI1 */
  Spi1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;    /* 2��Ƶ */
  Spi1Handle.Init.Direction         = SPI_DIRECTION_2LINES;       /* ȫ˫�� */
  Spi1Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* ʱ�Ӽ��Ե� */
  Spi1Handle.Init.CLKPhase          = SPI_PHASE_1EDGE ;           /* ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ */
  Spi1Handle.Init.DataSize          = SPI_DATASIZE_8BIT;          /* SPI���ݳ���Ϊ8bit */
  Spi1Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* �ȷ���MSB */
  Spi1Handle.Init.NSS               = SPI_NSS_HARD_OUTPUT;        /* NSS���ģʽ(Ӳ��ģʽ) */
  Spi1Handle.Init.Mode = SPI_MODE_MASTER;                         /* ����Ϊ���� */
	//spi �ر�
	HAL_SPI_DeInit(&Spi1Handle);
  /*SPI��ʼ��*/
	//����ģʽ������ɺ���HAL��spi�ĳ�ʼ���������г�ʼ��
   HAL_SPI_Init(&Spi1Handle) ;
}

//HAL_SPI_Init() ��ɺ�����HAL_SPI_MspInit(���HAL_SPI_MspInit������)����spi��������
void HAL_SPI_MspInit(SPI_HandleTypeDef * Spi1Handle){

  GPIO_InitTypeDef  GPIO_InitStruct;
  /* SPI1 ��ʼ�� */
  if (Spi1Handle->Instance == SPI1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();                   /* GPIOBʱ��ʹ�� */
    __HAL_RCC_GPIOA_CLK_ENABLE();                   /* GPIOAʱ��ʹ�� */
    __HAL_RCC_SYSCFG_CLK_ENABLE();                  /* ʹ��SYSCFGʱ�� */
    __HAL_RCC_SPI1_CLK_ENABLE();                    /* SPI1ʱ��ʹ�� */
 
    /*
      PA5-SCK  (AF0)
      PA6-MISO(AF0)
      PA7-MOSI(AF0)
      PA4-NSS(AF0)
    */
		HAL_NVIC_SetPriority(SPI1_IRQn,2,0);//spi1�ж�����
		HAL_NVIC_EnableIRQ(SPI1_IRQn); //spi1�ж�ʹ��
  
    /*GPIO����ΪSPI��SCK/MISO/MOSI*/
    GPIO_InitStruct.Pin       = GPIO_PIN_5| GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
}
	}

	
//spi ��λ����	
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /* ��λSPI���� */
    __HAL_RCC_SPI1_FORCE_RESET();
    __HAL_RCC_SPI1_RELEASE_RESET();

    /* �ر������GPIOʱ�� */
    /* ȡ������SPI SCK*/
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
		HAL_NVIC_DisableIRQ(SPI1_IRQn);

  }
}
