#include "room.h"
#include "mayfly.h"

extern TTF_Font *mayFont;
extern int mayflyTotal;
extern int enemyTotal;

int spacePressed;

//Room *gameRoom = NULL;

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

	spacePressed = 0;
	return temp;
}

void updateBackground(SDL_Surface* final)
{
	apply_surface(0, 0, gameRoom->background, final, NULL);
}

void changeBackground(char *imageFile)
{
	if (gameRoom->background != NULL)
	{
		SDL_FreeSurface(gameRoom->background);
	}
	strncpy(gameRoom->filename,imageFile,30);
	gameRoom->background = load_image(gameRoom->filename);
}

void displayRoomInfo()
{
	//Display Stat Names
	printString("Mayflies: ",	c_Black, screen, 700, 100);
	printString("   Mode: ",	c_Black, screen, 700, 150);
	printString(" Enemies: ",	c_Black, screen, 700, 200);

	//Display Stat values
	printInt(mayflyTotal,	c_Black, screen, 825, 100);
	printInt(enemyTotal,	c_Black, screen, 825, 200);

	if (gameRoom->mode == DRAFT)
	{
		printString("Draft", c_Black, screen, 825, 150);
		printString("Select Mayflies to fight for your cause!", c_White, screen, 256, 20);
	}
	else if (gameRoom->mode == HEAL)	
	{
		printString("Heal",  c_Black, screen, 825, 150);
		printString("Replenish your Mayfly's health! (uses daily action)", c_White, screen, 256, 20);
	}
	else if (gameRoom->mode == TRAIN)	
	{
		printString("Train", c_Black, screen, 825, 150);
		printString("Train up your Mayfly in class of your choice! (uses daily action)", c_White, screen, 256, 20);
	}
	else if (gameRoom->mode == SCOUT)	
	{
		printString("Scout", c_Black, screen, 825, 150);
		printString("Send out Mayfly to return with enemy information! (uses daily action)", c_White, screen, 256, 20);
	}
	else if (gameRoom->mode == BREED)	
	{
		printString("Breed", c_Black, screen, 825, 150);
		printString("Increase your army by selecting the best traits! (uses daily action)", c_White, screen, 256, 20);
	}
}

void modeChange()
{
	if (gameRoom->mode == BREED)		gameRoom->mode = DRAFT;
	else if (gameRoom->mode == DRAFT)	gameRoom->mode = HEAL;
	else if (gameRoom->mode == HEAL)	gameRoom->mode = TRAIN;
	else if (gameRoom->mode == TRAIN)	gameRoom->mode = SCOUT;
	else if (gameRoom->mode == SCOUT)	gameRoom->mode = BREED;
}

void roomThink(Room *r)
{
	int numkeys;
	Uint8 *keys = SDL_GetKeyState(&numkeys);

	if (r->roomName == MAIN) displayRoomInfo(r);
	if (keys[SDLK_SPACE])
	{
		if (!spacePressed)
		{
			modeChange();
			clearMayflySelection();
			spacePressed = 1;
		}
	}
	else spacePressed = 0;
}