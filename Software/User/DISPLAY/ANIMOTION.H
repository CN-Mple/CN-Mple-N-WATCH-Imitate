#ifndef __ANIMOTION_H
#define __ANIMOTION_H

#include "stm32f10x.h"

#include "MENU.H"
#include <stdbool.h>
#include "bsp_oled.h"

typedef struct{
	bool active;             		//����
	uint8_t offsetY;         		//��תY
	void (*animOnComplete)(void);	//ָ��ĺ���
	bool goingOffScreen;   			//��Ļ����
}anim_s;


extern anim_s animotion;

void Animation_InTo(void (*animOnComplete)(void));
void Animation_Leave(void (*animOnComplete)(void));

bool animation_active(void);
bool animation_movingOn(void);

void animotion_updata(void);


#endif /* __ANIMOTION_H */