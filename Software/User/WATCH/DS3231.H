#ifndef __DS3231_H
#define __DS3231_H

#include "stm32f10x.h"

/* 定义DS3231总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_DS3231		GPIOA			/* GPIO端口 */
#define RCC_DS3231_PORT 		RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define DS3231_SCL_PIN			GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define DS3231_SDA_PIN			GPIO_Pin_7			/* 连接到SDA数据线的GPIO */


#define DS3231_SCL_1()					GPIO_SetBits(GPIO_PORT_DS3231, DS3231_SCL_PIN)
#define DS3231_SCL_0()					GPIO_ResetBits(GPIO_PORT_DS3231, DS3231_SCL_PIN)

#define DS3231_SDA_1()					GPIO_SetBits(GPIO_PORT_DS3231, DS3231_SDA_PIN)
#define DS3231_SDA_0()					GPIO_ResetBits(GPIO_PORT_DS3231, DS3231_SDA_PIN)

#define DS3231_SDA_Read()				GPIO_ReadInputDataBit(GPIO_PORT_DS3231, DS3231_SDA_PIN)


typedef struct
{
    uint8_t	hour;
    uint8_t	min;
    uint8_t	sec;            
    uint8_t	w_year;
    uint8_t	w_month;
    uint8_t	w_date;
    uint8_t	week;        
    uint8_t	temper_H;
    uint8_t	temper_L;
}Stand_Time;

uint8_t BCDtoHEX(uint8_t val);
uint16_t HEXtoBCD(uint8_t val);

void DS3231_Init(void);						//DS3231初始化 只初始化IIC 提前写入寄存器将导致紊乱

void DS3231_WR_Byte(uint8_t addr,uint8_t bytedata);
uint8_t DS3231_RD_Byte(uint8_t addr);

void Get_DS3231_Time(void);

#endif /* __DS3231_H */

