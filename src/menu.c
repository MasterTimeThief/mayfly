#include "menu.h"
#include "button.h"
#include "room.h"
#include "mayfly.h"
#include "combat.h"

extern	SDL_Surface *screen;
extern int STARTING_MAYFLY;
extern char *mainBack;
Menu *mainMenu = NULL;


Menu *createMenu(char *fontFile, int fontSize)
{
	Menu *temp = (Menu*)malloc(sizeof(Menu));
	temp->message = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp->font = TTF_OpenFont( fontFile, fontSize );
	temp->choice = NEW;
	temp->cursor = LoadSprite("images/cursor.png",32,32);
	temp->changed = 0;

	createButton(768, 384, 96, 32, "images/newButton.png", (*menuButtonThink), menuNew);
	createButton(768, 448, 96, 32, "images/loadButton.png", (*menuButtonThink), menuLoad);
	createButton(768, 512, 96, 32, "images/exitButton.png", (*menuButtonThink), menuExit);

	return temp;
}

void menuNew()
{
	int i;
	
	gameRoom->roomName = MAIN;
	createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
	changeBackground(gameRoom, mainBack);
	//new game script
	for (i = 0; i < STARTING_MAYFLY; i++)
	{
		createMayfly();
	}
}

void menuLoad()
{
	menuNew();
	closeMayflies();
	//Load in saved mayflies
}

void menuExit()
{
	gameRoom->roomName = QUIT;
}

void displayMenu(Menu *m)
{
	/*SDL_Surface *temp;
	//SDL_Surface *curr;
	//int choicePos;

	if (m->message == NULL) return;

	m->message = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp = TTF_RenderText_Solid( m->font, "NEW", c_Black );
	apply_surface(800,400,temp,m->message,NULL);

	temp = TTF_RenderText_Solid( m->font, "LOAD", c_Black );
	apply_surface(800,450,temp,m->message,NULL);

	temp = TTF_RenderText_Solid( m->font, "EXIT", c_Black );
	apply_surface(800,500,temp,m->message,NULL);

	m->changed = 1;

	SDL_FreeSurface(temp);*/
}