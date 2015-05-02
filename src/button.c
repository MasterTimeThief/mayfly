#include "button.h"
#include "mouse.h"
#include "room.h"
#include "combat.h"
#include "event.h"
#include "save.h"
#include "audio.h"

Button buttonList[MAX_BUTTONS];
extern	SDL_Surface *screen;
extern char *combatBack;
extern Mayfly *trainee;

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
	return NULL;
}

void createButton(int x, int y, int w, int h, char *filename, void (*think)(int index), void (*effect)())
{
	Button *b = newButton();
	if (b != NULL)
	{
		if ((*think) != NULL)  b->think = (*think);
		if ((*effect) != NULL) b->effect = (*effect);

		b->entity = createEntity();
		b->entity->image = LoadSprite(filename,w,h);
		b->entity->ex = x;
		b->entity->ey = y;
	}
}

void freeButton(Button *b)
{
	if (b->entity != NULL) freeEntity(b->entity);
	b->visible = 0;
	b->effect = NULL;
	b->think = NULL;
}

void mainButtons()
{
	createButton(64, 384, 128, 64,  "images/button.png",	  (*combatButtonThink), toCombat);
	createButton(64, 448, 64, 64,   "images/healButton.png",  (*mainButtonThink), toHeal);
	createButton(128, 448, 64, 64,  "images/trainButton.png", (*mainButtonThink), toTrain);
	createButton(64, 512, 64, 64,   "images/breedButton.png", (*mainButtonThink), toBreed);
	createButton(128, 512, 64, 64,  "images/scoutButton.png", (*mainButtonThink), toScout);

	//sort buttons
	createButton(960, 64, 32, 32,  "images/sortButton.png", (*sortButtonThink), sortMayflyClass);
	createButton(960, 128, 32, 32,  "images/sortButton.png", (*sortButtonThink), sortMayflyGender);
	createButton(960, 196, 32, 32,  "images/sortButton.png", (*sortButtonThink), sortMayflyAge);
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
		freeButton(&buttonList[index]);
	}

	if (checkClick(&buttonList[index]))
	{
		saveToList();
		if (gameRoom->mode == DRAFT && mayflySelected > 0)
		{
			(*buttonList[index].effect)();
			freeButton(&buttonList[index]);
		}
		else
		{
			gameRoom->mode = DRAFT;
			changeBackgroundMusic();
			clearMayflySelection();
		}
	}
}

void mainButtonThink(int index)
{
	if (gameRoom->roomName != MAIN)
	{
		freeButton(&buttonList[index]);
	}

	if (checkClick(&buttonList[index]))
	{
		(*buttonList[index].effect)();
		changeBackgroundMusic();
		clearMayflySelection();
	}
}

void sortButtonThink(int index)
{
	if (gameRoom->roomName != MAIN)
	{
		freeButton(&buttonList[index]);
	}

	if (checkClick(&buttonList[index]))
	{
		(*buttonList[index].effect)();
	}
}

void toHeal()
{
	gameRoom->mode = HEAL;
	//clearMayflySelection();
}

void toTrain()
{
	gameRoom->mode = TRAIN;
	//clearMayflySelection();
}

void toBreed()
{
	gameRoom->mode = BREED;
	//clearMayflySelection();
}

void toScout()
{
	gameRoom->mode = SCOUT;
	//clearMayflySelection();
}

/**********************************************************************************************//**
 * @fn	void modeButtonThink(int index)
 *
 * @brief	Mode button think.
 *
 * @author	Camilo
 * @date	3/24/2015
 *
 * @param	index	Zero-based index of the.
 **************************************************************************************************/

/*void modeButtonThink(int index)
{
	if (gameRoom->roomName != MAIN)
	{
		buttonList[index].visible = 0;
		freeButton(&buttonList[index]);
	}
	else buttonList[index].visible = 1;

	if (checkClick(&buttonList[index]))
	{
		if (gameRoom->mode == DRAFT && mayflySelected > 0)
		{
			(*buttonList[index].effect)();
			freeButton(&buttonList[index]);
		}
		else gameRoom->mode = DRAFT;
	}
}*/

void menuButtonThink(int index)
{
	if (gameRoom->roomName != MENU)			freeButton(&buttonList[index]);
	if (checkClick(&buttonList[index]))
	{
		if (buttonList[index].effect != NULL) (*buttonList[index].effect)();
	}
}

void trainButtonThink(int index)
{
	if (gameRoom->mode != TRAIN || trainee == NULL)	freeButton(&buttonList[index]);
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
		if (buttonList[x].visible)
		{
			DrawSprite(buttonList[x].entity->image, screen, buttonList[x].entity->ex, buttonList[x].entity->ey, buttonList[x].entity->image->frame);
			if (buttonList[x].think != NULL) (*buttonList[x].think)(x);
			if (checkClick(&buttonList[x]))
			{
				buttonList[x].entity->image->frame = 1;
			}
			else buttonList[x].entity->image->frame = 0;
		}
	}
}