#ifndef __MENU__
#define __MENU__

#include "SDL.h"
#include "SDL_ttf.h"
#include "sprite.h"
#include "graphics.h"
#include "game.h"

typedef enum
{
	NEW,
	LOAD,
	EXIT
}menuChoice;

typedef struct Menu_T
{
	SDL_Surface *message;
	Sprite *cursor;
	TTF_Font *font;
	menuChoice choice;
}Menu;

Menu *createMenu(char *fontFile, int fontSize, Sprite *spr);
void displayMenu(Menu *m);

#endif