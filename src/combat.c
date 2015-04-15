#include "combat.h"
#include "event.h"
#include "button.h"

Enemy enemyList[MAX_ENEMIES];
int enemyTotal;

int currFighters;
int currentCombat;
int numKilled, numLost;

//Mayfly *mayflyFighters[15];
//Enemy *enemyFighters[15];
extern	SDL_Surface *screen;
extern Room *gameRoom;
extern char *mainBack;
extern char *combatBack;

void initEnemyList()
{
	int i;
	enemyTotal = MAX_ENEMIES;
	memset(enemyList,0,sizeof(Enemy) * MAX_ENEMIES);
	for(i = 0; i < MAX_ENEMIES; i++)
	{ 
		createEnemy();
	}
	for (i = 0; i < 15; i++)
	{
		mayflyFighters[i] = NULL;
		enemyFighters[i]  = NULL;
	}
}

Enemy *newEnemy()
{
	int i;
	for (i = 0;i < MAX_ENEMIES; i++)
	{
		if (!enemyList[i].alive)
		{
			enemyList[i].alive = 1;
			return &enemyList[i];	
		}
	}
	return NULL;
}

void setupEnemy(Enemy *m)
{
	Sprite *tempSprite = (Sprite*)malloc(sizeof(Sprite));

	int tempClass;

	//Class and Gender
	tempClass = rand() % 3;
	m->isFemale = rand() % 2;
	//m->think = mayflyDoNothing;

	//Load proper sprite
	if (tempClass == 0)
	{
		strncpy(m->currClass,"believer",10);
		if (m->isFemale) tempSprite = LoadSprite("images/believer_f.png",32,32);
		else				tempSprite = LoadSprite("images/believer_m.png",32,32);
	}
	else if (tempClass == 1)
	{
		strncpy(m->currClass,"archer",10);
		if (m->isFemale) tempSprite = LoadSprite("images/archer_f.png",32,32);
		else				tempSprite = LoadSprite("images/archer_m.png",32,32);
	}
	else if (tempClass == 2)
	{
		strncpy(m->currClass,"soldier",10);
		if (m->isFemale) tempSprite = LoadSprite("images/soldier_f.png",32,32);
		else				tempSprite =LoadSprite("images/soldier_m.png",32,32);
	}

	m->health =		rand() % 10 + 11;
	m->strength =	rand() % 10 + 1;
	m->speed =		rand() % 10 + 1;
	m->luck =		rand() % 10 + 1;

	m->combat = 0;
	m->fighting = 0;

	//Sprite and position
	m->entity = createEntity();
	m->entity->image = tempSprite;
	m->entity->ex = 0;
	m->entity->ey = 0;
	m->entity->image->currSpeed = 0;
	m->entity->image->maxSpeed = 60; //Bigger number means slower animation
	m->entity->image->frame = rand() % 4;
}

void createEnemy()
{
	Enemy *m = newEnemy();
	if (m != NULL)
	{
		setupEnemy(m);
	}
	
}

void displayEnemies()
{
	int i;
	for (i = 0;i < MAX_ENEMIES; i++)
	{
		if (enemyList[i].alive)
		{
			if (enemyList[i].fighting)
			{
				//change to fighting sprite, and fixed location
				DrawSprite(enemyList[i].entity->image, screen, 550, 200, enemyList[i].entity->image->frame);
			}
			else if (enemyList[i].combat)
			{
				DrawSprite(enemyList[i].entity->image, screen, enemyList[i].entity->ex, enemyList[i].entity->ey, enemyList[i].entity->image->frame);
			}
		} 
	}
}

void setupEnemyCombat()
{
	int i,j;

	currFighters = rand() % 10 + 5;

	for (i = 0, j = 0; j <= currFighters; i++)
	{
		if (!enemyList[i].alive)
		{
			continue;
		}
		enemyList[i].entity->ex = rand() % 300 + 650;
		enemyList[i].entity->ey = rand() % 400 + 50;
		enemyList[i].combat = 1;
		enemyFighters[j] = &enemyList[i];
		j++;
		//position enemy
	}
}

void enemyThinkAll(Room *r)
{
	int i;
	for (i = 0;i < MAX_ENEMIES; i++)
	{
		if (enemyList[i].entity != NULL)
		{
			enemyThink(&enemyList[i]);
		}
	}
}

void enemyThink(Enemy *e)
{
	updateSprite(e->entity->image);
}

void toCombat()
{
	Event *combatStart = newEvent();
	if (gameRoom->mode == DRAFT && mayflySelected >= 1)
	{
		gameRoom->roomName = COMBAT;
		setupEnemyCombat();
		setupMayflyCombat();
		changeBackground(gameRoom, combatBack);

		currentCombat = 0;
		numKilled = 0;
		numLost = 0;

		combatStart->timer = 300; // Time before combat starts
		combatStart->end = chooseFighters;
	}
}

void endCombat()
{
	int i;
	for (i = 0; i < 15; i++)
	{
		if (mayflyFighters[i] != NULL)
		{
			mayflyFighters[i]->fighting = 0;
			mayflyFighters[i]->selected = 0;
			mayflyFighters[i] = NULL;
		}

		if (enemyFighters[i] != NULL)
		{
			enemyFighters[i]->combat = 0;
			enemyFighters[i]->fighting = 0;
			enemyFighters[i]  = NULL;
		}
	}
	mayflyAfterCombat();
	gameRoom->roomName = MAIN;
	//createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
	mainButtons();
	changeBackground(gameRoom, mainBack);
}

void resetFighters()
{
	Event *nextFight = newEvent();

	mayflyFighters[currentCombat]->fighting = 0;
	enemyFighters[currentCombat]->fighting = 0;

	currentCombat++;
	
	nextFight->timer = 400; // time between fights
	nextFight->end = chooseFighters;
}

/**********************************************************************************************//**
 * @fn	void mayflyAttack()
 *
 * @brief	Mayfly attack.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void mayflyAttack()
{
	Event *nextAction = newEvent();
	int tempCrit;
	int myCrit = 0;
	int myAdv = 0;

	if		(mayflyFighters[currentCombat]->currClass == BELIEVER && enemyFighters[currentCombat]->currClass == "soldier") myAdv = rand() % 4;
	else if (mayflyFighters[currentCombat]->currClass == SOLDIER && enemyFighters[currentCombat]->currClass == "archer")	 myAdv = rand() % 4;
	else if (mayflyFighters[currentCombat]->currClass == ARCHER && enemyFighters[currentCombat]->currClass == "believer")  myAdv = rand() % 4;
	
	tempCrit = rand() % 30;
	if (tempCrit < mayflyFighters[currentCombat]->luck) myCrit = rand() % 5;

	enemyFighters[currentCombat]->health -= ( mayflyFighters[currentCombat]->strength + myAdv + myCrit);
	if (enemyFighters[currentCombat]->health <= 0)
	{
		enemyFighters[currentCombat]->alive = 0;
		numKilled++;
		enemyTotal--;
		mayflyExperience(mayflyFighters[currentCombat]);
		nextAction->end = resetFighters;
	}
	else
	{
		nextAction->end = enemyAttack;
	}
	nextAction->timer = 1;
}

/**********************************************************************************************//**
 * @fn	void enemyAttack()
 *
 * @brief	Enemy attack.
 *
 * @author	Camilo
 * @date	3/24/2015
 **************************************************************************************************/

void enemyAttack()
{
	Event *nextAction = newEvent();
	int tempCrit;
	int enemyCrit = 0;
	int enemyAdv = 0;

	if		(enemyFighters[currentCombat]->currClass == "believer" && mayflyFighters[currentCombat]->currClass == SOLDIER) enemyAdv = rand() % 4;
	else if (enemyFighters[currentCombat]->currClass == "soldier" && mayflyFighters[currentCombat]->currClass == ARCHER)	 enemyAdv = rand() % 4;
	else if (enemyFighters[currentCombat]->currClass == "archer" && mayflyFighters[currentCombat]->currClass == BELIEVER)  enemyAdv = rand() % 4;
	
	tempCrit = rand() % 30;
	if (tempCrit < enemyFighters[currentCombat]->luck) enemyCrit = rand() % 5;

	mayflyFighters[currentCombat]->health -= ( enemyFighters[currentCombat]->strength + enemyAdv + enemyCrit );
	if (mayflyFighters[currentCombat]->health <= 0)
	{
		freeMayfly(mayflyFighters[currentCombat]);
		numLost++;
		nextAction->end = resetFighters;
	}
	else
	{
		nextAction->end = mayflyAttack; //changed the semicolon, greek question mark
	}
	nextAction->timer = 1;
}

void displayResults()
{
	SDL_Surface *temp;
	char		tempString[3];

	//Display Stat Names
	temp = renderText( mayFont, "Mayflies Lost: ", c_Black );
	apply_surface(400,200,temp,screen,NULL);
	SDL_FreeSurface(temp);

	temp = renderText( mayFont, "Enemies Killed: ", c_Black );
	apply_surface(400,250,temp,screen,NULL);
	SDL_FreeSurface(temp);

	//Display Stat values
	sprintf(tempString, "%i", numLost);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(580,200,temp,screen,NULL);
	SDL_FreeSurface(temp);

	sprintf(tempString, "%i", numKilled);
	temp = renderText( mayFont, tempString, c_Black );
	apply_surface(580,250,temp,screen,NULL);
	SDL_FreeSurface(temp);
}

void chooseFighters()
{
	Event *fightResult = newEvent();
	
	//currentCombat keeps track of who's fighting
	if (currentCombat > 15 || mayflyFighters[currentCombat] == NULL || enemyFighters[currentCombat] == NULL)
	{
		fightResult->timer = 1500;
		fightResult->think = displayResults;
		fightResult->end = endCombat;
		return;
	}

	mayflyFighters[currentCombat]->fighting = 1;
	enemyFighters[currentCombat]->fighting = 1;

	fightResult->timer = 300; //Time in combat
	if		(mayflyFighters[currentCombat]->speed >= enemyFighters[currentCombat]->speed) fightResult->end = mayflyAttack;
	else if (enemyFighters[currentCombat]->speed >  mayflyFighters[currentCombat]->speed) fightResult->end = enemyAttack;
}