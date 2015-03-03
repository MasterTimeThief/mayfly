#include "menu.h"

SDL_Color textColor = { 255, 8, 255 };

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

Menu *createMenu(char *fontFile, int fontSize)
{
	Menu *temp = (Menu*)malloc(sizeof(Menu));
	temp->message = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp->font = TTF_OpenFont( fontFile, fontSize );
	temp->choice = NEW;
	temp->cursor = LoadSprite("images/believer.png",32,32);
	temp->changed = 0;
	return temp;
}

void displayMenu(Menu *m)
{
	SDL_Surface *temp;
	//SDL_Surface *curr;
	//int choicePos;

	if (m->message == NULL) return;

	m->message = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	temp = TTF_RenderText_Solid( m->font, "NEW", textColor );
	apply_surface(800,400,temp,m->message,NULL);

	temp = TTF_RenderText_Solid( m->font, "LOAD", textColor );
	apply_surface(800,450,temp,m->message,NULL);

	temp = TTF_RenderText_Solid( m->font, "EXIT", textColor );
	apply_surface(800,500,temp,m->message,NULL);

	/*if (m->choice == NEW) choicePos = 400;
	else if (m->choice == LOAD) choicePos = 450;
	else if (m->choice == EXIT) choicePos = 500;*/
	//DrawSprite(m->cursor,m->message,750,choicePos, 0);

	m->changed = 1;

	//m->message = curr;
	SDL_FreeSurface(temp);
	//SDL_FreeSurface(curr);
}