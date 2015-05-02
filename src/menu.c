#include "menu.h"
#include "button.h"
#include "room.h"
#include "mayfly.h"
#include "combat.h"
#include "audio.h"

extern	SDL_Surface *screen;
extern int STARTING_MAYFLY;
extern char *mainBack;

void menuNew()
{
	int i;
	
	gameRoom->roomName = MAIN;
	changeBackgroundMusic();
	//createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
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
	//createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
	mainButtons();
	changeBackground(mainBack);
	//Load in saved mayflies
	loadFromList();
}

void menuExit()
{
	gameRoom->roomName = QUIT;
}