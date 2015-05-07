#include "menu.h"
#include "button.h"
#include "room.h"
#include "mayfly.h"
#include "combat.h"
#include "audio.h"

extern	SDL_Surface *screen;
extern int STARTING_MAYFLY;
extern char *mainBack;
extern char *editBack;

void menuNew()
{
	int i;
	
	gameRoom->roomName = MAIN;
	changeBackgroundMusic();
	mainButtons();
	changeBackground(mainBack);
	//new game script
	for (i = 0; i < STARTING_MAYFLY; i++)
	{
		createMayfly();
	}
	for(i = 0; i < MAX_ENEMIES; i++)
	{ 
		createEnemy();
	}
}

void menuLoad()
{
	gameRoom->roomName = MAIN;
	changeBackgroundMusic();
	mainButtons();
	changeBackground(mainBack);
	//Load in saved mayflies
	loadFromList();
}

void menuExit()
{
	gameRoom->roomName = QUIT;
}

void menuEdit()
{
	gameRoom->roomName = EDIT;
	changeBackgroundMusic();
	changeBackground(editBack);
	loadFromList();
	setupMayflyEdit();
}