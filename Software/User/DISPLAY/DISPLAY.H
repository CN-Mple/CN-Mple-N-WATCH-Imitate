#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "MENU.H"
#include "ANIMOTION.H"
#include "BREATHSCREEN.H"

#include "bsp_mpu6050.h"
#include "BUTTON.H"

extern uint8_t displayauto;

void Set_Draw_Func(draw_f New_drawFunc);

void display_updata(void);

#endif /* __DISPLAY_H */
