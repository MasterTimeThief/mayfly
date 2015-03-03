#include "room.h"

Room *createRoom()
{
	Room *temp = (Room*)malloc(sizeof(Room));
	temp->background = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp->roomName = MENU;
	return temp;
}

void changeBackground(Room *r, char *imageFile, SDL_Surface* final)
{
	strncpy(r->filename,imageFile,30);
	r->background = load_image(imageFile);
	apply_surface(0, 0, r->background, final, NULL);
}