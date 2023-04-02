#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_Font.h"
#include "ANIMOTION.H"

extern uint8_t Grame[8][128];
extern uint8_t MOD;

#define WHITH_MAX	128
#define HEIGHTH_MAX	64


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C		GPIOB			/* GPIO端口 */
#define RCC_I2C_PORT 		RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN			GPIO_Pin_9			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN			GPIO_Pin_8			/* 连接到SDA数据线的GPIO */


#define I2C_SCL_1()					GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)
#define I2C_SCL_0()					GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)

#define I2C_SDA_1()					GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)
#define I2C_SDA_0()					GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)

#define I2C_SDA_Read()				GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)

/*--------------------------------------------------------------------------------------------*/
void IIC_GPIO_Config(void);
/*--------------------------------------------------------------------------------------------*/
void I2C_Start(void);
void I2C_Stop(void);

void Send_Byte(uint8_t Data);
uint8_t Read_Byte(void);

uint8_t I2C_WaitAck(void);
void I2C_Send_Ack(uint8_t ACK);

void OLED_CMD(uint8_t cmd);
void OLED_Init(void);

/*--------------------------------------------------------------------------------------------*/
void Set_Pos(uint8_t x, uint8_t y);
void OLED_Frame_Fast(uint8_t Data[]);
/*--------------------------------------------------------------------------------------------*/
void OLED_ReFresh(uint8_t MOD);
void Fill_All(uint8_t fill_Data);
/*--------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------*/
void OLED_flash_8_Fast(uint8_t x,int y,uint8_t n,char offsetY);
uint8_t OLED_flash_Show_Char_8(uint8_t x,int y,uint8_t C,char offsetY);
void OLED_flash_String_8(uint8_t x,int y,const char *C,char offsetY);
/*--------------------------------------------------------------------------------------------*/
void OLED_flash_8_16_Num_Fast(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][16]);
uint8_t OLED_flash_Show_Char_16(uint8_t x,uint8_t y,uint8_t C);
void OLED_flash_Show_String_16(uint8_t x,uint8_t y,char *C);
/*--------------------------------------------------------------------------------------------*/
uint8_t OLED_flash_APPlication(uint8_t x,uint8_t y,uint8_t n,uint8_t A[][128]);
void Select_Windows(void);
/*--------------------------------------------------------------------------------------------*/

uint8_t Draw_pot(uint8_t x,uint8_t y);
uint8_t Clear_pot(uint8_t x,uint8_t y);
/*--------------------------------------------------------------------------------------------*/
uint8_t Draw_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end);
uint8_t Clear_line(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end);
/*--------------------------------------------------------------------------------------------*/
void Draw_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t weighth);
void Clear_square(uint8_t x_start,uint8_t y_start,uint8_t length,uint8_t weighth);
/*--------------------------------------------------------------------------------------------*/

void Draw_bmp(uint8_t x,int y,const uint8_t *bmp,uint8_t w,uint8_t h,char offsetY);

#define Draw_icon(x,y,bmp)	Draw_bmp(x,y,bmp,32,32,0)
#define Draw_Str(x,y,str)	OLED_flash_String_8(x,y,str,0)


#endif /* __BSP_OLED_H */

