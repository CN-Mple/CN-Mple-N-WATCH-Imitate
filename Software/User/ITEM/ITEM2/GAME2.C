#include "GAME2.H"

static void Exit_Menu(void);//返回菜单
static prev_menu_s prev_menu;

static void Flybrid_Init(void);
static void Flybrid_Game(void);


static void Flybrid(void);
static void Fly(void);
static void Walls(void);
static void Creat_Wall(void);


static void RESTART(void);


enum UPorDOWM
{
	STATE_UP,
	STATE_BLA,
	STATE_DOWN,
	STATE_NUM,
};

enum GAME_STAT
{
	GAME_RUNNING,
	GAME_STOPPING,
	GAME_RESTART,
	GAME_MOD_NUM
};

typedef struct POS
{
	uint8_t life;
	uint8_t state;
	uint8_t length;
	uint8_t x;
	int y;
}POS_t;



POS_t Wall[WALL_MAX] = {0}; 
POS_t brid = {0};

static uint8_t Game_Stat = GAME_RUNNING;
static uint8_t brid_x;



static void RESTART(void)					//重新开始
{
	Game_Stat = GAME_RESTART;
}

static void Fly(void)						//飞行状态
{
	switch(brid.state)
	{
		case STATE_UP:	if(--brid.x == brid_x)brid.state = STATE_BLA;	break;
		case STATE_BLA:	brid.state = STATE_DOWN;						break;
		case STATE_DOWN:if(++brid.x == 63)	Game_Stat = GAME_RESTART;	break;
		default:														break;
	}
}

static void UPUP(void)						//飞！
{
	brid.state = STATE_UP;
	brid_x = brid.x - 10;
	if(brid_x > 64)
		brid_x = 0;
}



void GAME_Flybrid(void)
{
	Flybrid_Init();
	Set_Draw_Func((draw_f)Flybrid_Game);
	Set_Button_Func((button_f)UPUP,(button_f) Exit_Menu,(button_f)RESTART);
	
	Set_Menu_Pre(&prev_menu,GAME_Flybrid);
	
	Animation_InTo(NULL);
}

static void Exit_Menu(void)
{
	displayauto = 0;

	Main_Menu.prevMenu = (&prev_menu)->last;
	(&prev_menu)->lastSelected = 0;
	Exit();
}

static char Win[5];
static uint8_t Last_Wall = 0;
static uint32_t brid_Win_num = 0;

static void Flybrid_Init(void)								//随机初始化
{
	uint8_t i;

	brid.life = 1;
	brid.state = STATE_BLA;
	brid.x = 20;
	brid.y = 20;
	
	Last_Wall = 0;
	brid_Win_num = 0;
	
	sprintf(Win,"%d",brid_Win_num);
	
	for(i = 0; i < WALL_MAX; i++)
		Wall[i].life = 0;
}

static uint8_t Get_roo(uint8_t x,uint8_t y)				//获取当前碰撞箱状态
{
	uint8_t xx1 = x/8,xx2 = x%8,err = 0;
	if(x >= 64 || y >= 128)
		return err;
	if(Grame[xx1][y] & ( 0x01 << xx2 ))
	{
		err = 1;
	}
	return err;
}

static void Flybrid(void)								//brid绘制 且 碰撞判断
{
	const uint8_t* bitmap = (const uint8_t*)&brid_state;
	uint8_t i;
	for(i = brid.y; i < brid.y + 24; i++)
	{
		if(Get_roo(brid.x + 6,i) || Get_roo(brid.x + 18,i))
		{
			Game_Stat = GAME_RESTART;
			break;
		}
	}
	Draw_bmp(brid.x,brid.y,&bitmap[brid.state * 72],24,24,0);
}


uint8_t Line_24[] = {0xFF,0xFF,0xFF};
uint8_t Line_32[] = {0xFF,0xFF,0xFF,0xFF};


static void Walls(void)								//绘制对向水管
{
	uint8_t i,Wall_num,x;
	int y;
	
	const uint8_t* bitmap1 = (const uint8_t*)&Line_24;
	const uint8_t* bitmap2 = (const uint8_t*)&Line_32;
	
	for(i = 0; i < WALL_MAX; i++)
	{
		Wall_num = ( Last_Wall + i ) % WALL_MAX;
		if(Wall[Wall_num].life)
		{
			y = Wall[Wall_num].y;
			for(x = 0; x < Wall[Wall_num].length; x++)
			{
				Draw_bmp(x,y,&bitmap1[0],24,1,0);
			}
			y = ( Wall[Wall_num].y - 4 );
			x++;
			Draw_bmp(x,y,&bitmap2[0],32,1,0);
			x++;
			Draw_bmp(x,y,&bitmap2[0],32,1,0);
			//上水管
			
			x += 32;//空余空间
			
			//下水管
			Draw_bmp(x,y,&bitmap2[0],32,1,0);
			x++;
			Draw_bmp(x,y,&bitmap2[0],32,1,0);
			x++;
			x++;
			y = Wall[Wall_num].y;
			for(; x < 128; x++)
			{
				Draw_bmp(x,y,&bitmap1[0],24,1,0);
			}
			y = ( Wall[Wall_num].y - 4 );
			

			Wall[Wall_num].y -= 2;
			if(Wall[Wall_num].y < -30)
			{
				Wall[Wall_num].life = 0;
			}
		}
		
	}
	
}

static void Creat_Wall(void)				//更新水管
{
	uint8_t i,num = Last_Wall;
	for(i = 0; i < WALL_MAX; i++)
	{
		num = ( Last_Wall + i ) % WALL_MAX;
		if(Wall[num].life == 0)
			break;
	}
	if(num == Last_Wall)
	{
		Wall[Last_Wall].life = 1;
		Wall[Last_Wall].y = 128;
		Wall[Last_Wall].length = rand() % 31;
	}
	else
	{
		if(Wall[Last_Wall].y  <  30)
		{
			Wall[num].life = 1;
			Wall[num].y = 128;
			Wall[num].length = rand() % 31;
			Last_Wall = num;
		}
	}
	
}

static void Flybrid_Game(void)					//flybrid更新函数
{
	static uint32_t lastUpdate;

	uint32_t now = millis();
	
	static uint8_t filter = 0;
	
	if ((uint32_t)(now - lastUpdate) >= 20)
	{
		lastUpdate = now;
		if(++filter%50 == 0)
		{
			filter = 0;
			brid_Win_num++;
			sprintf(Win,"%d",brid_Win_num);
		}
			

		Draw_Str(0,0,Win);
		switch(Game_Stat)
		{
			case GAME_RUNNING: 	Creat_Wall();Fly();Walls();Flybrid();			break;
			case GAME_STOPPING: 												break;
			case GAME_RESTART: 	Flybrid_Init(); Game_Stat = GAME_RUNNING;		break;
			default:															break;
		}
		
	}
}
