#include "SDL_ttf.h"
#include "mayfly.h"
#include "graphics.h"
#include "mouse.h"
#include "combat.h"

extern int entityTotal;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
Mayfly mayflyList[MAX_MAYFLIES];
int mayflyPositions[15][2];
int mayflySelected;

extern	SDL_Color c_Black;
extern  SDL_Color c_White;
extern	SDL_Surface *screen;
extern Room *gameRoom;

int mayflyTotal;
static int maxMayflies = 0;

static Mayfly *mouseMayfly = NULL;
extern int mx;
extern int my;

TTF_Font *mayFont;
Sprite *selectSpr;
Sprite *inactiveSpr;

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

	m->fighting = 0;
	m->visible	= 1;
	m->selected = 0;
	m->action	= 1;
	m->cx		= 0;
	m->cy		= 0;
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
			if (gameRoom->roomName == MAIN)
			{
				DrawSprite(mayflyList[i].entity->image, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->frame);
				if (mayflyList[i].selected) DrawSprite(selectSpr, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->frame);
				else if (!mayflyList[i].action) DrawSprite(inactiveSpr, screen, mayflyList[i].entity->ex, mayflyList[i].entity->ey, mayflyList[i].entity->frame);
			}
			if (gameRoom->roomName == COMBAT)
			{
				if (enemyList[i].fighting)
				{
					//change to fighting sprite, and fixed location
					DrawSprite(enemyList[i].entity->image, screen, 450, 200, enemyList[i].entity->frame);
				}
				else if (mayflyList[i].selected) DrawSprite(mayflyList[i].entity->image, screen, mayflyList[i].cx, mayflyList[i].cy, mayflyList[i].entity->frame);
			}
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



void mayflyCombatPosition(Mayfly *m, int index)
{
	m->cx = mayflyPositions[index][0];
	m->cy = mayflyPositions[index][1];
}

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
		mayflyList[i].selected = 0;
		mayflyCombatPosition(&mayflyList[i], j);
		mayflyFighters[j] = &mayflyList[i];
		j++;
	}
}

void displayMayflyStats(Mayfly *m)
{
	SDL_Surface *temp;
	char		tempString[3];

	//Display Stat Names
	temp = renderText( mayFont, "Health: ", c_Black );
	apply_surface(50,75,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Speed: ", c_Black );
	apply_surface(50,100,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Strength: ", c_Black );
	apply_surface(50,125,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Luck: ", c_Black );
	apply_surface(50,150,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Age: ", c_Black );
	apply_surface(50,175,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Believer: ", c_Black );
	apply_surface(50,225,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Archer: ", c_Black );
	apply_surface(50,250,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Soldier: ", c_Black );
	apply_surface(50,275,temp,screen,NULL);
	SDL_FreeSurface(temp);


	//Display Stat values
	sprintf(tempString, "%i", m->health);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,75,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", m->speed);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,100,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", m->strength);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,125,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", m->luck);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,150,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", m->age);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,175,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", m->believerExp);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,225,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", m->archerExp);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,250,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", m->soldierExp);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(150,275,temp,screen,NULL);
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
	mayflySelected = x;
	return x;
}

void trainMayfly()
{
	int i, numkeys;
	Mayfly *trainee;
	SDL_Surface *temp;

	Uint8 *keys = SDL_GetKeyState(&numkeys);
	trainee = NULL;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].selected == 1)
		{
			trainee = &mayflyList[i];
			break;
		}
	}

	temp = renderText( mayFont, "Train in Believer (1)", c_White );
	apply_surface(250,50,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Train in Archer  (2)",  c_White );
	apply_surface(250,100,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Train in Soldier  (3)", c_White );
	apply_surface(250,150,temp,screen,NULL);
	SDL_FreeSurface(temp);

	//check which key is pressed
	if (keys[SDLK_1])
	{
		trainee->believerExp += rand() % 10 + 1;
		trainee->action = 0;
		clearMayflySelection();
	}
	else if (keys[SDLK_2])
	{
		trainee->archerExp += rand() % 10 + 1;
		trainee->action = 0;
		clearMayflySelection();
	}
	else if (keys[SDLK_3])
	{
		trainee->soldierExp += rand() % 10 + 1;
		trainee->action = 0;
		clearMayflySelection();
	}
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

	if (p1->strength == p2->strength)		child->strength =	p1->health;
	else if (p1->strength > p2->strength)	child->strength =	(rand() % (p1->strength - p2->strength)) + p2->strength + 1;
	else									child->strength =	(rand() % (p2->strength - p1->strength)) + p1->strength + 1;

	if (p1->speed == p2->speed)				child->speed =		p1->health;
	else if (p1->speed > p2->speed)			child->speed =		(rand() % (p1->speed - p2->speed)) + p2->speed + 1;
	else									child->speed =		(rand() % (p2->speed - p1->speed)) + p1->speed + 1;

	if (p1->luck == p2->luck)				child->luck =		p1->health;
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

	if (r->mode == TRAIN && checkSelected() == 1)
	{
		trainMayfly();
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

		if (clickRight && !stopClick && m->action)
		{
			if		(!m->selected && checkSelected() < 15)	m->selected = 1;
			else if (m->selected)	m->selected = 0;
			stopClick = 1;
		}
		else if (!m->action)
		{
			m->selected = 0;
		}
	}

	updateSprite(m->entity);
}