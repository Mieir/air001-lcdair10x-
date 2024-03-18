#ifndef __ST7735V_H
#define __ST7735V_H		
#include "air001xx.h"
#include "air001xx_hal.h"


void SPI_GPIO(void);
void ST7735_Init(void);
void Lcd_Fill(uint16_t color);
void Lcd_Set_Address(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void Lcd_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void Lcd_Write_Cmd(uint8_t cmd);
void SPI_Write_Data(uint8_t data);
void SPI_Write_Data16(uint8_t data);
void Lcd_DrawWord(uint16_t x,uint16_t y,uint16_t Font_color,uint16_t Brackground_color,uint8_t *font,uint8_t font_length);
void Lcd_DrawNums(uint16_t x,uint16_t y,uint16_t nums,uint16_t Font_color,uint16_t Brackground_color);
void Lcd_DrawWrods(uint16_t x,uint16_t y,uint8_t word_times,uint8_t word_start,uint16_t Font_color,uint16_t Brackground_color);
void Lcd_time(uint8_t h,uint8_t m,uint8_t s,uint16_t x,uint16_t y);
#endif

