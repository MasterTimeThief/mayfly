#include "SDL_ttf.h"
#include "mayfly.h"
#include "graphics.h"
#include "mouse.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
Mayfly mayflyList[MAX_MAYFLIES];

extern	SDL_Color textColor;
extern SDL_Surface *screen;

int mayflyTotal;
static int maxMayflies = 0;

static Mayfly *mouseMayfly = NULL;
extern int mx;
extern int my;

TTF_Font *mayFont;

void initMayflyList()
{
	int x;
	mayflyTotal = 0;
	memset(mayflyList,0,sizeof(Mayfly) * MAX_MAYFLIES);
	for(x = 0; x < MAX_MAYFLIES; x++)
	{ 
		mayflyList[x].entity = NULL;
	}
	mayFont = TTF_OpenFont("fonts/mayflyFont.ttf", 20);
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

void displayMayflies()
{
	int i;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].inUse && mayflyList[i].visible)
		{
			DrawSprite(mayflyList[i].entity->image, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->frame);
		}
	}
}

void displayMayflyStats(Mayfly *m)
{
	SDL_Surface *temp;
	char		tempString[2];

	//Display Stat Names
	temp = TTF_RenderText_Solid( mayFont, "Health: ", textColor );
	apply_surface(20,20,temp,screen,NULL);

	temp = TTF_RenderText_Solid( mayFont, "Speed: ", textColor );
	apply_surface(20,70,temp,screen,NULL);

	temp = TTF_RenderText_Solid( mayFont, "Strength: ", textColor );
	apply_surface(20,120,temp,screen,NULL);

	temp = TTF_RenderText_Solid( mayFont, "Luck: ", textColor );
	apply_surface(20,170,temp,screen,NULL);


	//Display Stat values
	sprintf(tempString, "%i", m->health);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,20,temp,screen,NULL);

	sprintf(tempString, "%i", m->speed);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,70,temp,screen,NULL);

	sprintf(tempString, "%i", m->strength);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,120,temp,screen,NULL);

	sprintf(tempString, "%i", m->luck);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,170,temp,screen,NULL);

	SDL_FreeSurface(temp);
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
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].entity != NULL)
		{
			freeMayfly(&mayflyList[i]);
		}
	}
}

void mayflyAllThink()
{
	int i;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].entity != NULL)
		{
			mayflyThink(&mayflyList[i]);
		}
	}
}

void mayflyThink(Mayfly *m)
{
	//Check if mouse collides with Mayfly
	if (mouseHover(m->entity->ex,  m->entity->ey,  m->entity->image->w,  m->entity->image->h))
	{
		displayMayflyStats(m);
		if (mouseMayfly == NULL && clickLeft)
		{
			mouseMayfly = m;
		}
		else if (!clickLeft) mouseMayfly = NULL;

	}

	if (mouseMayfly != NULL)
	{
		
		mouseMayfly->entity->ex = mx-16;
		mouseMayfly->entity->ey = my-16;
	}
}