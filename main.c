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

  /*配置时钟源HSE/HSI/LSE/LSI*/
  RCC_OscInitStruct.OscillatorType =RCC_OSCILLATORTYPE_LSI;//| RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI |   //RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                                                      /* 开启HSI */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;                           /* 配置HSI输出时钟为16MHz */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                                                      /* HSI不分频 */
  //RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                                                     /* 关闭HSE */
 // RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;                                                 /* HSE工作频率范围16M~32M */
  //RCC_OscInitStruct.LSIState = RCC_LSI_ON;                                                     /* 关闭LSI */
 // RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                                                     /* 关闭LSE */
  //RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;                                            /* LSE默认驱动能力 */
	//RCC_OscInitStruct.PLL.

  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                                                  /* 关闭PLL */
  HAL_RCC_OscConfig(&RCC_OscInitStruct) ;

  /* 初始化CPU,AHB,APB总线时钟 */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* RCC系统时钟类型 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;                                        /* SYSCLK的源选择为HSI */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                                            /* APH时钟不分频 */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_SYSCLK_DIV1;                                             /* APB时钟不分频 */
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
	//HAL库初始化
	HAL_Init();
	//时钟配置
	APP_SystemClockConfig();
	//延时等待屏幕上电
	HAL_Delay(200);
	//IO引脚初始化
	GPIO_Init();
	//SPI 通信初始化
  MX_SPI1_Init();
	//屏幕驱动初始化
	ST7735_Init();
	//定时器初始化
  MX_TIM3_Init();
	//PWM开启
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	//图片显示函数
	Lcd_disPlayPicture(0,17);
	//时钟显示函数
	Lcd_time(4,23,30,40,24);

	while (1)
	{ 
//主循环函数
	}
}
