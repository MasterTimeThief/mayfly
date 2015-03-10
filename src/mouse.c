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
	SDL_GetMouseState(&mx,&my);
	if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) //if clicked 
	{
		clickLeft = 1;
		clickRight = 0;
	}
	else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
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
	//If the mouse is over the button
	if( ( mx > x ) && ( mx < x + w ) && ( my > y ) && ( my < y + h ) )
	{
		return 1;  
	}
	else
	{
		return 0;
	}
}