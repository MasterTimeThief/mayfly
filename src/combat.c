#include "combat.h"
#include "event.h"
#include "button.h"
#include "audio.h"

//Enemy enemyList[MAX_ENEMIES];
//int enemyTotal;

int currFighters;
int currentCombat;
int numKilled, numLost;
int currentDamage, currentFighter, currentCrit;


extern	SDL_Surface *screen;
extern Room *gameRoom;
extern char *mainBack;
extern char *combatBack;

Mayfly *myFighter;
Enemy *enemyFighter;

void initEnemyList()
{
	int i;
	enemyTotal = 0;
	memset(enemyList,0,sizeof(Enemy) * MAX_ENEMIES);
	for(i = 0; i < MAX_ENEMIES; i++)
	{ 
		enemyList[i].alive = 0;
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
				printInt(enemyList[i].health, c_Red, screen, 615, 200);
			}
			else if (enemyList[i].combat)
			{
				DrawSprite(enemyList[i].entity->image, screen, enemyList[i].entity->ex, enemyList[i].entity->ey, enemyList[i].entity->image->frame);
			}
		} 
	}
}

void freeEnemy(Enemy *e)
{
	FreeSprite(e->entity->image);
	freeEntity(e->entity);
	e->alive = 0;
	enemyTotal--;
}

void closeEnemies()
{
	int i;
	for (i = 0;i < MAX_ENEMIES; i++)
	{
		if (enemyList[i].entity != NULL)
		{
			freeEnemy(&enemyList[i]);
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
	enemyTotal = 0;
	for (i = 0;i < MAX_ENEMIES; i++)
	{
		if (enemyList[i].entity != NULL)
		{
			if (enemyList[i].alive) enemyTotal++;
			enemyThink(&enemyList[i]);
		}
	}
}

void enemyThink(Enemy *e)
{
	updateSprite(e->entity->image);
}

void displayAttack()
{
	if (currentCrit > 0) printString("CRITICAL HIT",	c_Red, screen, 400, 150);//print the word CRITICAL HIT

	if (currentFighter == 1) //mayfly attack
	{
		printString("Mayfly attacked for    !",	c_Black, screen, 400, 250);
		printInt(currentDamage, c_Red, screen, 615, 250);
	}
	else if (currentFighter == 2) //enemy attack
	{
		printString("Enemy attacked for     !",	c_Black, screen, 400, 250);
		printInt(currentDamage, c_Red, screen, 615, 250);
	}


}

void toCombat()
{
	Event *combatStart = newEvent();
	if (gameRoom->mode == DRAFT && mayflySelected >= 1)
	{
		gameRoom->roomName = COMBAT;
		setupEnemyCombat();
		setupMayflyCombat();
		changeBackground(combatBack);
		changeBackgroundMusic();

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
	changeBackgroundMusic();
	//createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
	mainButtons();
	changeBackground(mainBack);
}

void resetFighters()
{
	Event *nextFight = newEvent();

	myFighter->fighting = 0;
	enemyFighter->fighting = 0;
	myFighter = NULL;
	enemyFighter = NULL;

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
	int myAdv = 0;

	currentFighter = 1;
	if		(myFighter->currClass == BELIEVER && enemyFighter->currClass == "soldier") myAdv = rand() % 4;
	else if (myFighter->currClass == SOLDIER && enemyFighter->currClass == "archer")	 myAdv = rand() % 4;
	else if (myFighter->currClass == ARCHER && enemyFighter->currClass == "believer")  myAdv = rand() % 4;
	
	tempCrit = rand() % 30;
	currentCrit = 0;
	if (tempCrit < myFighter->luck) currentCrit = rand() % 4 + 1;

	currentDamage = myFighter->strength + myAdv + currentCrit;
	enemyFighter->health -= currentDamage;
	nextAction->think = displayAttack;

	if (enemyFighter->health <= 0)
	{
		freeEnemy(enemyFighter);
		numKilled++;
		mayflyExperience(myFighter);
		nextAction->end = resetFighters;
	}
	else
	{
		
		nextAction->end = enemyAttack;
	}
	nextAction->timer = 300;
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
	//int enemyCrit = 0;
	int enemyAdv = 0;
	//int enemyDamage;

	currentFighter = 2;
	if		(enemyFighter->currClass == "believer" && myFighter->currClass == SOLDIER) enemyAdv = rand() % 4;
	else if (enemyFighter->currClass == "soldier" && myFighter->currClass == ARCHER)	 enemyAdv = rand() % 4;
	else if (enemyFighter->currClass == "archer" && myFighter->currClass == BELIEVER)  enemyAdv = rand() % 4;
	
	tempCrit = rand() % 30;
	currentCrit = 0;
	if (tempCrit < enemyFighter->luck) currentCrit = rand() % 4 + 1;

	currentDamage = enemyFighter->strength + enemyAdv + currentCrit;
	myFighter->health -= currentDamage;
	nextAction->think = displayAttack;

	if (myFighter->health <= 0)
	{
		freeMayfly(myFighter);
		numLost++;
		nextAction->end = resetFighters;
	}
	else
	{
		nextAction->end = mayflyAttack; //changed the semicolon, greek question mark
	}
	nextAction->timer = 300;
}

void displayResults()
{
	printString("Mayflies Lost: ",	c_Black, screen, 400, 200);
	printString("Enemies Killed: ",	c_Black, screen, 400, 250);

	printInt(numLost,	c_Black, screen, 580, 200);
	printInt(numKilled, c_Black, screen, 580, 250);
}

void mayflyFightChoose(Mayfly *m)
{
	if (myFighter == NULL)
	{
		myFighter = m;
		myFighter->fighting = 1;
	}
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

	enemyFighter = enemyFighters[currentCombat];
	//enemyFighter->fighting = 1;

	if (myFighter == NULL)
	{
		fightResult->timer = 10;
		fightResult->end = chooseFighters;
	}
	else
	{
		enemyFighter->fighting = 1;
		fightResult->timer = 300; //Time in combat
		if		(myFighter->speed >= enemyFighter->speed) fightResult->end = mayflyAttack;
		else if (enemyFighter->speed >  myFighter->speed) fightResult->end = enemyAttack;
	}
}