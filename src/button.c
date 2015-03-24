#include "button.h"
#include "mouse.h"
#include "room.h"
#include "mayfly.h"
#include "combat.h"
#include "event.h"

Button buttonList[MAX_BUTTONS];
//Button *combatButton;
extern	SDL_Surface *screen;
extern char *combatBack;

void initButtonList()
{
	int x;
	memset(buttonList,0,sizeof(Button) * MAX_BUTTONS);
	for(x = 0; x < MAX_BUTTONS; x++)
	{ 
		buttonList[x].entity = NULL;
		buttonList[x].effect = NULL;
		buttonList[x].think = NULL;
		buttonList[x].visible = 0;
	}
}

Button *newButton()
{
	int i;
	for (i = 0;i < MAX_BUTTONS; i++)
	{
		if (!buttonList[i].visible)
		{
			buttonList[i].visible = 1;
			return &buttonList[i];	
		}
	}
}

void createButton(int x, int y, int w, int h, char *filename, void (*think)(int index), void (*effect)())
{
	Button *b = newButton();
	if (b != NULL)
	{
		b->think = (*think);
		b->effect = (*effect);

		b->entity = createEntity();
		b->entity->image = LoadSprite(filename,w,h);
		b->entity->ex = x;
		b->entity->ey = y;
	}
	
	/*
	createButton(64, 416, 128, 64, "images/button.png", combatButtonThink, toCombat)
	combatButton = (Button*)malloc(sizeof(Button));
	combatButton->visible = 1;
	combatButton->effect = toCombat;

	combatButton->entity = createEntity();
	combatButton->entity->image = LoadSprite("images/button.png",128,64);
	combatButton->entity->ex = 64;
	combatButton->entity->ey = 416;*/
}

void freeButton(Button *b)
{
	if (b->entity != NULL) freeEntity(b->entity);
	b->effect = NULL;
	b->think = NULL;
}

int checkClick(Button *b)
{
	if (mouseHover(b->entity->ex,  b->entity->ey,  b->entity->image->w,  b->entity->image->h))
	{
		if (clickLeft)	return 1;
	}
	return 0;
}

void combatButtonThink(int index)
{
	if (gameRoom->roomName != MAIN)
	{
		buttonList[index].visible = 0;
	}
	else buttonList[index].visible = 1;

	if (checkClick(&buttonList[index]) && gameRoom->mode == DRAFT && checkSelected() > 1)
	{
		(*buttonList[index].effect)();
		freeButton(&buttonList[index]);
	}
}

void menuButtonThink(int index)
{
	if (gameRoom->roomName != MENU)			freeButton(&buttonList[index]);
	if (checkClick(&buttonList[index]))
	{
		if (buttonList[index].effect != NULL) (*buttonList[index].effect)();
	}
}

void buttonThink()
{
	int x;
	for(x = 0; x < MAX_BUTTONS; x++)
	{ 
		/*if (!buttonList[x].visible) freeButton(&buttonList[x]);//delete button
		else
		{*/
		if (buttonList[x].visible)
		{
			DrawSprite(buttonList[x].entity->image, screen, buttonList[x].entity->ex, buttonList[x].entity->ey, buttonList[x].entity->frame);
			if (buttonList[x].think != NULL) (*buttonList[x].think)(x);
			if (checkClick(&buttonList[x])) buttonList[x].entity->frame = 1;
			else							buttonList[x].entity->frame = 0;
		}
	}
	
	/*if (gameRoom->roomName == MAIN)
	{
		combatButton->visible = 1;
		if (combatButton->visible) DrawSprite(combatButton->entity->image, screen, combatButton->entity->ex, combatButton->entity->ey, combatButton->entity->frame);
		if (mouseHover(combatButton->entity->ex,  combatButton->entity->ey,  combatButton->entity->image->w,  combatButton->entity->image->h))
		{
			if (clickLeft)
			{
				combatButton->entity->frame = 1;
				if (gameRoom->mode == DRAFT && checkSelected() > 1)
				{
					(combatButton->effect)();
					combatButton->entity->frame = 0;
				}
			}
			else if (!clickLeft) combatButton->entity->frame = 0;
		}
	}
	else combatButton->visible = 0;*/
}