#include "DISPLAY.H"


static draw_f drawFunc;

uint8_t displayauto = 0;

void Set_Draw_Func(draw_f New_drawFunc)
{
	if(New_drawFunc != NULL)
	{
		drawFunc = New_drawFunc;
	}
}

extern float Pitch;extern float roll;extern float yaw;
void display_updata(void)
{
	
//	static char buff1[16];
//	static char buff2[16];
//	static char buff3[16];
//	//��ӡ
//	sprintf ( buff1, "p%.4f",Pitch );
//	sprintf ( buff2, "r%.4f",roll );
//	sprintf ( buff3, "y%.4f",yaw );
//	//����
//	Draw_Str(8,32,buff1);
//	Draw_Str(48,32,buff2);
//	Draw_Str(56,32,buff3);
	
	//����
	animotion_updata();
	//����
	drawFunc();
	//Ƶ��
	User_screen();
	

	//�û�������ʽ
	if(!User.UserScreen.Screen_stat && !User.UserScreen.active)
	mpl_updata();
	else if(User.UserScreen.Screen_stat && !User.UserScreen.active)
	Buttons_updata();
	
	OLED_ReFresh(MOD);			//֡����
	if(displayauto == 0)		//�Զ�˫���潻��ģʽ ֱ�Ӹ�������
	memset(Grame, 0x00, HEIGHTH_MAX * WHITH_MAX / 8);
}