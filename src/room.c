#include "room.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

Room *createRoom()
{
	Room *temp = (Room*)malloc(sizeof(Room));
	temp->background = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp->roomName = MENU;
	temp->mode = MOVE;
	return temp;
}

void updateBackground(Room *r, char *imageFile, SDL_Surface* final)
{
	strncpy(r->filename,imageFile,30);
	r->background = load_image(imageFile);
	apply_surface(0, 0, r->background, final, NULL);
}