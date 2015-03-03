#include "mayfly.h"
#include "graphics.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
Mayfly mayflyList[MAX_MAYFLIES];

int mayflyTotal;
static int maxMayflies = 0;
void initMayflyList()
{
	int x;
	mayflyTotal = 0;
	memset(mayflyList,0,sizeof(Mayfly) * MAX_MAYFLIES);
	for(x = 0; x < MAX_MAYFLIES; x++)
	{ 
		mayflyList[x].entity = NULL;
	}
}

Mayfly *newMayfly()
{
	int i;
	
	if (maxMayflies + 1 > MAX_MAYFLIES)
	{
		fprintf(stderr, "Maximum Mayflies Reached.\n");
        exit(1);	
	}
	for (i = 0;i < maxMayflies; i++)
	{
		if (!mayflyList[i].inUse)
		{
			mayflyList[i].inUse = 1;
			return &mayflyList[i];	
		}
	}
	mayflyList[maxMayflies].inUse = 1;
	return &mayflyList[maxMayflies++];
}

void setupMayfly(Mayfly *m)
{
	Sprite *tempSprite = (Sprite*)malloc(sizeof(Sprite));
	int tempX, tempY;

	int tempClass;

	//Class and Gender
	tempClass = rand() % 2;
	m->isFemale = rand() % 1;

	//Position
	/*tempX = rand() % (0.8 * SCREEN_WIDTH) + (0.1 * SCREEN_WIDTH);
	tempY = rand() % (0.8 * SCREEN_HEIGHT) + (0.1 * SCREEN_HEIGHT);*/
	tempX = rand() % 800 + 100;
	tempY = rand() % 400 + 50;

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

	m->health =		rand() % 10 + 11;
	m->strength =	rand() % 10 + 1;
	m->speed =		rand() % 10 + 1;
	m->luck =		rand() % 10 + 1;

	m->archerExp =	0;
	m->believerExp = 0;
	m->soldierExp =	0;


	m->visible = 1;
	mayflyTotal++;

	//Sprite and position
	m->entity = createEntity();
	m->entity->image = tempSprite;
	m->entity->ex = tempX;
	m->entity->ey = tempY;
}

void createMayfly()
{
	Mayfly *m = newMayfly();
	setupMayfly(m);
}

void createMayflyOffspring(Mayfly *m1, Mayfly *m2)
{
	Mayfly *m = newMayfly();
	setupMayfly(m);
	//genetic code
}

void displayMayflies(SDL_Surface *screen)
{
	int i;
	for (i = 0;i < 40; i++)
	{
		if (mayflyList[i].inUse && mayflyList[i].visible)
		{
			DrawSprite(mayflyList[i].entity->image, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->frame);
		}
	}
}

void freeMayfly(Mayfly *m)
{
	FreeSprite(m->entity->image);
	freeEntity(m->entity);
	m->inUse = 0;
	mayflyTotal--;
}

void closeMayflies()
{
	int i;
	for (i = 0;i < 40; i++)
	{
		if (mayflyList[i].entity != NULL)
		{
			freeMayfly(&mayflyList[i]);
		}
	}
}