#include "GAME1.H"

static void Exit_Menu(void);//返回菜单

static void Life_Game(void);
static void Life_Init(void);

static uint8_t Get_Now_Life_Stat(uint8_t x,uint8_t y);
static void Life_Now_Cell(uint8_t x,uint8_t y);
static void Dead_Now_Cell(uint8_t x,uint8_t y);

static uint8_t Get_Nex_Life_Stat(uint8_t x,uint8_t y);
static void Life_Nex_Cell(uint8_t x,uint8_t y);
static void Dead_Nex_Cell(uint8_t x,uint8_t y);

static void OneCell_Dead_or_Life(uint8_t x,uint8_t y);

static uint8_t Life[8][128] = {0};

static prev_menu_s prev_menu;

enum GAME_STAT
{
	GAME_RUNNING,
	GAME_STOPPING,
	GAME_RUNSTEP,
	
	GAME_RESTART,
	GAME_MOD_NUM
};

static uint8_t Game_Stat = GAME_STOPPING;

static void RESTART(void)
{
	Game_Stat = GAME_RESTART;
}

static void CHANGE(void)
{
	if(Game_Stat == GAME_STOPPING)
	{
		Game_Stat = GAME_RUNNING;
	}
	else if(Game_Stat == GAME_RUNNING)
	{
		Game_Stat = GAME_STOPPING;
	}
}


void GAME_LIFE(void)
{
	Set_Draw_Func((draw_f)Life_Game);
	Set_Button_Func((button_f)CHANGE,(button_f) Exit_Menu,(button_f)RESTART);
	
	Set_Menu_Pre(&prev_menu,GAME_LIFE);
	
	Animation_InTo(Life_Init);
}

static void Exit_Menu(void)
{
	Game_Stat = GAME_STOPPING;
	displayauto = 0;

	Main_Menu.prevMenu = (&prev_menu)->last;
	(&prev_menu)->lastSelected = 0;
	Exit();
}

static void Life_Init(void)								//随机初始化
{
	uint8_t x,y;
	displayauto = 1;
	for(x = 0; x < 8; x++)
	{
		for(y = 0; y < 128; y++)
		{
			Grame[x][y] = rand()%255;
		}
	}
}

/*-------------------------------------------------------------------------*/
static uint8_t Get_Now_Life_Stat(uint8_t x,uint8_t y)//获取当前生命状态
{
	uint8_t xx1 = x/8,xx2 = x%8,life = 0;
	if(x >= 64 || y >= 128)
		return 0;
	if(Grame[xx1][y] & ( 0x01 << xx2 ))
	{
		life = 1;
	}
	return life;
}

static void Life_Now_Cell(uint8_t x,uint8_t y)				//繁衍生命
{
	uint8_t xx1 = x/8,xx2 = x%8;
	Life[xx1][y] |= ( 0x01 << xx2 );
}

static void Dead_Now_Cell(uint8_t x,uint8_t y)				//结束生命
{
	uint8_t xx1 = x/8,xx2 = x%8;
	Life[xx1][y] &= ~( 0x01 << xx2 );
}
/*-------------------------------------------------------------------------*/
static uint8_t Get_Nex_Life_Stat(uint8_t x,uint8_t y)//获取下一周期生命状态
{
	uint8_t xx1 = x/8,xx2 = x%8,life = 0;
	if(x >= 64 || y >= 128)
		return 0;
	if(Life[xx1][y] & ( 0x01 << xx2 ))
	{
		life = 1;
	}
	return life;
}

static void Life_Nex_Cell(uint8_t x,uint8_t y)				//繁衍生命
{
	uint8_t xx1 = x/8,xx2 = x%8;
	Grame[xx1][y] |= ( 0x01 << xx2 );
}

static void Dead_Nex_Cell(uint8_t x,uint8_t y)				//结束生命
{
	uint8_t xx1 = x/8,xx2 = x%8;
	Grame[xx1][y] &= ~( 0x01 << xx2 );
}

/*-------------------------------------------------------------------------*/
static void OneCell_Dead_or_Life(uint8_t x,uint8_t y)	//单生命状态
{
	uint8_t cell_roundlife = ( Get_Now_Life_Stat(x - 1,y - 1) + Get_Now_Life_Stat(x - 1,y)\
								+ Get_Now_Life_Stat(x - 1,y + 1)+ Get_Now_Life_Stat(x,y - 1)\
								+ Get_Now_Life_Stat(x,y + 1) + Get_Now_Life_Stat(x + 1,y - 1)\
								+ Get_Now_Life_Stat(x + 1,y) + Get_Now_Life_Stat(x + 1,y + 1) );
	uint8_t now_cell_life_stata = Get_Now_Life_Stat(x,y);
	if(now_cell_life_stata)
	{
		if(cell_roundlife > 3 || cell_roundlife < 2)
		{
			Dead_Now_Cell(x,y);
		}
		else
		{
			Life_Now_Cell(x,y);
		}
	}
	else
	{
		if(cell_roundlife == 3)
		{
			Life_Now_Cell(x,y);
		}
		else
		{
			Dead_Now_Cell(x,y);
		}
	}
}

static void Life_Now(void)
{
	uint8_t x,y;
	for(x = 0; x < 64; x++)
		{
			for(y = 0; y < 128; y++)
			{
				OneCell_Dead_or_Life(x,y);
			}
		}
}

static void Life_Nex(void)
{
	uint8_t x,y;
	for(x = 0; x < 64; x++)
		{
			for(y = 0; y < 128; y++)
			{
				if(Get_Nex_Life_Stat(x,y))
				{
					Life_Nex_Cell(x,y);
				}
				else
				{
					Dead_Nex_Cell(x,y);
				}
			}
		}
}

static void Life_Game(void)
{
	static uint32_t lastUpdate;

	uint32_t now = millis();
	if ((uint32_t)(now - lastUpdate) >= 200)
	{
		lastUpdate = now;

		switch(Game_Stat)
				{
					case GAME_RUNNING: 	Life_Now(); Life_Nex();							  break;
					case GAME_RUNSTEP: 	Life_Now(); Life_Nex(); Game_Stat = GAME_STOPPING;break;
					case GAME_STOPPING: 												  break;
					case GAME_RESTART: 	Life_Init(); Game_Stat = GAME_STOPPING;			  break;
					default: 															  break;
				}
	}
}
