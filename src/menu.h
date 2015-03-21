#ifndef MENU__
#define MENU__

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
	int changed;  /*0 if same, 1 if changed*/
}Menu;

extern Menu *mainMenu;

Menu *createMenu(char *fontFile, int fontSize);
void displayMenu(Menu *m);

#endif