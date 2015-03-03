#ifndef __GAME__
#define __GAME__

#include "SDL.h"
#include "graphics.h"
#include "menu.h"
#include "room.h"

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 576;
int SCREEN_BPP = 32;

char *menuBack = "images/battle2.png";
char *mainBack = "images/battle2.png";
char *combatBack = "images/battle2.png";

//SDL_Surface *buffer = NULL;
SDL_Surface *screen = NULL;

SDL_Event eventCheck;

Sprite *cursor = NULL;
Menu *mainMenu = NULL;
Room *gameRoom = NULL;

#endif