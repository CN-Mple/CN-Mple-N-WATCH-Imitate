#ifndef __BSP_MPU6050_H
#define __BSP_MPU6050_H

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_time.h"
#include "bsp_SysTick.h"

#include "math.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "BREATHSCREEN.H"
#include "MENU.H"

#define Sensors_I2C_WriteRegister   Write_MPU6050
#define Sensors_I2C_ReadRegister    Read_MPU6050
#define get_ms      get_tick_count


#define MPU6050_GPIO_PORT_I2C		GPIOB			/* GPIO端口 */
#define MPU6050_RCC_I2C_PORT 		RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define MPU6050_I2C_SCL_PIN			GPIO_Pin_2			/* 连接到SCL时钟线的GPIO */
#define MPU6050_I2C_SDA_PIN			GPIO_Pin_1			/* 连接到SDA数据线的GPIO */


#define MPU6050_I2C_SCL_1()					GPIO_SetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SCL_PIN)
#define MPU6050_I2C_SCL_0()					GPIO_ResetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SCL_PIN)

#define MPU6050_I2C_SDA_1()					GPIO_SetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SDA_PIN)
#define MPU6050_I2C_SDA_0()					GPIO_ResetBits(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SDA_PIN)

#define MPU6050_I2C_SDA_Read()				GPIO_ReadInputDataBit(MPU6050_GPIO_PORT_I2C, MPU6050_I2C_SDA_PIN)

#define MPU6050_I2C_SDA_OUT()				{GPIOB->CRL &= ( ~(0x0000000F << 4) );GPIOB->CRL |= 0x00000003 << 4;}
#define MPU6050_I2C_SDA_IN()				{GPIOB->CRL &= ( ~(0x0000000F << 4) );GPIOB->CRL |= 0x00000004 << 4;}


/***/
#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG			0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG			0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG			0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG			0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
/***/

#define SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define MPU6050_ACC_OUT         0x3B     //MPU6050加速度数据寄存器地址
#define MPU6050_RA_TEMP_OUT_H   0x41	 //MPU6050 温度 数据寄存器地址
#define MPU6050_GYRO_OUT        0x43     //MPU6050陀螺仪数据寄存器地址

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

#define ADDRESS_MPU6050	0x69	//IIC写入时的地址字节数据，+1为读取
#define IICSPEED     	0x24

//#define MPU_FIFO_EN_REG 0x23

/*信息输出*/
#define MPU_DEBUG_ON         0
#define MPU_DEBUG_FUNC_ON    0

#define MPU_INFO(fmt,arg...)           printf("<<-MPU-INFO->> "fmt"\n",##arg)
#define MPU_ERROR(fmt,arg...)          printf("<<-MPU-ERROR->> "fmt"\n",##arg)
#define MPU_DEBUG(fmt,arg...)          do{\
                                          if(MPU_DEBUG_ON)\
                                          printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
#define MPU_DEBUG_FUNC()               do{\
                                         if(MPU_DEBUG_FUNC_ON)\
                                         printf("<<-MPU-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)

									   

									   
									   
void MPU6050_GPIO_Config(void);
void MPU6050_Init(void);
									   
uint8_t Write_MPU6050(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t const *data);
uint8_t Read_MPU6050(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t *data);

uint8_t MPU6050ReadID(void);
									   
void MPU6050ReadAcc(short *accData);
void MPU6050ReadGyro(short *gyroData);
void MPU6050ReadTemp(short *tempData);
void MPU6050_ReturnTemp(float *Temperature);

void MPU6050_updata(void);
									   
//dmp
uint8_t mpu_dmp_init(void);
uint8_t Read_from_mpl(float *Pitch, float *roll, float *yaw);
void mpl_updata(void);
#endif /* __BSP_MPU6050_H */
