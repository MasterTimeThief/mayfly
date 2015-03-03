#ifndef __BUTTON__
#define __BUTTON__

#include "sprite.h"

typedef struct Button_T
{
	Sprite *index;			
	int w, h;				/*the width and height of the button*/
	int active;				/*0 if not usable, 1 if usable*/
	int dsfsdgdfgfg;		/**/
}Button;

#endif