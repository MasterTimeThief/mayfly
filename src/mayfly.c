#include "SDL_ttf.h"
#include "mayfly.h"
#include "graphics.h"
#include "mouse.h"
#include "combat.h"
#include "button.h"

extern int entityTotal;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
Mayfly mayflyList[MAX_MAYFLIES];
int mayflyPositions[15][2];
int mayflySelected;
Mayfly *trainee;

extern	SDL_Surface *screen;
extern Room *gameRoom;

int mayflyTotal;			//Always has current number in game
int maxMayflies = 0; //Goes up as number of mayflies exceed initial count

static Mayfly *mouseMayfly = NULL;
extern int mx;
extern int my;

Sprite *selectSpr;
Sprite *inactiveSpr;
Sprite *trainFrame;

void initMayflyList()
{
	int x;
	mayflyTotal = 0;
	memset(mayflyList,0,sizeof(Mayfly) * MAX_MAYFLIES);
	for(x = 0; x < MAX_MAYFLIES; x++)
	{ 
		mayflyList[x].entity = NULL;
	}

	setupMayflyCombatPositions();

	mayFont = TTF_OpenFont("fonts/mayflyFont.ttf", 30);
	selectSpr = LoadSprite("images/select.png",32,32);
	inactiveSpr = LoadSprite("images/inactive.png",32,32);
	trainFrame = LoadSprite("images/trainFrame.png",384,192);
	trainee = NULL;
}

void setupMayflyCombatPositions()
{
	int i;
	for (i = 0;i < 15; i++)
	{
		if		(i < 5)		mayflyPositions[i][0] = 96;
		else if (i < 10)	mayflyPositions[i][0] = 192;
		else if (i < 15)	mayflyPositions[i][0] = 288;

		if		(i % 5 == 0)	mayflyPositions[i][1] = 96;
		else if (i % 5 == 1)	mayflyPositions[i][1] = 192;
		else if (i % 5 == 2)	mayflyPositions[i][1] = 288;
		else if (i % 5 == 3)	mayflyPositions[i][1] = 384;
		else if (i % 5 == 4)	mayflyPositions[i][1] = 480;
	}
}

/**********************************************************************************************//**
 * @fn	Mayfly *newMayfly()
 *
 * @brief	Returns an empty mayfly list entry.
 *
 * @author	Camilo
 * @date	3/24/2015
 *
 * @return	null if it fails, else a Mayfly*.
 **************************************************************************************************/

Mayfly *newMayfly()
{
	int i;
	
	if (maxMayflies + 1 > MAX_MAYFLIES)
	{
		fprintf(stderr, "Maximum Mayflies Reached.\n");
		return NULL;
        //exit(1);	
	}
	for (i = 0;i < maxMayflies; i++)
	{
		if (!mayflyList[i].alive)
		{
			mayflyList[i].alive = 1;
			return &mayflyList[i];	
		}
	}
	mayflyList[maxMayflies].alive = 1;
	return &mayflyList[maxMayflies++];
}

/**********************************************************************************************//**
 * @fn	void setupMayfly(Mayfly *m)
 *
 * @brief	Sets up the mayfly.
 *
 * @author	Camilo
 * @date	3/24/2015
 *
 * @param [in,out]	m	If non-null, the Mayfly * to process.
 **************************************************************************************************/

void setupMayfly(Mayfly *m)
{
	Sprite *tempSprite = (Sprite*)malloc(sizeof(Sprite));
	int tempX, tempY;
	int i;

	int tempClass;

	//Class and Gender
	tempClass = rand() % 3;
	m->isFemale = rand() % 2;
	//m->think = mayflyDoNothing;
	//Position
	tempX = rand() % 700 + 256;
	tempY = rand() % 220 + 288;

	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].entity != NULL && (tempX+16) > mayflyList[i].entity->ex && (tempX+16) < mayflyList[i].entity->ex + mayflyList[i].entity->image->w && (tempY+16) > mayflyList[i].entity->ey && (tempY+16) < mayflyList[i].entity->ey + mayflyList[i].entity->image->h)
		{
			tempX = rand() % 700 + 256;
			tempY = rand() % 220 + 288;
			i = 0;
		}
	}

	//Load proper sprite
	if (tempClass == 0)
	{
		m->currClass = BELIEVER;
		if (m->isFemale) tempSprite = LoadSprite("images/believer_f.png",32,32);
		else				tempSprite = LoadSprite("images/believer_m.png",32,32);
	}
	else if (tempClass == 1)
	{
		m->currClass = ARCHER;
		if (m->isFemale) tempSprite = LoadSprite("images/archer_f.png",32,32);
		else				tempSprite = LoadSprite("images/archer_m.png",32,32);
	}
	else if (tempClass == 2)
	{
		m->currClass = SOLDIER;
		if (m->isFemale) tempSprite = LoadSprite("images/soldier_f.png",32,32);
		else				tempSprite =LoadSprite("images/soldier_m.png",32,32);
	}

	m->age = 1;

	m->health =		rand() % 10 + 11;
	m->strength =	rand() % 10 + 1;
	m->speed =		rand() % 10 + 1;
	m->luck =		rand() % 10 + 1;

	m->archerExp =	0;
	m->believerExp = 0;
	m->soldierExp =	0;

	m->fighting = 0;
	m->visible	= 1;
	m->selected = 0;
	m->action	= 1;
	m->cx		= 0;
	m->cy		= 0;
	//mayflyTotal++;

	//Sprite and position
	m->entity = createEntity();
	m->entity->image = tempSprite;
	m->entity->ex = tempX;
	m->entity->ey = tempY;
	m->entity->image->currSpeed = 0;
	m->entity->image->maxSpeed = 500; //Bigger number means slower animation
	m->entity->image->frame = rand() % 4;
}

void createMayfly()
{
	Mayfly *m = newMayfly();
	if (m != NULL)
	{
		setupMayfly(m);
	}
}

void createMayflyOffspring()
{
	Mayfly *m = newMayfly();
	if (m != NULL)
	{
		setupMayfly(m);
		setupMayflyOffspring(m);
	}
}

/**********************************************************************************************//**
 * @fn	void displayMayflies()
 *
 * @brief	Displays the mayflies.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void displayMayflies()
{
	int i;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].alive && mayflyList[i].visible)
		{
			if (gameRoom->roomName == MAIN)
			{
				DrawSprite(mayflyList[i].entity->image, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->image->frame);
				if (mayflyList[i].selected) DrawSprite(selectSpr, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->image->frame);
				else if (!mayflyList[i].action) DrawSprite(inactiveSpr, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->image->frame);
			}
			if (gameRoom->roomName == COMBAT)
			{
				if (mayflyList[i].fighting)
				{
					//change to fighting sprite, and fixed location
					DrawSprite(mayflyList[i].entity->image, screen, 450, 200, mayflyList[i].entity->image->frame);
				}
				else if (mayflyList[i].selected) DrawSprite(mayflyList[i].entity->image, screen, mayflyList[i].cx, mayflyList[i].cy, mayflyList[i].entity->image->frame);
			}
		}
	}
	if (trainee != NULL)
	{
		DrawSprite(trainFrame, screen, 256, 64, 0);
		DrawSprite(trainee->entity->image, screen, 432, 80, trainee->entity->image->frame);
	} 
}

void mayflyAfterCombat()
{
	int i;
	for (i = 0;i < maxMayflies; i++)
	{
		mayflyList[i].visible = 1;
		mayflyList[i].action = 1;
		mayflyList[i].age++;
		if (mayflyList[i].age == 7) freeMayfly(&mayflyList[i]); //Kill Function
	}
}

/**********************************************************************************************//**
 * @fn	void mayflyCombatPosition(Mayfly *m, int index)
 *
 * @brief	Sets up Mayfly combat position.
 *
 * @author	Camilo
 * @date	3/24/2015
 *
 * @param [in,out]	m	If non-null, the Mayfly * to process.
 * @param	index	 	Zero-based index of the combat position to use.
 **************************************************************************************************/

void mayflyCombatPosition(Mayfly *m, int index)
{
	m->cx = mayflyPositions[index][0];
	m->cy = mayflyPositions[index][1];
}

/**********************************************************************************************//**
 * @fn	void setupMayflyCombat()
 *
 * @brief	Sets up the mayfly combat.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void setupMayflyCombat()
{
	int i,j;
	for (i = 0, j = 0;i < maxMayflies; i++)
	{
		if (!mayflyList[i].selected)
		{
			mayflyList[i].visible = 0;
			continue;
		}
		//mayflyList[i].selected = 0;
		mayflyCombatPosition(&mayflyList[i], j);
		mayflyFighters[j] = &mayflyList[i];
		j++;
	}
}

/**********************************************************************************************//**
 * @fn	void displayMayflyStats(Mayfly *m)
 *
 * @brief	Displays a mayfly statistics described by m.
 *
 * @author	Camilo
 * @date	3/24/2015
 *
 * @param [in,out]	m	If non-null, the Mayfly * to process.
 **************************************************************************************************/

void displayMayflyStats(Mayfly *m)
{
	//Display Stat Names
	printString("Health: ",		c_Black, screen, 50, 75);
	printString("Speed: ",		c_Black, screen, 50, 100);
	printString("Strength: ",	c_Black, screen, 50, 125);
	printString("Luck: ",		c_Black, screen, 50, 150);
	printString("Age: ",		c_Black, screen, 50, 175);

	printString("Believer: ",	c_Black, screen, 50, 225);
	printString("Archer: ",		c_Black, screen, 50, 250);
	printString("Soldier: ",	c_Black, screen, 50, 275);

	//Display Stat values
	if (m->health <= 5)		printInt(m->health,	c_Red, screen, 150, 75);
	else					printInt(m->health,	c_Black, screen, 150, 75);
	if (m->speed <= 3)		printInt(m->speed,	c_Red, screen, 150, 100);
	else					printInt(m->speed,	c_Black, screen, 150, 100);
	if (m->strength <= 3)	printInt(m->strength,c_Red, screen, 150, 125);
	else					printInt(m->strength,c_Black, screen, 150, 125);
	if (m->luck <= 3)		printInt(m->luck,	c_Red, screen, 150, 150);
	else					printInt(m->luck,	c_Black, screen, 150, 150);

	if (m->age >= 5)		printInt(m->age,c_Red, screen, 150, 175);
	else					printInt(m->age,c_Black, screen, 150, 175);

	printInt(m->believerExp,c_Black, screen, 150, 225);
	printInt(m->archerExp,	c_Black, screen, 150, 250);
	printInt(m->soldierExp,	c_Black, screen, 150, 275);
}

/**********************************************************************************************//**
 * @fn	void freeMayfly(Mayfly *m)
 *
 * @brief	Free mayfly.
 *
 * @author	Camilo
 * @date	3/24/2015
 *
 * @param [in,out]	m	If non-null, the Mayfly * to process.
 **************************************************************************************************/

void freeMayfly(Mayfly *m)
{
	FreeSprite(m->entity->image);
	freeEntity(m->entity);
	m->alive = 0;
	m->age = 0;

	m->health =	0;
	m->strength = 0;
	m->speed = 0;
	m->luck = 0;

	m->archerExp =	0;
	m->believerExp = 0;
	m->soldierExp =	0;

	m->fighting = 0;
	m->visible	= 0;
	m->selected = 0;
	m->action	= 0;
	m->cx		= 0;
	m->cy		= 0;
}

void closeMayflies()
{
	int i;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].entity != NULL)
		{
			freeMayfly(&mayflyList[i]);
		}
	}
}

void clearMayflySelection()
{
	int i;
	for (i = 0;i < maxMayflies; i++)
	{
		mayflyList[i].selected = 0;
	}
}

/**********************************************************************************************//**
 * @fn	void checkSelected()
 *
 * @brief	Check how many Mayflies are selected.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void checkSelected()
{
	int i;
	mayflySelected = 0;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].selected == 1) mayflySelected++;
	}
}

/**********************************************************************************************//**
 * @fn	void trainBeliever()
 *
 * @brief	Train believer.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void trainBeliever()
{
	trainee->believerExp += rand() % 10 + 1;
	trainee->action = 0;
	trainee->selected = 0;
	trainee = NULL;
}

/**********************************************************************************************//**
 * @fn	void trainArcher()
 *
 * @brief	Train archer.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void trainArcher()
{
	trainee->archerExp += rand() % 10 + 1;
	trainee->action = 0;
	trainee->selected = 0;
	trainee = NULL;
}

/**********************************************************************************************//**
 * @fn	void trainSoldier()
 *
 * @brief	Train soldier.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void trainSoldier()
{
	trainee->soldierExp += rand() % 10 + 1;
	trainee->action = 0;
	trainee->selected = 0;
	trainee = NULL;
}

void trainMayfly()
{
	int i, numkeys;

	Uint8 *keys = SDL_GetKeyState(&numkeys);
	//trainee = NULL;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].selected == 1)
		{
			trainee = &mayflyList[i];
			break;
		}
	}

	//if (keys[SDLK_s]) saveToList();

	createButton(224, 160, 128, 64, "images/belButton.png", (*trainButtonThink), trainBeliever);
	createButton(384, 192, 128, 64, "images/archButton.png", (*trainButtonThink), trainArcher);
	createButton(544, 160, 128, 64, "images/solButton.png", (*trainButtonThink), trainSoldier);
}

void mayflyExperience(Mayfly *m)
{
	if (m->currClass == BELIEVER) m->believerExp += rand() % 10;
	else if (m->currClass == ARCHER) m->archerExp += rand() % 10;
	else if (m->currClass == SOLDIER) m->soldierExp += rand() % 10;
}

void healMayfly(Mayfly *m)
{
	m->health += rand() % 5 + 3;
	m->action = 0;
}

void setupMayflyOffspring(Mayfly *child)
{
	int i;
	Mayfly *p1, *p2;

	p1 = NULL;
	p2 = NULL;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].selected == 1)
		{
			if (p1 == NULL) p1 = &mayflyList[i];
			else if (p2 == NULL)
			{
				p2 = &mayflyList[i];
				break;
			}
		}
	}

	if (p1->health == p2->health)			child->health =		p1->health;
	else if (p1->health > p2->health)		child->health =		(rand() % (p1->health - p2->health)) + p2->health + 1;
	else									child->health =		(rand() % (p2->health - p1->health)) + p1->health + 1;

	if (p1->strength == p2->strength)		child->strength =	p1->strength;
	else if (p1->strength > p2->strength)	child->strength =	(rand() % (p1->strength - p2->strength)) + p2->strength + 1;
	else									child->strength =	(rand() % (p2->strength - p1->strength)) + p1->strength + 1;

	if (p1->speed == p2->speed)				child->speed =		p1->speed;
	else if (p1->speed > p2->speed)			child->speed =		(rand() % (p1->speed - p2->speed)) + p2->speed + 1;
	else									child->speed =		(rand() % (p2->speed - p1->speed)) + p1->speed + 1;

	if (p1->luck == p2->luck)				child->luck =		p1->luck;
	else if (p1->luck > p2->luck)			child->luck =		(rand() % (p1->luck - p2->luck)) + p2->luck + 1;
	else									child->luck =		(rand() % (p2->luck - p1->luck)) + p1->luck + 1;

	child->age = 0;

	child->archerExp	=	p1->archerExp + p2->archerExp;
	child->believerExp	=	p1->believerExp + p2->believerExp;
	child->soldierExp	=	p1->soldierExp + p2->soldierExp;

	p1->action = 0;
	p2->action = 0;
	child->action = 0;
}

void sortMayflyClass()
{
	int i;

	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].currClass == BELIEVER)
		{
			mayflyList[i].entity->ex = rand() % 200 + 256;
			mayflyList[i].entity->ey = rand() % 220 + 288;
		}
		else if (mayflyList[i].currClass == ARCHER)
		{
			mayflyList[i].entity->ex = rand() % 200 + 506;
			mayflyList[i].entity->ey = rand() % 220 + 288;
		}
		else if (mayflyList[i].currClass == SOLDIER)
		{
			mayflyList[i].entity->ex = rand() % 200 + 756;
			mayflyList[i].entity->ey = rand() % 220 + 288;
		}
	}
}

void sortMayflyGender()
{
	int i;

	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].isFemale)
		{
			mayflyList[i].entity->ex = rand() % 300 + 256;
			mayflyList[i].entity->ey = rand() % 220 + 288;
		}
		else
		{
			mayflyList[i].entity->ex = rand() % 300 + 656;
			mayflyList[i].entity->ey = rand() % 220 + 288;
		}
	}
}

void sortMayflyAge()
{
	int i;

	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].age < 4)
		{
			mayflyList[i].entity->ex = rand() % 300 + 256;
			mayflyList[i].entity->ey = rand() % 220 + 288;
		}
		else
		{
			mayflyList[i].entity->ex = rand() % 300 + 656;
			mayflyList[i].entity->ey = rand() % 220 + 288;
		}
	}
}

void mayflyAllThink(Room *r)
{
	int i;

	mayflyTotal = 0;
	
	checkSelected();
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].entity != NULL && mayflyList[i].alive)
		{
			mayflyTotal++;
			mayflyThink(&mayflyList[i]);
		}
	}
	
	if (mouseMayfly != NULL)
	{
		if (mx-16 < 256)		mouseMayfly->entity->ex = 256;
		else if (mx-16 > 956)	mouseMayfly->entity->ex = 956;
		else					mouseMayfly->entity->ex = mx-16;

		if (my-16 < 288)		mouseMayfly->entity->ey = 288;
		else if (my-16 > 508)	mouseMayfly->entity->ey = 508;
		else					mouseMayfly->entity->ey = my-16;
	}

	if (r->mode == BREED && mayflySelected == 2) 
	{
		createMayflyOffspring();
		clearMayflySelection();
	}

	if (r->mode == TRAIN && mayflySelected == 1)
	{
		if (trainee == NULL) trainMayfly();
	}
	else trainee = NULL;
}

void mayflyThink(Mayfly *m)
{
	//Check if mouse collides with Mayfly
	if (mouseHover(m->entity->ex,  m->entity->ey,  m->entity->image->w,  m->entity->image->h))
	{
		if (gameRoom->roomName == MAIN && m->alive) displayMayflyStats(m);
		if (clickLeft)
		{
			if (mouseMayfly == NULL) mouseMayfly = m;
			else if (mouseMayfly == m) mouseMayfly = NULL;
		}
		else if (!clickLeft) mouseMayfly = NULL;

		if (clickRight && !stopClick && m->action)
		{
			if (gameRoom->mode == HEAL) healMayfly(m);
			else
			{
				
				if	(!m->selected && mayflySelected < 15)
				{
					if (gameRoom->mode != TRAIN || mayflySelected == 0) m->selected = 1;
				}
				else if (m->selected)	m->selected = 0;
				stopClick = 1;
			}
		}
		else if (!m->action)
		{
			m->selected = 0;
		}
	}

	if(mouseHover(m->cx,  m->cy,  m->entity->image->w,  m->entity->image->h) && gameRoom->roomName == COMBAT)
	{
		if (clickLeft)
		{
			mayflyFightChoose(m);
		}
	}

	updateSprite(m->entity->image);
}