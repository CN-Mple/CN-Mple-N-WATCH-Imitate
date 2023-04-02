#include "bsp_mpu6050.h"


static void Delay(void)
{
	uint8_t i;
	for(i = 0; i < 10; i++);
}


void MPU6050_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	/* 使能 I2C_GPIO 时钟 */
	RCC_APB2PeriphClockCmd(MPU6050_RCC_I2C_PORT, ENABLE);
	
	
	/* 配置SCL SDA引脚 输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MPU6050_I2C_SCL_PIN | MPU6050_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(MPU6050_GPIO_PORT_I2C, &GPIO_InitStructure);
	
	MPU6050_I2C_SCL_1();
	MPU6050_I2C_SDA_1();
	Delay();
	
}

static uint8_t MPU6050_I2C_Start(void)
{
	MPU6050_I2C_SDA_1();
	Delay();
	
	MPU6050_I2C_SCL_1();
	Delay();
	
	if(!MPU6050_I2C_SDA_Read()) return 1;//BUSY
	
	MPU6050_I2C_SDA_0();
	Delay();
	
	MPU6050_I2C_SCL_0();
	Delay();
	
	if(MPU6050_I2C_SDA_Read()) return 2;//ERROR
	
	return 0;
}

static void MPU6050_I2C_Stop(void)
{
	MPU6050_I2C_SDA_0();
	Delay();
	
	MPU6050_I2C_SCL_1();
	Delay();
	
	MPU6050_I2C_SDA_1();
	Delay();
}



static uint8_t MPU6050_I2C_WaitAck(void)
{
	uint8_t ucErrTime=0;

	MPU6050_I2C_SDA_1();	/* CPU释放SDA总线 */
	Delay();
	MPU6050_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	Delay();
//	while(MPU6050_I2C_SDA_Read())	/* CPU读取SDA口线状态 */
//	{
//		ucErrTime++;
//		if(ucErrTime > 250)
//		{
//			MPU6050_I2C_Stop();
//			return 2;
//		}
//	}
	
	MPU6050_I2C_SCL_0();
	Delay();
	return 0;
}

static void MPU6050_I2C_Send_Ack(uint8_t ACK)			//ACK = 1 表示应答可继续发送数据
{
	if (ACK)
	{
		MPU6050_I2C_SDA_0();
	}
	else
	{
		MPU6050_I2C_SDA_1();
	}
	Delay();
	MPU6050_I2C_SCL_1();
	Delay();
	MPU6050_I2C_SCL_0();
	Delay();
}

static uint8_t MPU6050_Send_Byte(uint8_t Data)
{
	uint8_t i = 0;
	MPU6050_I2C_SCL_0();			//拉低SCL
	for(i = 0; i < 8; i++)
	{
		if(Data & 0x80)
		{
			MPU6050_I2C_SDA_1();		//	SDA写1
		}
		else
		{
			MPU6050_I2C_SDA_0();		//	SDA写0
		}
		Data <<= 1;						//左移下一位
		Delay();
		MPU6050_I2C_SCL_1();			//拉高SCL
		Delay();
		MPU6050_I2C_SCL_0();			//拉高SCL
		Delay();
	}
	MPU6050_I2C_SCL_0();				//拉低SCL
	Delay();
	return MPU6050_I2C_WaitAck();
}

static uint8_t MPU6050_Read_Byte(void)
{
	uint8_t i = 0,Value = 0;
	MPU6050_I2C_SDA_1();				//	SDA写1
	MPU6050_I2C_SCL_0();				//拉低SCL
	
	for(i = 0; i < 8; i++)
	{
		MPU6050_I2C_SCL_1();
		Delay();
		Value <<= 1;
		if(MPU6050_I2C_SDA_Read())
		{
			Value|=0x01;
		}
		MPU6050_I2C_SCL_0();
		Delay();
	}
	MPU6050_I2C_Send_Ack(0);
	return Value;
}

static uint8_t MPU6050_Read_Byte_withACK(void)
{
	uint8_t i = 0,Value = 0;
	MPU6050_I2C_SDA_1();				//	SDA写1
	MPU6050_I2C_SCL_0();				//拉低SCL
	MPU6050_I2C_SDA_IN();
	for(i = 0; i < 8; i++)
	{
		MPU6050_I2C_SCL_1();
		Delay();
		Value <<= 1;
		if(MPU6050_I2C_SDA_Read())
		{
			Value|=0x01;
		}
		MPU6050_I2C_SCL_0();
		Delay();
	}
	MPU6050_I2C_SDA_OUT();
	MPU6050_I2C_Send_Ack(1);
	return Value;
}



uint8_t Write_MPU6050(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t const *buff)
{
	uint8_t i, result=0;
	
	result = MPU6050_I2C_Start();
	
	result = MPU6050_Send_Byte(( ( slave_addr << 1 ) | 0 ));
	if(result != 0) return result;

	result = MPU6050_Send_Byte(reg_addr);
	if(result != 0) return result;
	
	for(i = 0; i < length; i++)
	{
		result = MPU6050_Send_Byte(buff[i]);
		if(result != 0) return result;
	}

	MPU6050_I2C_Stop();
	return 0;
}

uint8_t Read_MPU6050(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t *buff)
{
	uint8_t result=0;

	result = MPU6050_I2C_Start();
	
	result = MPU6050_Send_Byte(( ( slave_addr << 1 ) | 0 ));
	if(result != 0) return result;

	result = MPU6050_Send_Byte(reg_addr);
	if(result != 0) return result;
	
	MPU6050_I2C_Start();

	result = MPU6050_Send_Byte(( ( slave_addr << 1 ) | 1 ));
	if(result != 0) return result;
	
	while(length)
	{
		if(length == 1)
		*buff = MPU6050_Read_Byte();
		else
		*buff = MPU6050_Read_Byte_withACK();
		buff++;
		length--;
	}
	
	MPU6050_I2C_Stop();
	return 0;
}

uint8_t const MPU_INIT1 = 0x00;
uint8_t const MPU_INIT2 = 0x07;
uint8_t const MPU_INIT3 = 0x06;
uint8_t const MPU_INIT4 = 0x18;
uint8_t const MPU_INIT5 = 0x00;

uint8_t const MPU_INIT6 = 0x00;
uint8_t const MPU_INIT7 = 0x00;
uint8_t const MPU_INIT8 = 0xFF;
uint8_t const MPU_INIT9 = 0x00;



void MPU6050_Init(void)
{
	MPU6050_GPIO_Config();
	Write_MPU6050(ADDRESS_MPU6050,PWR_MGMT_1,1,&MPU_INIT1);	//解除休眠状态
	Write_MPU6050(ADDRESS_MPU6050,SMPLRT_DIV,1,&MPU_INIT2);  //陀螺仪1Khz
	Write_MPU6050(ADDRESS_MPU6050,CONFIG,1,&MPU_INIT3);      //21HZ滤波 延时A8.5ms G8.3ms  此处取值应相当注意，延时与系统周期相近为宜
	Write_MPU6050(ADDRESS_MPU6050,GYRO_CONFIG,1,&MPU_INIT4); //陀螺仪500度/S 65.5LSB/g
	Write_MPU6050(ADDRESS_MPU6050,ACCEL_CONFIG,1,&MPU_INIT5);//加速度+-4g  8192LSB/g
	
	
	Write_MPU6050(ADDRESS_MPU6050,MPU_INT_EN_REG,1,&MPU_INIT6);		//关闭所有中断
	Write_MPU6050(ADDRESS_MPU6050,MPU_USER_CTRL_REG,1,&MPU_INIT7);	//I2C主模式关闭
	Write_MPU6050(ADDRESS_MPU6050,MPU_FIFO_EN_REG,1,&MPU_INIT8);	//FIFO

	MPU6050ReadID();
}


/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	uint8_t Re = 0;
    Read_MPU6050(ADDRESS_MPU6050,WHO_AM_I,1,&Re);    //读器件地址
	if(Re != 0x68)
	{
		Re = 1;
	}
	else
	{
		Re = 0;
	}
	return Re;	
}
/**
  * @brief   读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    Read_MPU6050(ADDRESS_MPU6050,MPU6050_ACC_OUT,6, buf);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    Read_MPU6050(ADDRESS_MPU6050,MPU6050_GYRO_OUT,6,buf);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}


/**
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    Read_MPU6050(ADDRESS_MPU6050,MPU6050_RA_TEMP_OUT_H,2,buf);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	Read_MPU6050(ADDRESS_MPU6050,MPU6050_RA_TEMP_OUT_H,2,buf);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}

//MPU6050上传  10ms
void MPU6050_updata(void)
{
	static uint32_t lastUpdate;
	static short Accel[3];
	static short Gyro[3];
	static float Temp;
	uint32_t now = millis();
	if ((uint32_t)(now - lastUpdate) >= 30)
	{
		lastUpdate = now;

		MPU6050ReadAcc(Accel);			

		MPU6050ReadGyro(Gyro);

		MPU6050_ReturnTemp(&Temp); 
	}
}

//以下为dmp初始化函数

//方向转换
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

//陀螺仪方向控制
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar; 
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

//MPU6050自测试
//返回值:0,正常
//    其他,失败
u8 run_self_test(void)
{
	int result;
	//char test_packet[4] = {0};
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) 
	{
		/* Test passed. We can trust the gyro data here, so let's push it down
		* to the DMP.
		*/
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		return 0;
	}else return 1;
}

//陀螺仪方向设置
static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};

//mpu6050,dmp初始化
//返回值:0,正常
//    其他,失败
uint8_t mpu_dmp_init(void)
{
	uint8_t res=0;
	struct int_param_s int_param;
	
	if(mpu_init(&int_param)==0)	//初始化MPU6050
	{	 
		
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//设置所需要的传感器
		if(res)return 1; 
		res=mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);//设置FIFO
		if(res)return 2; 
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//设置采样率
		if(res)return 3; 
		res=dmp_load_motion_driver_firmware();		//加载dmp固件
		if(res)return 4; 
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//设置陀螺仪方向
		if(res)return 5; 
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//设置dmp功能
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res)return 6; 
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//设置DMP输出速率(最大不超过200Hz)
		if(res)return 7;   
		res=run_self_test();		//自检
		if(res)return 8; 
		res=mpu_set_dmp_state(1);	//使能DMP
		if(res)return 9;     
	}else return 10;
	return 0;
}

#define q30	1073741824.0f

uint8_t Read_from_mpl(float *Pitch, float *roll, float *yaw)
{
    float q0 = 1.0f,q1 = 0.0f,q2 = 0.0f,q3 = 0.0f;
	
	short gyro[3];short accel[3];
	
	unsigned long timestamp;
	short sensors;
	uint8_t more;
	
	long quat[4];
	
	if(dmp_read_fifo(gyro, accel,quat,&timestamp,&sensors,&more))return 1;
	
	if(sensors & INV_WXYZ_QUAT)
	{
		q0 = quat[0] / q30;
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;
		
		*roll = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;
		*Pitch = atan2(-2 * q2 * q3 + 2 * q0 * q1,-2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;
		*yaw =  atan2(-2 *(q1 * q2 + q0 * q3),q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;
	}
	else return 2;
	
	return 0;
}

/******状态机相关******/
typedef enum
{
	mpu_STATE_RELEASE  = 0,		//释放
	mpu_STATE_WAITING,			//按下
	mpu_STATE_PRESSED,			//按下
}enum_mpuState;

#define PRESSED 				1
#define NOT_PRESSED				0


float Pitch;float roll;float yaw;

void mpl_updata(void)
{
	static uint32_t lastUpdate;
	
	static uint32_t time = 0;
	static enum_mpuState mpu_state = mpu_STATE_RELEASE;
	uint8_t detectResult = NOT_PRESSED;
	
	uint32_t now = millis();
	if ((uint32_t)(now - lastUpdate) >= 100)		//100ms测角
	{
		lastUpdate = now;
//		 mpu_reset_fifo();
		switch(mpu_state)
		{
			case mpu_STATE_RELEASE:
				if((roll - 70.0f) > 0)
				{
					mpu_state = mpu_STATE_WAITING;
					detectResult = NOT_PRESSED;
				}
				else
				{
					mpu_state = mpu_STATE_RELEASE;
					detectResult = NOT_PRESSED;
				}
				break;
			case mpu_STATE_WAITING:
				if((roll - 70.0f) > 0)
				{
					time++;
					if(time > 3)
					{
						time = 0;
						mpu_state = mpu_STATE_PRESSED;
						detectResult = NOT_PRESSED;
					}
					else
					{
						mpu_state = mpu_STATE_WAITING;
						detectResult = NOT_PRESSED;
					}
				}
	//			else
	//			{
	//				time = 0;
	//				mpu_state = mpu_STATE_RELEASE;
	//				detectResult = NOT_PRESSED;
	//			}
				break;
			case mpu_STATE_PRESSED:
				if((0.0f - roll) > 0)
				{
					mpu_state = mpu_STATE_PRESSED;
					detectResult = PRESSED;
				}
				else
				{
	//				mpu_state = mpu_STATE_RELEASE;
					detectResult = NOT_PRESSED;
				}
				break;
			default:
					mpu_state = mpu_STATE_RELEASE;
					detectResult = NOT_PRESSED;
				break;
		}
		if(detectResult)
		{
			mpu_state = mpu_STATE_RELEASE;
			detectResult = NOT_PRESSED;
			Screen_Open();
		}
	}
}
