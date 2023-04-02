#include "DS3231.H"
#include "WATCH.H"

static void DS3231_Start(void);
static void DS3231_Stop(void);

static void Send_Byte_DS3231M(uint8_t Data);
static uint8_t Read_Byte_DS3231M(void);

static uint8_t DS3231_WaitAck(void);
static void DS3231_Send_Ack(uint8_t ACK);

Stand_Time calendar = {0};


static void DS3231_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	/* 使能 DS3231_GPIO 时钟 */
	RCC_APB2PeriphClockCmd(RCC_DS3231_PORT, ENABLE);
	
	
	/* 配置SCL SDA引脚 od输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Pin = DS3231_SCL_PIN | DS3231_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIO_PORT_DS3231, &GPIO_InitStructure);
	

	DS3231_Stop();
}

static void Delay_DS3231(void)
{
	uint8_t i;
	for(i = 0; i < 10; i++);
}

static void DS3231_Start(void)
{
	DS3231_SDA_1();
	DS3231_SCL_1();
	Delay_DS3231();
	
	DS3231_SDA_0();
	Delay_DS3231();
	DS3231_SCL_0();
	Delay_DS3231();
}

static void DS3231_Stop(void)
{
	DS3231_SDA_0();
	DS3231_SCL_1();
	Delay_DS3231();
	
	DS3231_SDA_1();
}

static void Send_Byte_DS3231M(uint8_t Data)
{
	uint8_t i = 0;
	for(i = 0; i < 8; i++)
	{
		if(Data & 0x80)
		{
			DS3231_SDA_1();		//	SDA写1
		}
		else
		{
			DS3231_SDA_0();		//	SDA写0
		}
		Data <<= 1;				//左移下一位
		DS3231_SCL_1();			//拉高SCL
		Delay_DS3231();
		DS3231_SCL_0();			//拉低SCL
	}
	DS3231_SCL_0();			//拉低SCL
}

static uint8_t Read_Byte_DS3231M(void)
{
	uint8_t i = 0,Data=0x00;
	
	DS3231_SDA_1();
	
 	for (i=0;i<8;i++) 	
 	{	
		Data<<=1;
		DS3231_SCL_0();			//拉低SCL
		Delay_DS3231();
		
		DS3231_SCL_1();			//拉高SCL
		Delay_DS3231();
		
 		if(DS3231_SDA_Read())
		{
			Data|=0x01;
		}
	} 
	DS3231_SCL_0();			//拉低SCL
	return Data;
}


static uint8_t DS3231_WaitAck(void)
{
	uint8_t re;

	DS3231_SDA_1();	/* CPU释放SDA总线 */
	Delay_DS3231();
	DS3231_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	Delay_DS3231();
	if (DS3231_SDA_Read())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	DS3231_SCL_0();
	Delay_DS3231();
	return re;
}

static void DS3231_Send_Ack(uint8_t ACK)			//ACK = 1 表示应答可继续发送数据
{
	DS3231_SCL_0();
	if (ACK)
	{
		DS3231_SDA_0();
	}
	else
	{
		DS3231_SDA_1();
	}
	Delay_DS3231();
	DS3231_SCL_1();
	Delay_DS3231();
	DS3231_SCL_0();
	DS3231_SDA_1();
	Delay_DS3231();
}

#define DS3231_WriteAddress        0xD0  
#define DS3231_ReadAddress         0xD1

void DS3231_WR_Byte(uint8_t addr,uint8_t bytedata)
{
    DS3231_Start();
	
    Send_Byte_DS3231M(DS3231_WriteAddress);
    DS3231_WaitAck();
	
    Send_Byte_DS3231M(addr);
    DS3231_WaitAck();
	
    Send_Byte_DS3231M(bytedata);
    DS3231_WaitAck();
	
    DS3231_Stop();
}    
uint8_t DS3231_RD_Byte(uint8_t addr)
{
    uint8_t Dat=0;
  
    DS3231_Start();
    Send_Byte_DS3231M(DS3231_WriteAddress);
    DS3231_WaitAck();
	
    Send_Byte_DS3231M(addr);
    DS3231_WaitAck();
	
    DS3231_Start();
    Send_Byte_DS3231M(DS3231_ReadAddress);
    DS3231_WaitAck();
	
    Dat = Read_Byte_DS3231M();
	DS3231_Send_Ack(0);
	
    DS3231_Stop();
    return Dat;
}

uint8_t BCDtoHEX(uint8_t val)
{
    uint8_t i;
    i= val&0x0F;
    val >>= 4;
    val &= 0x0F;
    val *= 10;
    i += val;
    
    return i;
}
uint16_t HEXtoBCD(uint8_t val)
{
    uint8_t i,j,k;
    i=val/10;
    j=val%10;
    k=j+(i<<4);
    return k;
}

void DS3231_Init(void)
{
    DS3231_GPIO_Config();
   	DS3231_WR_Byte(0x0e,0x1f);
    Delay_DS3231();
    DS3231_WR_Byte(0x0f,0x00);
    Delay_DS3231();
	DS3231_WR_Byte(0x0e,0x20);//用于打开温度转换标志

	
//	DS3231_WR_Byte(0x06,0x23);
//    Delay_DS3231();
//    DS3231_WR_Byte(0x05,0x03);
//    Delay_DS3231();
//	DS3231_WR_Byte(0x04,0x1);
//	Delay_DS3231();
//	
//	DS3231_WR_Byte(0x02,0x23);
//    Delay_DS3231();
//    DS3231_WR_Byte(0x01,0x41);
//    Delay_DS3231();
//	DS3231_WR_Byte(0x00,0x30);
//	Delay_DS3231();
//	DS3231_WR_Byte(0x03,0x02);
}


void Get_DS3231_Time(void)
{
	calendar.w_year = DS3231_RD_Byte(0x06);  
    timeDate.date.year = BCDtoHEX(calendar.w_year)%99;
    calendar.w_month = DS3231_RD_Byte(0x05);
    timeDate.date.month = BCDtoHEX(calendar.w_month)%12;
    calendar.w_date = DS3231_RD_Byte(0x04);  
    timeDate.date.date = BCDtoHEX(calendar.w_date)%31;
    
    calendar.hour = DS3231_RD_Byte(0x02);                
    timeDate.time.hour = BCDtoHEX(calendar.hour)%24;
    calendar.min = DS3231_RD_Byte(0x01);
    timeDate.time.mins = BCDtoHEX(calendar.min)%60;
    calendar.sec = DS3231_RD_Byte(0x00);
    timeDate.time.secs = BCDtoHEX(calendar.sec)%60;
    
//    calendar.week = DS3231_RD_Byte(0x03);
//    timeDate.date.day = BCDtoHEX(calendar.week)%99;
	
//	calendar.temper_H = DS3231_RD_Byte(0x11);

//	
//	calendar.temper_L = DS3231_RD_Byte(0x12);

}

