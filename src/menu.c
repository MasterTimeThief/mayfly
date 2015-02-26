#include "menu.h"
#include "graphics.h"

SDL_Color textColor = { 255, 255, 255 };

Menu *createMenu(char *fontFile, int fontSize, Sprite *spr)
{
	Menu *temp;
	temp->font = TTF_OpenFont( fontFile, fontSize );
	temp->choice = NEW;
}

void displayMenu(Menu *m)
{
	SDL_Surface *temp;
	SDL_Surface *curr;
	int choicePos;

	temp = TTF_RenderText_Solid( m->font, "NEW", textColor );
	apply_surface(600,250,temp,curr,NULL);

	temp = TTF_RenderText_Solid( m->font, "LOAD", textColor );
	apply_surface(600,300,temp,curr,NULL);

	temp = TTF_RenderText_Solid( m->font, "EXIT", textColor );
	apply_surface(600,350,temp,curr,NULL);

	if (m->choice == NEW) choicePos = 250;
	else if (m->choice == LOAD) choicePos = 300;
	else if (m->choice == EXIT) choicePos = 350;
	DrawSprite(m->cursor,curr,575,choicePos, 0);

	m->message = curr;
	SDL_FreeSurface(temp);
	SDL_FreeSurface(curr);
}