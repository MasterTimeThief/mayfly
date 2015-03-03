#ifndef __ROOM__
#define __ROOM__

#include "game.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "graphics.h"

typedef enum
{
	MENU,
	MAIN,
	COMBAT
}RoomType;

typedef struct Room_T
{
	SDL_Surface *background;
	char filename[30];
	RoomType roomName;
	TTF_Font *font; //Optional
}Room;

Room *createRoom();
void changeBackground(Room *r, char *imageFile, SDL_Surface* final);

#endif