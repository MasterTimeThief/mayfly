#include "SDL_ttf.h"
#include "mayfly.h"
#include "graphics.h"
#include "mouse.h"

extern int entityTotal;

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
		return NULL;
        //exit(1);	
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


	m->visible = 1;
	m->selected = 0;
	mayflyTotal++;

	//Sprite and position
	m->entity = createEntity();
	m->entity->image = tempSprite;
	m->entity->ex = tempX;
	m->entity->ey = tempY;
	m->entity->maxSpeed = 45; //Bigger number means slower animation
	m->entity->frame = rand() % 4;
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

/*void mayflyDoNothing(Entity *e)
{
	return;
}
void UpdateMayflies()
{
	int i;
	for(i = 0;i < MAX_MAYFLIES;i++)

		(*mayflyList[i].think)(mayflyList[i].entity);
}*/
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

	temp = TTF_RenderText_Solid( mayFont, "Age: ", textColor );
	apply_surface(20,250,temp,screen,NULL);


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

	sprintf(tempString, "%i", m->age);
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

int checkSelected()
{
	int i,x;
	x = 0;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].selected == 1) x++;
	}
	return x;
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

	if (p1->health > p2->health)		child->health =	(rand() % p1->health + p2->health) + 1;
	else								child->health =	(rand() % p2->health + p1->health) + 1;

	if (p1->strength > p2->strength)	child->strength =	(rand() % p1->strength + p2->strength) + 1;
	else								child->strength =	(rand() % p2->strength + p1->strength) + 1;

	if (p1->speed > p2->speed)			child->speed =	(rand() % p1->speed + p2->speed) + 1;
	else								child->speed =	(rand() % p2->speed + p1->speed) + 1;

	if (p1->luck > p2->luck)			child->luck =	(rand() % p1->luck + p2->luck) + 1;
	else								child->luck =	(rand() % p2->luck + p1->luck) + 1;

	child->age = 0;

	child->archerExp	=	p1->archerExp + p2->archerExp;
	child->believerExp	=	p1->believerExp + p2->believerExp;
	child->soldierExp	=	p1->soldierExp + p2->soldierExp;
}

void mayflyAllThink(Room *r)
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

	if (r->mode == BREED && checkSelected() == 2) 
	{
		createMayflyOffspring();
		clearMayflySelection();
	}

	if (r->mode == BREED && checkSelected() == 1) 
	{
		//trainMayfly
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

		if (clickRight && !m->selected && !stopClick)
		{
			m->selected = 1;
			stopClick = 1;
		}
	}

	updateSprite(m->entity);
}