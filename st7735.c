#include "st7735.h"
#include "spi.h"
#include "font.h"
#include "picture.h"
#include "tim.h"
#include "stdbool.h"
#define DC GPIO_PIN_1
//#define BL GPIO_PIN_0
#define CS GPIO_PIN_4
#define RES GPIO_PIN_6

#define RES_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)
#define RES_H HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)

#define CS_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
#define CS_H HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)

#define DC_L HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)
#define DC_H HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
//BL背光由Pwm调节
//#define BL_L HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)
//#define BL_H HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)

//screen size
#define LCD_W 160
#define LCD_H 80

//offset
//显示偏移
#define LCD_OFFSET_X 1
#define LCD_OFFSET_Y 26
//gpio init
//spi相关io初始化函数
void SPI_GPIO(){
	GPIO_InitTypeDef ss;
	ss.Pin=CS|RES;
	ss.Mode=GPIO_MODE_OUTPUT_PP;
	ss.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ee;
	ee.Pin=DC;//|BL;
	ee.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	ee.Mode=GPIO_MODE_OUTPUT_PP;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &ee);
	HAL_GPIO_Init(GPIOA, &ss);
	CS_H;
}
	
//spi数据发送
void SPI_Write_Data(uint8_t data){
		CS_L;
	//  spi 传输接口
		HAL_SPI_Transmit(&Spi1Handle,&data,1,1000);
		CS_H;
}
//命令发送函数
void Lcd_Write_Cmd(uint8_t cmd){
	DC_L;
	SPI_Write_Data(cmd);
	DC_H;
}
//数据写入函数
void Lcd_Write_Data(uint8_t data){
	
	SPI_Write_Data(data);
	
}
void Lcd_Write_Data16(uint16_t data){
	SPI_Write_Data(data>>8);//先写入高8位数据
	SPI_Write_Data(data);//写入低8位数据
}
void Lcd_Set_Address(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	Lcd_Write_Cmd(0x2a);//x轴的写入范围
	Lcd_Write_Data((x1+LCD_OFFSET_X)>>8);//先写入高8位
	Lcd_Write_Data(x1+LCD_OFFSET_X);     //写入低8位
	Lcd_Write_Data((x2+LCD_OFFSET_X)>>8);
	Lcd_Write_Data(x2+LCD_OFFSET_X);
	Lcd_Write_Cmd(0x2b);//y轴的写入范围
	Lcd_Write_Data((y1+LCD_OFFSET_Y)>>8);
	Lcd_Write_Data(y1+LCD_OFFSET_Y);
	Lcd_Write_Data((y2+LCD_OFFSET_Y)>>8);
	Lcd_Write_Data(y2+LCD_OFFSET_Y);
	Lcd_Write_Cmd(0x2c);//写入命令结束
}
//屏幕填充函数
void Lcd_Fill(uint16_t color){
	uint32_t i;
	//片选信号
	CS_L;
	Lcd_Set_Address(0,0,LCD_W-1,LCD_H-1);
	for(i = 0; i < LCD_W * LCD_H; i++)
	{ 
		//每个像素点为16位RGB
		//发送高8位
		Lcd_Write_Data(color>>8);
		//发送低8位
		Lcd_Write_Data(color);
	}
	CS_H;
}
//画点函数
void Lcd_DrawPoint(uint16_t x,uint16_t y,uint16_t color){
	Lcd_Set_Address(x-1,y-1,x,y);
	Lcd_Write_Data(color>>8);
	Lcd_Write_Data(color);	
}
//输出单个汉字
//parm
//@ x,y 文字显示的起始位置
//@ Font_color 字体颜色
//@ Brackground_color 字体背景色
void Lcd_DrawWord(uint16_t x,uint16_t y,uint16_t Font_color,uint16_t Brackground_color,uint8_t *font,uint8_t font_length){
	
    Lcd_Set_Address(x,y,x+(font_length/2)-1,y+(font_length/2)-1);//设置写入区域
		for(int i=0;i<font_length;i++){      //
			for(int j=0;j<8;j++){
				if((font[i]>>j)&0x001){
					Lcd_Write_Data(Font_color>>8);
					Lcd_Write_Data(Font_color);
				}else{
					Lcd_Write_Data(Brackground_color>>8);
					Lcd_Write_Data(Brackground_color);
				}
			}
		}
}
//输出连续汉字
//parm
//@ x,y 文字显示的起始位置
//@ word_times 要显示的文字数
//@ word_start 从font.h找开始显示的文字
//@ Font_color 字体颜色
//@ Brackground_color 字体背景色
void Lcd_DrawWrods(uint16_t x,uint16_t y,uint8_t word_times,uint8_t word_start,uint16_t Font_color,uint16_t Brackground_color){
	 
	Lcd_Set_Address(x,y,x+16-1,y+16-1);//设置写入区域
		for(int i=word_start;i<word_start+2;i++){
		for(int j=0;j<16;j++){
			for(int q=0;q<8;q++){
				if((font[i][j]>>q)&0x001){
					Lcd_Write_Data(Font_color>>8);
					Lcd_Write_Data(Font_color);
				}else{
					Lcd_Write_Data(Brackground_color>>8);
					Lcd_Write_Data(Brackground_color);
				}
			}
		}
		}
		word_times--;
		if(word_times>0){
		Lcd_DrawWrods(x+16,y,word_times,word_start+2,Font_color,Brackground_color);
		}
}
//图片显示函数  16位色彩度
void Lcd_disPlayPicture(uint16_t x,uint16_t y){
Lcd_Set_Address(x,y,x+40-1,y+45-1);//设置写入区域
	for(int i=0;i<sizeof(gImage_33);i++){
	Lcd_Write_Data(gImage_33[i]);
	}
}

void Lcd_DrawNums(uint16_t x,uint16_t y,uint16_t nums,uint16_t Font_color,uint16_t Brackground_color){
		Lcd_Set_Address(x,y,x+16-1,y+32-1);//设置写入区域
			for(int i=(nums*4);i<4+(nums*4);i++){
		for(int j=0;j<16;j++){
			for(int q=0;q<8;q++){
				if((num[i][j]>>q)&0x001){
					Lcd_Write_Data(Font_color>>8);
					Lcd_Write_Data(Font_color);
				}else{
					Lcd_Write_Data(Brackground_color>>8);
					Lcd_Write_Data(Brackground_color);
				}
			}
		}
		}
}
void Lcd_time(uint8_t h,uint8_t m,uint8_t s,uint16_t x,uint16_t y){
	Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m高位
	Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m低位
	Lcd_DrawNums(x+32,y,10,0x0001,0xffff);//：
	Lcd_DrawNums(x   ,y,(((h-(h%10)))/10),0x0001,0xffff);//h高位
	Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h低位
while(1){
	//时间计算函数
	HAL_Delay(1000);
	s++;
	if(s>59){
	s=0;
	m++; //每当分钟加1时刷新分钟
		//时间显示函数
		if(m>=10){
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m低位
						Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m高位
						}else{
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m低位	
						Lcd_DrawNums(x+48,y,0,0x0001,0xffff); //m高位
						}
	if(m>59){
		m=0;
		h++;
		if(h>24){
		h=0;
		}
		//每当时钟加1时刷新分钟
			//时间显示函数
			if(h>=10){
					Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h低位
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h高位
					
					}else{
						Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h低位
						Lcd_DrawNums(x+0,y,0,0x0001,0xffff);//h高位
						
							}
		
	}
}
if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)==RESET){
	uint8_t stats=0;
	//stats //状态标记
	while(1){
		if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)==RESET){
		stats++;
			if(stats>5){
				stats=0;
			}//状态清零
		}
		switch (stats){
			case 1:{//分钟低位
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					//确定按下数值加1
					if(m>=59){
					m=0;
					}else{
						m++;
					}
				}//数值刷新
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h高位
					Lcd_DrawNums(x+64,y,(m%10),0xffff,0x0001); // m低位	
					Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m高位
				//跳出switch
				//并未跳出while 由于stats还在该状态位 可以一直更改时间
				//直到跳出while函数（跳出while循环函数在下面）
				//或者stats状态位改变
				break;
			}
			case 2:{//分钟高位
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					
					m+=10;
					if(m>=59){
					m=0;
					}
				}
				Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m低位	
				Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0xffff,0x0001); //m高位
				break;
			}
			case 3:{////时钟低位
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					if(h>=24){
					h=0;
					}else{
					h++;
					}
				}
				Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m高位
				Lcd_DrawNums(x+16,y,(h%10),0xffff,0x0001);//h低位
				Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h高位
				break;
			}
			case 4:{//时钟高位
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					h+=10;
					if(h>24){
					h=0;
					}
				}
					Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h低位
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0xffff,0x0001);//h高位
					
				break;
			}
			case 5:{
				//图形复位
				Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h高位
				//亮度汉字显示（阴码）
				Lcd_DrawWrods(5,0,2,0,0xffff,0x0001);
				//亮度值
				uint8_t b=10;
				//bool st= true;
				while(true){
					//死循环
					if(b>200){
					b=0;
					}//亮度值恢复
					if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
						HAL_Delay(100);//消抖
						if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
							__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,b);
							b+=10;
						}
				}
					if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)==RESET){
						HAL_Delay(100);//消抖
						if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)==RESET){
							//汉字显示改变（阳码）
						Lcd_DrawWrods(5,0,2,0,0x0001,0xffff);
						HAL_Delay(200);
							//汉字清除
						Lcd_DrawWrods(5,0,2,0,0xffff,0xffff);
							//st=false;
							//确定后跳出死循环
						break;
					}}
				}
				//switch 结束
				break;
			}	
		}
		
	
		
		if(1){
		if(stats==5){  
		break;
		}
					//时间确定按钮执行函数
					//时间确定后刷新时间
					//跳出循环
			if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)==RESET){
				s=0;//秒清零
				if(m>=10){
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m低位
						Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m高位
						}else{
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m低位	
						Lcd_DrawNums(x+48,y,0,0x0001,0xffff); //m高位
						}
					//时间显示函数
			if(h>=10){
					Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h低位
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h高位
					}else{
						Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h低位
						Lcd_DrawNums(x+0,y,0,0x0001,0xffff);//h高位
						
							}
				break;	
			}
		
		}

		
	}
}

}
}

//屏幕初始化函数
void ST7735_Init(void){
 SPI_GPIO();
	RES_L;	
	CS_H;
	DC_H;
//	BL_L;
	RES_L;
	HAL_Delay(100);
	RES_H;
//	BL_H;	
	HAL_Delay(120);
	Lcd_Write_Cmd(0x11);
	HAL_Delay(120);
	Lcd_Write_Cmd(0x20);//lcd_inv_off如果颜色错了就用0x21
  HAL_Delay(5);
    Lcd_Write_Cmd(0xB1);
    Lcd_Write_Data(0x05);
    Lcd_Write_Data(0x3A);
    Lcd_Write_Data(0x3A);

    Lcd_Write_Cmd(0xB2);
    Lcd_Write_Data(0x05);
    Lcd_Write_Data(0x3A);
    Lcd_Write_Data(0x3A);

    Lcd_Write_Cmd(0xB3);
    Lcd_Write_Data(0x05);
    Lcd_Write_Data(0x3A);
    Lcd_Write_Data(0x3A);
    Lcd_Write_Data(0x05);
    Lcd_Write_Data(0x3A);
    Lcd_Write_Data(0x3A);

    Lcd_Write_Cmd(0xB4);//Dotinversion
    Lcd_Write_Data(0x03);

    Lcd_Write_Cmd(0xC0);
    Lcd_Write_Data(0x62);
    Lcd_Write_Data(0x02);
    Lcd_Write_Data(0x04);

    Lcd_Write_Cmd(0xC1);
    Lcd_Write_Data(0xC0);

    Lcd_Write_Cmd(0xC2);
    Lcd_Write_Data(0x0D);
    Lcd_Write_Data(0x00);

    Lcd_Write_Cmd(0xC3);
    Lcd_Write_Data(0x8D);
    Lcd_Write_Data(0x6A);

    Lcd_Write_Cmd(0xC4);
    Lcd_Write_Data(0x8D);
    Lcd_Write_Data(0xEE);

    Lcd_Write_Cmd(0xC5);//VCOM
    Lcd_Write_Data(0x0E);

    Lcd_Write_Cmd(0xE0);
    Lcd_Write_Data(0x10);
    Lcd_Write_Data(0x0E);
    Lcd_Write_Data(0x02);
    Lcd_Write_Data(0x03);
    Lcd_Write_Data(0x0E);
    Lcd_Write_Data(0x07);
    Lcd_Write_Data(0x02);
    Lcd_Write_Data(0x07);
    Lcd_Write_Data(0x0A);
    Lcd_Write_Data(0x12);
    Lcd_Write_Data(0x27);
    Lcd_Write_Data(0x37);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x0D);
    Lcd_Write_Data(0x0E);
    Lcd_Write_Data(0x10);

    Lcd_Write_Cmd(0xE1);
    Lcd_Write_Data(0x10);
    Lcd_Write_Data(0x0E);
    Lcd_Write_Data(0x03);
    Lcd_Write_Data(0x03);
    Lcd_Write_Data(0x0F);
    Lcd_Write_Data(0x06);
    Lcd_Write_Data(0x02);
    Lcd_Write_Data(0x08);
    Lcd_Write_Data(0x0A);
    Lcd_Write_Data(0x13);
    Lcd_Write_Data(0x26);
    Lcd_Write_Data(0x36);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x0D);
    Lcd_Write_Data(0x0E);
    Lcd_Write_Data(0x10);

    Lcd_Write_Cmd(0x3A);
    Lcd_Write_Data(0x05);

    //direction
    Lcd_Write_Cmd(0x36);
    Lcd_Write_Data(0x78);

    Lcd_Write_Cmd(0x29);
		HAL_Delay(100);
		CS_L;
    Lcd_Fill(0xffff);
//	  BL_H;



}



