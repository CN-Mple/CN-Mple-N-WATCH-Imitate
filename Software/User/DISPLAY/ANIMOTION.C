#include "ANIMOTION.H"



anim_s animotion;


void animotion_start(void (*animOnComplete)(void),bool active)
{
	if(1)
	{
		animotion.active = true;
		animotion.animOnComplete = animOnComplete;
		animotion.offsetY = active ? 0 : 192;
		animotion.goingOffScreen = active;
	}
	else
	{
		
	}
}
void Animation_InTo(void (*animOnComplete)(void))
{
	animotion_start(animOnComplete,false);
}

void Animation_Leave(void (*animOnComplete)(void))
{
	animotion_start(animOnComplete,true);
}
 	

bool animation_active(void)
{
	return animotion.active;
}

bool animation_movingOn(void)
{
	return !animotion.goingOffScreen;
}

void animotion_updata(void)
{
	uint8_t offsetY = 0;

	if(animotion.active)
	{
		offsetY = animotion.offsetY;
		if(animotion.goingOffScreen)
		{
			if(offsetY < 4)
			{
				offsetY = offsetY + 1;
			}
			else if(offsetY < 8)
			{
				offsetY = offsetY + 3;
			}
			else if(offsetY < 16)
			{
				offsetY = offsetY + 5;
			}
			else
			{
				offsetY = offsetY + 8;
			}
			if(offsetY > 64)
			{
				animotion.active = false;
				offsetY = 0;
			}
		}
		else
		{
			if(offsetY > 255 - 4)
				offsetY += 1;
			else if(offsetY > 255 - 8)
				offsetY += 3;
			else if(offsetY > 255 - 16)
				offsetY += 5;
			else
				offsetY += 8;

			if(offsetY < 10)
			{
				animotion.active = false;
				offsetY = 0;
			}
		}
		animotion.offsetY = offsetY;
		if(!animation_active() && animotion.animOnComplete != NULL)
		{
			animotion.animOnComplete();
			animotion.animOnComplete = NULL;
		}
	}
}


