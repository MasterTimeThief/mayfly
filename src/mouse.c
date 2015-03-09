#include "mouse.h"

extern SDL_Event eventCheck;

void mouseInit()
{
	clickLeft = 0;
	clickRight = 0;
	mx = 0;
	my = 0;
}

void mouseThink()
{
	if(SDL_GetMouseState(&mx,&my) == 1) //if clicked 
	{
		clickLeft = 1;
		clickRight = 0;
	}
	else if (SDL_GetMouseState(&mx,&my) == 3)
	{
		clickLeft = 0;
		clickRight = 1;
	}
	else
	{
		clickLeft = 0;
		clickRight = 0;
	}
}

int mouseHover(int x, int y, int w, int h)
{
	//The mouse offsets
	int tempx = 0, tempy = 0;

	//If the mouse moved
	if( clickLeft )
	{
		//Get the mouse offsets
		tempx = mx;
		tempy = my;

		//If the mouse is over the button
		if( ( tempx > x ) && ( tempx < x + w ) && ( tempy > y ) && ( tempy < y + h ) )
		{
			return 1;  
		}
		else
		{
			return 0;
		}
	}
}