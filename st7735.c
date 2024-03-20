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
//BL������Pwm����
//#define BL_L HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)
//#define BL_H HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)

//screen size
#define LCD_W 160
#define LCD_H 80

//offset
//��ʾƫ��
#define LCD_OFFSET_X 1
#define LCD_OFFSET_Y 26
//gpio init
//spi���io��ʼ������
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
	
//spi���ݷ���
void SPI_Write_Data(uint8_t data){
		CS_L;
	//  spi ����ӿ�
		HAL_SPI_Transmit(&Spi1Handle,&data,1,1000);
		CS_H;
}
//����ͺ���
void Lcd_Write_Cmd(uint8_t cmd){
	DC_L;
	SPI_Write_Data(cmd);
	DC_H;
}
//����д�뺯��
void Lcd_Write_Data(uint8_t data){
	
	SPI_Write_Data(data);
	
}
void Lcd_Write_Data16(uint16_t data){
	SPI_Write_Data(data>>8);//��д���8λ����
	SPI_Write_Data(data);//д���8λ����
}
void Lcd_Set_Address(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	Lcd_Write_Cmd(0x2a);//x���д�뷶Χ
	Lcd_Write_Data((x1+LCD_OFFSET_X)>>8);//��д���8λ
	Lcd_Write_Data(x1+LCD_OFFSET_X);     //д���8λ
	Lcd_Write_Data((x2+LCD_OFFSET_X)>>8);
	Lcd_Write_Data(x2+LCD_OFFSET_X);
	Lcd_Write_Cmd(0x2b);//y���д�뷶Χ
	Lcd_Write_Data((y1+LCD_OFFSET_Y)>>8);
	Lcd_Write_Data(y1+LCD_OFFSET_Y);
	Lcd_Write_Data((y2+LCD_OFFSET_Y)>>8);
	Lcd_Write_Data(y2+LCD_OFFSET_Y);
	Lcd_Write_Cmd(0x2c);//д���������
}
//��Ļ��亯��
void Lcd_Fill(uint16_t color){
	uint32_t i;
	//Ƭѡ�ź�
	CS_L;
	Lcd_Set_Address(0,0,LCD_W-1,LCD_H-1);
	for(i = 0; i < LCD_W * LCD_H; i++)
	{ 
		//ÿ�����ص�Ϊ16λRGB
		//���͸�8λ
		Lcd_Write_Data(color>>8);
		//���͵�8λ
		Lcd_Write_Data(color);
	}
	CS_H;
}
//���㺯��
void Lcd_DrawPoint(uint16_t x,uint16_t y,uint16_t color){
	Lcd_Set_Address(x-1,y-1,x,y);
	Lcd_Write_Data(color>>8);
	Lcd_Write_Data(color);	
}
//�����������
//parm
//@ x,y ������ʾ����ʼλ��
//@ Font_color ������ɫ
//@ Brackground_color ���屳��ɫ
void Lcd_DrawWord(uint16_t x,uint16_t y,uint16_t Font_color,uint16_t Brackground_color,uint8_t *font,uint8_t font_length){
	
    Lcd_Set_Address(x,y,x+(font_length/2)-1,y+(font_length/2)-1);//����д������
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
//�����������
//parm
//@ x,y ������ʾ����ʼλ��
//@ word_times Ҫ��ʾ��������
//@ word_start ��font.h�ҿ�ʼ��ʾ������
//@ Font_color ������ɫ
//@ Brackground_color ���屳��ɫ
void Lcd_DrawWrods(uint16_t x,uint16_t y,uint8_t word_times,uint8_t word_start,uint16_t Font_color,uint16_t Brackground_color){
	 
	Lcd_Set_Address(x,y,x+16-1,y+16-1);//����д������
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
//ͼƬ��ʾ����  16λɫ�ʶ�
void Lcd_disPlayPicture(uint16_t x,uint16_t y){
Lcd_Set_Address(x,y,x+40-1,y+45-1);//����д������
	for(int i=0;i<sizeof(gImage_33);i++){
	Lcd_Write_Data(gImage_33[i]);
	}
}

void Lcd_DrawNums(uint16_t x,uint16_t y,uint16_t nums,uint16_t Font_color,uint16_t Brackground_color){
		Lcd_Set_Address(x,y,x+16-1,y+32-1);//����д������
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
	Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m��λ
	Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m��λ
	Lcd_DrawNums(x+32,y,10,0x0001,0xffff);//��
	Lcd_DrawNums(x   ,y,(((h-(h%10)))/10),0x0001,0xffff);//h��λ
	Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h��λ
while(1){
	//ʱ����㺯��
	HAL_Delay(1000);
	s++;
	if(s>59){
	s=0;
	m++; //ÿ�����Ӽ�1ʱˢ�·���
		//ʱ����ʾ����
		if(m>=10){
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m��λ
						Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m��λ
						}else{
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m��λ	
						Lcd_DrawNums(x+48,y,0,0x0001,0xffff); //m��λ
						}
	if(m>59){
		m=0;
		h++;
		if(h>24){
		h=0;
		}
		//ÿ��ʱ�Ӽ�1ʱˢ�·���
			//ʱ����ʾ����
			if(h>=10){
					Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h��λ
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h��λ
					
					}else{
						Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h��λ
						Lcd_DrawNums(x+0,y,0,0x0001,0xffff);//h��λ
						
							}
		
	}
}
if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)==RESET){
	uint8_t stats=0;
	//stats //״̬���
	while(1){
		if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)==RESET){
		stats++;
			if(stats>5){
				stats=0;
			}//״̬����
		}
		switch (stats){
			case 1:{//���ӵ�λ
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					//ȷ��������ֵ��1
					if(m>=59){
					m=0;
					}else{
						m++;
					}
				}//��ֵˢ��
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h��λ
					Lcd_DrawNums(x+64,y,(m%10),0xffff,0x0001); // m��λ	
					Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m��λ
				//����switch
				//��δ����while ����stats���ڸ�״̬λ ����һֱ����ʱ��
				//ֱ������while����������whileѭ�����������棩
				//����stats״̬λ�ı�
				break;
			}
			case 2:{//���Ӹ�λ
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					
					m+=10;
					if(m>=59){
					m=0;
					}
				}
				Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m��λ	
				Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0xffff,0x0001); //m��λ
				break;
			}
			case 3:{////ʱ�ӵ�λ
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					if(h>=24){
					h=0;
					}else{
					h++;
					}
				}
				Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m��λ
				Lcd_DrawNums(x+16,y,(h%10),0xffff,0x0001);//h��λ
				Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h��λ
				break;
			}
			case 4:{//ʱ�Ӹ�λ
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
					h+=10;
					if(h>24){
					h=0;
					}
				}
					Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h��λ
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0xffff,0x0001);//h��λ
					
				break;
			}
			case 5:{
				//ͼ�θ�λ
				Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h��λ
				//���Ⱥ�����ʾ�����룩
				Lcd_DrawWrods(5,0,2,0,0xffff,0x0001);
				//����ֵ
				uint8_t b=10;
				//bool st= true;
				while(true){
					//��ѭ��
					if(b>200){
					b=0;
					}//����ֵ�ָ�
					if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
						HAL_Delay(100);//����
						if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==RESET){
							__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,b);
							b+=10;
						}
				}
					if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)==RESET){
						HAL_Delay(100);//����
						if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)==RESET){
							//������ʾ�ı䣨���룩
						Lcd_DrawWrods(5,0,2,0,0x0001,0xffff);
						HAL_Delay(200);
							//�������
						Lcd_DrawWrods(5,0,2,0,0xffff,0xffff);
							//st=false;
							//ȷ����������ѭ��
						break;
					}}
				}
				//switch ����
				break;
			}	
		}
		
	
		
		if(1){
		if(stats==5){  
		break;
		}
					//ʱ��ȷ����ťִ�к���
					//ʱ��ȷ����ˢ��ʱ��
					//����ѭ��
			if(HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)==RESET){
				s=0;//������
				if(m>=10){
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m��λ
						Lcd_DrawNums(x+48,y,(((m-(m%10)))/10),0x0001,0xffff); //m��λ
						}else{
						Lcd_DrawNums(x+64,y,(m%10),0x0001,0xffff); // m��λ	
						Lcd_DrawNums(x+48,y,0,0x0001,0xffff); //m��λ
						}
					//ʱ����ʾ����
			if(h>=10){
					Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h��λ
					Lcd_DrawNums(x+0,y,(((h-(h%10)))/10),0x0001,0xffff);//h��λ
					}else{
						Lcd_DrawNums(x+16,y,(h%10),0x0001,0xffff);//h��λ
						Lcd_DrawNums(x+0,y,0,0x0001,0xffff);//h��λ
						
							}
				break;	
			}
		
		}

		
	}
}

}
}

//��Ļ��ʼ������
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
	Lcd_Write_Cmd(0x20);//lcd_inv_off�����ɫ���˾���0x21
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



