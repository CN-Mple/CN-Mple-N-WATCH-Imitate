#ifndef __ANIMOTION_H
#define __ANIMOTION_H

#include "stm32f10x.h"

#include "MENU.H"
#include <stdbool.h>
#include "bsp_oled.h"

typedef struct{
	bool active;             		//开关
	uint8_t offsetY;         		//反转Y
	void (*animOnComplete)(void);	//指向的函数
	bool goingOffScreen;   			//屏幕方向
}anim_s;


extern anim_s animotion;

void Animation_InTo(void (*animOnComplete)(void));
void Animation_Leave(void (*animOnComplete)(void));

bool animation_active(void);
bool animation_movingOn(void);

void animotion_updata(void);


#endif /* __ANIMOTION_H */
