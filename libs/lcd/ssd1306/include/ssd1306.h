#ifndef __SSD1306_H
#define __SSD1306_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "stdbool.h"
#include "api_hal_gpio.h"
#include "api_hal_spi.h"


#define SSD1306_CMD  0	//写命令
#define SSD1306_DATA 1	//写数据

/**
  * Initialize SSD1306
  * 
  * @param spiN: spi number
  * @param spiConfig: spi config
  * @return bool: return true if init success or false
  */			    
bool SSD1306_Init(uint16_t width, uint16_t height, SPI_ID_t spiN, SPI_Config_t* spiConfig, 
                     GPIO_PIN rst, GPIO_PIN dc);

/**
  * set data ponter
  * @param x: start position,from 0 to lcd width (bit)-1
  * @param y: start position virtical pages: from 0 to (lcd height/8-1)
  *           e.g. 128x64:0~(64/8-1)==> 0~7
 */
void SSD1306_SetPos(unsigned char x, unsigned char y);


/**
  * 
  * write a byte to ssd1306
  * 
  */
void SSD1306_WriteByte(uint8_t data, uint8_t cmd);

void SSD1306_DisplayOn(void);
void SSD1306_DisplayOff(void);

/**
  * clear screen, then screen display nothing, 
  * just like after call SSD1306_DisplayOff
  * 
 */
void SSD1306_Clear(void);

/**
  * 
  * @param x: start position,from 0 to lcd width (bit)-1
  * @param y: start position virtical pages: from 0 to (lcd height/8-1)
  *           e.g. 128x64:0~(64/8-1)==> 0~7
  * @param chr: ascii to display
  * @param size: font size, 8 or 16
  */
void SSD1306_ShowChar(uint8_t x,uint8_t y,uint8_t chr, uint8_t size);

/**
  * display string
  * 
  * @param x: start position,from 0 to lcd width (bit)-1
  * @param y: start position virtical pages: from 0 to (lcd height/8-1)
  *           e.g. 128x64:0~(64/8-1)==> 0~7
  * @param size: font size, 8 or 16
  */
void SSD1306_ShowString(uint8_t x,uint8_t y,uint8_t *chr, uint8_t size);

/**
  * display BMP picture
  * 
  * @param x0,x1: BMP pic horizon bits, value from 0 to 127 in 128x64 display module
  *               e.g.: 48x48(x*y)(horizon*virtival) --> x0,x1 = 0,47
  * @param x1,y1: BMP pic virtical pages(every 8 bits one page), value from 0 to 7 in 128x64 display module
  *               e.g.: 48x48(x*y)(horizon*virtival) --> {y0,y1 = 0,(48/8-1) ==> 0,5}
  * @param bmp: BMP picture.
  *             e.g. 16x8 picture, `x` means lcd bit on, `-` means lcd bit off
  *                  x - x x - - - - - - - - - - - - x
  *                  x - x x - - - - - - - - - - - - x
  *                  x - - - - - - - - - - - - - - - x
  *                  x - - - - - - - - - - - - - - - x
  *                  x - - - - - - - - - - - - - - - x
  *                  x - - - - - - - - - - - - - - - x
  *                  x - - - - - - - - - - - - - - - x
  *                  x - - - - - - - - - - - - - - - x
  *        the array should be bmp[]={
  *             0xff,0x00,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff
  *           }
  */
void SSD1306_DrawBMP(unsigned char x0, unsigned char x1, unsigned char y0, unsigned char y1,unsigned char* bmp );


/**
  * 
  * display flush all screen
  * 
  * @param data: data to display, length: lcd width*height/8
  *                                      e.g. 128x64: 128*64/8 
  */
void SSD1306_Show(uint8_t* data);




#ifdef __cplusplus
}
#endif

#endif

