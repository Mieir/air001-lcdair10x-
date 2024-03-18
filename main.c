#include "main.h"
#include "spi.h"
#include "st7735.h"
#include "gpio.h"
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
	
	HAL_Init();
	APP_SystemClockConfig();
	HAL_Delay(200);
  MX_SPI1_Init();

	ST7735_Init();
	GPIO_Init();

uint8_t state=1;
	
 Lcd_time(4,23,30,40,24);
	while (1)
	{ 
	


		
/*
		if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET)||(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)==RESET)){
			HAL_Delay(10);
		if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET)||(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)==RESET)){
			state=1+state;
		}
		}
		if(state>3){
		state=1;
		}
		if(state<0){
		state=3;
		}
		if(state==1){
		Lcd_DrawWrods(16,36,2,12,0x0001,0xffff);	
		Lcd_DrawWrods(16,0,2,4,0xffff,0x0001);
		Lcd_DrawWrods(16,18,2,8,0x0001,0xffff);
		}
		if(state==2){
		Lcd_DrawWrods(16,0,2,4,0x0001,0xffff);
		Lcd_DrawWrods(16,18,2,8,0xffff,0x0001);
		Lcd_DrawWrods(16,36,2,12,0x0001,0xffff);		
		}
		if(state==3){
		Lcd_DrawWrods(16,0,2,4,0x0001,0xffff);
		Lcd_DrawWrods(16,18,2,8,0x0001,0xffff);
		Lcd_DrawWrods(16,36,2,12,0xffff,0x0001);		
		}*/
		/*switch (state){
				
					case 1:{
						Lcd_DrawWord(0,16,0x0001,0xffff,font3,32);
						Lcd_DrawWord(16,16,0x0001,0xffff,font4,32);
						Lcd_DrawWord(0,32,0x0001,0xffff,font5,32);
						Lcd_DrawWord(16,32,0x0001,0xffff,font6,32);
						Lcd_DrawWord(0,0,0xffff,0x0001,font1,32);
						Lcd_DrawWord(16,0,0xffff,0x0001,font2,32);
						
					
						break;
				}
					case 2:{	
						Lcd_DrawWord(0,0,0x0001,0xffff,font1,32);
						Lcd_DrawWord(16,0,0x0001,0xffff,font2,32);	
						Lcd_DrawWord(0,16,0xffff,0x0001,font3,32);
						Lcd_DrawWord(16,16,0xffff,0x0001,font4,32);
						Lcd_DrawWord(0,32,0x0001,0xffff,font5,32);
						Lcd_DrawWord(16,32,0x0001,0xffff,font6,32);
						break;
				}
					case 3:{
						Lcd_DrawWord(0,0,0x0001,0xffff,font1,32);
						Lcd_DrawWord(16,0,0x0001,0xffff,font2,32);
						Lcd_DrawWord(0,16,0x0001,0xffff,font3,32);
						Lcd_DrawWord(16,16,0x0001,0xffff,font4,32);
						Lcd_DrawWord(0,32,0xffff,0x0001,font5,32);
						Lcd_DrawWord(16,32,0xffff,0x0001,font6,32);
						
						break;
				}
					
				}*/



	}
}
