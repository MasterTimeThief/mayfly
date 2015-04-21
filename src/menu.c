#include "menu.h"
#include "button.h"
#include "room.h"
#include "mayfly.h"
#include "combat.h"

extern	SDL_Surface *screen;
extern int STARTING_MAYFLY;
extern char *mainBack;

void menuNew()
{
	int i;
	
	gameRoom->roomName = MAIN;
	//createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
	mainButtons();
	changeBackground(gameRoom, mainBack);
	//new game script
	for (i = 0; i < STARTING_MAYFLY; i++)
	{
		createMayfly();
	}
}

void menuLoad()
{
	gameRoom->roomName = MAIN;
	//createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
	mainButtons();
	changeBackground(gameRoom, mainBack);
	//Load in saved mayflies
	loadFromList();
}

void menuExit()
{
	gameRoom->roomName = QUIT;
}