#include "main.h"
#include "spi.h"
#include "st7735.h"
#include "gpio.h"
#include "tim.h"
 SPI_HandleTypeDef Spi1Handle;
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /*����ʱ��ԴHSE/HSI/LSE/LSI*/
  RCC_OscInitStruct.OscillatorType =RCC_OSCILLATORTYPE_LSI;//| RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI |   //RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                                      /* ����HSI */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;                           /* ����HSI���ʱ��Ϊ16MHz */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                                      /* HSI����Ƶ */
  //RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                                     /* �ر�HSE */
 // RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;                                                 /* HSE����Ƶ�ʷ�Χ16M~32M */
  //RCC_OscInitStruct.LSIState = RCC_LSI_ON;                                                     /* �ر�LSI */
 // RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                                                     /* �ر�LSE */
  //RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;                                            /* LSEĬ���������� */
	//RCC_OscInitStruct.PLL.

  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                                                  /* �ر�PLL */
  HAL_RCC_OscConfig(&RCC_OscInitStruct) ;

  /* ��ʼ��CPU,AHB,APB����ʱ�� */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* RCCϵͳʱ������ */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;                                        /* SYSCLK��Դѡ��ΪHSI */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                            /* APHʱ�Ӳ���Ƶ */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_SYSCLK_DIV1;                                             /* APBʱ�Ӳ���Ƶ */
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

uint8_t sum=0x43;
void HAL_MspInit(void)
{ 
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}


int main(void)
{	
	//HAL���ʼ��
	HAL_Init();
	//ʱ������
	APP_SystemClockConfig();
	//��ʱ�ȴ���Ļ�ϵ�
	HAL_Delay(200);
	//IO���ų�ʼ��
	GPIO_Init();
	//SPI ͨ�ų�ʼ��
  MX_SPI1_Init();
	//��Ļ������ʼ��
	ST7735_Init();
	//��ʱ����ʼ��
  MX_TIM3_Init();
	//PWM����
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	//ͼƬ��ʾ����
	Lcd_disPlayPicture(0,17);
	//ʱ����ʾ����
	Lcd_time(4,23,30,40,24);

	while (1)
	{ 
//��ѭ������
	}
}
