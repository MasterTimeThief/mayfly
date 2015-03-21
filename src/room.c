#include "room.h"

extern TTF_Font *mayFont;
extern int mayflyTotal;

Room *gameRoom = NULL;

extern	SDL_Color c_Black;
extern SDL_Surface *screen;

Room *createRoom()
{
	Room *temp = (Room*)malloc(sizeof(Room));
	//temp->background = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp->background = load_image("images/menu.png");
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
	SDL_Surface *temp;
	char		tempString[3];

	//Display Stat Names
	temp = renderText( mayFont, "Mayflies: ", c_Black );
	apply_surface(700,50,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "    Mode: ", c_Black );
	apply_surface(700,100,temp,screen,NULL);
	SDL_FreeSurface(temp);

	//Display Stat values
	sprintf(tempString, "%i", mayflyTotal);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(825,50,temp,screen,NULL);
	SDL_FreeSurface(temp);

	if (r->mode == DRAFT)		temp = renderText( mayFont, "Draft", c_Black );
	else if (r->mode == HEAL)	temp = renderText( mayFont, "Heal", c_Black );
	else if (r->mode == TRAIN)	temp = renderText( mayFont, "Train", c_Black );
	else if (r->mode == SCOUT)	temp = renderText( mayFont, "Scout", c_Black );
	else if (r->mode == BREED)	temp = renderText( mayFont, "Breed", c_Black );

	apply_surface(825,100,temp,screen,NULL);

	//Add in enemy counter
	SDL_FreeSurface(temp);
}

void roomThink(Room *r)
{
	if (r->roomName == MAIN) displayRoomInfo(r);
}