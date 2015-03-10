#include "SDL_ttf.h"
#include "mayfly.h"
#include "graphics.h"
#include "mouse.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
Mayfly mayflyList[MAX_MAYFLIES];
Mayfly mayflySelect[15];

extern	SDL_Color textColor;
extern	SDL_Surface *screen;

int mayflyTotal;
static int maxMayflies = 0;

static Mayfly *mouseMayfly = NULL;
extern int mx;
extern int my;

TTF_Font *mayFont;
Sprite *selectSpr;

void initMayflyList()
{
	int x;
	mayflyTotal = 0;
	memset(mayflyList,0,sizeof(Mayfly) * MAX_MAYFLIES);
	memset(mayflySelect,0,sizeof(Mayfly) * 15);
	for(x = 0; x < MAX_MAYFLIES; x++)
	{ 
		mayflyList[x].entity = NULL;
		if (x < 15)	mayflySelect[x].entity = NULL;
	}
	mayFont = TTF_OpenFont("fonts/mayflyFont.ttf", 30);
	selectSpr = LoadSprite("images/select.png",32,32);
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
	int i;

	int tempClass;

	//Class and Gender
	tempClass = rand() % 3;
	m->isFemale = rand() % 2;

	//Position
	/*tempX = rand() % (0.8 * SCREEN_WIDTH) + (0.1 * SCREEN_WIDTH);
	tempY = rand() % (0.8 * SCREEN_HEIGHT) + (0.1 * SCREEN_HEIGHT);*/
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

	m->health =		rand() % 10 + 11;
	m->strength =	rand() % 10 + 1;
	m->speed =		rand() % 10 + 1;
	m->luck =		rand() % 10 + 1;

	m->archerExp =	0;
	m->believerExp = 0;
	m->soldierExp =	0;


	m->visible = 1;
	m->selected = 0;
	mayflyTotal++;

	//Sprite and position
	m->entity = createEntity();
	m->entity->image = tempSprite;
	m->entity->ex = tempX;
	m->entity->ey = tempY;
	m->entity->maxSpeed = 5;
	m->entity->frame = rand() % 4;
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
			if (mayflyList[i].selected) DrawSprite(selectSpr, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->frame);
		}
	}
}

void displayMayflyStats(Mayfly *m)
{
	SDL_Surface *temp;
	char		tempString[3];

	//Display Stat Names
	temp = TTF_RenderText_Solid( mayFont, "Health: ", textColor );
	apply_surface(20,50,temp,screen,NULL);

	temp = TTF_RenderText_Solid( mayFont, "Speed: ", textColor );
	apply_surface(20,100,temp,screen,NULL);

	temp = TTF_RenderText_Solid( mayFont, "Strength: ", textColor );
	apply_surface(20,150,temp,screen,NULL);

	temp = TTF_RenderText_Solid( mayFont, "Luck: ", textColor );
	apply_surface(20,200,temp,screen,NULL);


	//Display Stat values
	sprintf(tempString, "%i", m->health);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,50,temp,screen,NULL);

	sprintf(tempString, "%i", m->speed);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,100,temp,screen,NULL);

	sprintf(tempString, "%i", m->strength);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,150,temp,screen,NULL);

	sprintf(tempString, "%i", m->luck);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,200,temp,screen,NULL);

	sprintf(tempString, "%i", clickRight);
	temp = TTF_RenderText_Solid( mayFont, tempString, textColor );
	apply_surface(150,250,temp,screen,NULL);

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

void clearMayflySelection()
{
	int i;
	for (i = 0;i < maxMayflies; i++)
	{
		mayflyList[i].selected = 0;
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
	
	if (mouseMayfly != NULL)
	{
		if (mx-16 < 256)		mouseMayfly->entity->ex = 256;
		else if (mx-16 > 956)	mouseMayfly->entity->ex = 956;
		else					mouseMayfly->entity->ex = mx-16;

		if (my-16 < 288)		mouseMayfly->entity->ey = 288;
		else if (my-16 > 508)	mouseMayfly->entity->ey = 508;
		else					mouseMayfly->entity->ey = my-16;
	}
}

void mayflyThink(Mayfly *m)
{
	//Check if mouse collides with Mayfly
	if (mouseHover(m->entity->ex,  m->entity->ey,  m->entity->image->w,  m->entity->image->h))
	{
		displayMayflyStats(m);
		if (clickLeft)
		{
			if (mouseMayfly == NULL) mouseMayfly = m;
			else if (mouseMayfly == m) mouseMayfly == NULL;
		}
		else if (!clickLeft) mouseMayfly = NULL;

		if (clickRight && !m->selected)
		{
			m->selected = 1;
		}
	}

	updateSprite(m->entity);
}