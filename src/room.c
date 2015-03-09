#include "room.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern TTF_Font *mayFont;
extern int mayflyTotal;

extern	SDL_Color textColor;
extern SDL_Surface *screen;

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

void displayRoomInfo(Room *r)
{
	SDL_Surface *temp;
	char		tempString[3];

	//Display Stat Names
	temp = TTF_RenderText_Solid( mayFont, "Mayflies: ", textColor );
	apply_surface(700,50,temp,screen,NULL);

	temp = TTF_RenderText_Solid( mayFont, "    Mode: ", textColor );
	apply_surface(700,100,temp,screen,NULL);

	//Display Stat values
	sprintf(tempString, "%i", mayflyTotal);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(825,50,temp,screen,NULL);

	if (r->mode == MOVE)		temp = TTF_RenderText_Solid( mayFont, "Move", textColor );
	else if (r->mode == DRAFT)	temp = TTF_RenderText_Solid( mayFont, "Draft", textColor );
	else if (r->mode == HEAL)	temp = TTF_RenderText_Solid( mayFont, "Heal", textColor );
	else if (r->mode == TRAIN)	temp = TTF_RenderText_Solid( mayFont, "Train", textColor );
	else if (r->mode == SCOUT)	temp = TTF_RenderText_Solid( mayFont, "Scout", textColor );
	else if (r->mode == BREED)	temp = TTF_RenderText_Solid( mayFont, "Breed", textColor );

	apply_surface(825,100,temp,screen,NULL);

	//Add in enemy counter
	SDL_FreeSurface(temp);
}

void roomThink(Room *r)
{
	if (r->roomName == MAIN) displayRoomInfo(r);
}