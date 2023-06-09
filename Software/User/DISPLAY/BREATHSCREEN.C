#include "BREATHSCREEN.H"



User_t User = {0,0};						//Ĭ�Ϲر�״̬

static uint8_t High_Length = 32;			//����߶�
static uint8_t Wigh_Length = 0;				//���� �߿���
static uint8_t active = 2;					//��ֹ״̬

static void Clear_screenLine(uint8_t x)		//��������
{
	uint8_t y,xx1,xx2;
	
	xx1 = x / 8;
	xx2 = x % 8;
	for(y = 0; y < 127; y++)
	{
		Grame[xx1][y] &= ~( 0x01 << xx2 );
	}
}

static void Replay_screenLine(uint8_t x,uint8_t Wigh_Length)			//�������� x �߿�
{
	uint8_t y,xx1,xx2;
	uint8_t yy = ( 128 -  Wigh_Length) / 2;
	
	if(Wigh_Length == 0)return;
	xx1 = x / 8;
	xx2 = x % 8;
	if( x == 31 || x == 32 )
	{
		for(y = yy; y < ( yy + Wigh_Length ); y++)
		{
			Grame[xx1][y] |= ( 0x01 << xx2 );
		}
	}
	else
	{
		for(y = 0; y < 128; y++)
		{
			Grame[xx1][y] |= ( 0x01 << xx2 );
		}
	}
}

static void Clear_screen(uint8_t High_Length,bool active)			//������ �߶� 32msx ����
{
	uint8_t x;
	if(High_Length == 0 && Wigh_Length == 128)
		return;
	
	
	if(active)
	{
		for(x = 0; x < High_Length; x++) //0-31  32-63  31 
		{
			Clear_screenLine(x);
		}
		Replay_screenLine(x,Wigh_Length);
	}
	else
	{
		for(x = 63; x > ( 63 - High_Length ); x--)
		{
			Clear_screenLine(x);
		}
		Replay_screenLine(x,Wigh_Length);
	}
	
}


//void auto_screen(void)
//{
//	Clear_screen(High_Length,true);
//	Clear_screen(High_Length,false);
//	
//	switch(active)
//	{
//		case 0:if(++High_Length == 32)active = 1;break;
//		case 1:if(--High_Length == 0)active = 0;	break;
//		default:if(++High_Length == 32)active = 1;break;
//	}
//}

void User_screen(void)												//����ִ��
{

	Clear_screen(High_Length,true);
	Clear_screen(High_Length,false);
	
	if(User.UserScreen.active == true)
	{
		active = User.UserScreen.Screen_stat;
		switch(active)
		{
			case 0: if(High_Length <  32){High_Length += 2;}else{Wigh_Length -= 8;if(Wigh_Length == 0)active = 2;};break;	//close
			case 1: if(Wigh_Length < 128){Wigh_Length += 8;}else{High_Length -= 2;if(High_Length == 0)active = 2;};break;	//open
			default:if(High_Length <  32){High_Length += 2;}else{Wigh_Length -= 8;if(Wigh_Length == 0)active = 2;};break;	//close
		}
		if(active == 2)
		{
			User.UserScreen.active = false;
		}
	}

	
}


void Screen_Open(void)
{
	if(User.UserScreen.Screen_stat)return;
	User.UserScreen.Screen_stat = true;
	User.UserScreen.active = true;
}

void Screen_Close(void)
{
	if(!User.UserScreen.Screen_stat)return;
	User.UserScreen.Screen_stat = false;
	User.UserScreen.active = true;
}

void Screen_Change(void)											//�л�����״̬
{
	if(User.UserScreen.active == false)
	{
		switch((uint8_t)User.UserScreen.Screen_stat)
		{
			case 0: User.UserScreen.Screen_stat = true; break;
			case 1: User.UserScreen.Screen_stat = false;break;
			default:User.UserScreen.Screen_stat = true; break;
		}
		User.UserScreen.active = true;
	}
}
