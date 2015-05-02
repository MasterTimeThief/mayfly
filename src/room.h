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
	COMBAT,
	QUIT
}RoomType;

typedef enum
{
	DRAFT,
	HEAL,
	TRAIN,
	SCOUT,
	BREED
}RoomMode;

typedef struct Room_T
{
	SDL_Surface *background;
	char		filename[30];
	RoomType	roomName;
	RoomMode	mode;
	//TTF_Font *font; //Optional
}Room;

Room *gameRoom;

Room *createRoom();
void updateBackground(SDL_Surface* final);
void changeBackground(char *imageFile);
void roomThink(Room *r);

#endif