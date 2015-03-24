#include "room.h"

extern TTF_Font *mayFont;
extern int mayflyTotal;
extern int enemyTotal;

Room *gameRoom = NULL;

extern	SDL_Color c_Black;
extern SDL_Surface *screen;
extern char *menuBack;

Room *createRoom()
{
	Room *temp = (Room*)malloc(sizeof(Room));
	//temp->background = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp->background = load_image(menuBack);
	temp->roomName = MENU;
	temp->mode = DRAFT;
	return temp;
}

void updateBackground(Room *r, SDL_Surface* final)
{
	apply_surface(0, 0, r->background, final, NULL);
}

void changeBackground(Room *r, char *imageFile)
{
	if (r->background != NULL)
	{
		SDL_FreeSurface(r->background);
	}
	strncpy(r->filename,imageFile,30);
	r->background = load_image(r->filename);
}

void displayRoomInfo(Room *r)
{
	//Display Stat Names
	printString("Mayflies: ",	c_Black, screen, 700, 50);
	printString("    Mode: ",	c_Black, screen, 700, 100);
	printString(" Enemies: ",	c_Black, screen, 700, 150);

	//Display Stat values
	printInt(mayflyTotal,	c_Black, screen, 825, 50);
	printInt(enemyTotal,	c_Black, screen, 825, 150);

	if (r->mode == DRAFT)		printString("Draft", c_Black, screen, 825, 100);
	else if (r->mode == HEAL)	printString("Heal",  c_Black, screen, 825, 100);
	else if (r->mode == TRAIN)	printString("Train", c_Black, screen, 825, 100);
	else if (r->mode == SCOUT)	printString("Scout", c_Black, screen, 825, 100);
	else if (r->mode == BREED)	printString("Breed", c_Black, screen, 825, 100);
}

void roomThink(Room *r)
{
	if (r->roomName == MAIN) displayRoomInfo(r);
}