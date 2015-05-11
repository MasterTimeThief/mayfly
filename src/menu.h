#ifndef MENU__
#define MENU__

#include "SDL.h"
#include "SDL_ttf.h"
#include "sprite.h"
#include "graphics.h"
#include "game.h"
/*
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
	int changed;
}Menu;
*/

void menuNew();
void menuLoad();
void menuExit();
void menuEdit();
void menuMulti();

#endif