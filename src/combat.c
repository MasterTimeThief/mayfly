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
extern char *winBack;
extern char *loseBack;

Mayfly *myFighter;
Enemy *enemyFighter;
Sprite *deathSprite;

int mayflyPos;
int enemyPos;

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
	deathSprite = LoadSprite("images/death.png",32,32);
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

void setupEnemy(Enemy *e)
{
	Sprite *tempSprite = (Sprite*)malloc(sizeof(Sprite));

	int tempClass;

	//Class and Gender
	tempClass = rand() % 3;
	e->isFemale = rand() % 2;

	//Load proper sprite
	if (tempClass == 0)
	{
		strncpy(e->currClass,"believer",10);
		if (e->isFemale) tempSprite = LoadSprite("images/people/believer_f_e.png",32,32);
		else				tempSprite = LoadSprite("images/people/believer_m_e.png",32,32);
	}
	else if (tempClass == 1)
	{
		strncpy(e->currClass,"archer",10);
		if (e->isFemale) tempSprite = LoadSprite("images/people/archer_f_e.png",32,32);
		else				tempSprite = LoadSprite("images/people/archer_m_e.png",32,32);
	}
	else if (tempClass == 2)
	{
		strncpy(e->currClass,"soldier",10);
		if (e->isFemale) tempSprite = LoadSprite("images/people/soldier_f_e.png",32,32);
		else				tempSprite =LoadSprite("images/people/soldier_m_e.png",32,32);
	}

	e->health =		rand() % 10 + 11;
	e->strength =	rand() % 10 + 1;
	e->speed =		rand() % 10 + 1;
	e->luck =		rand() % 10 + 1;

	e->combat = 0;
	e->fighting = 0;

	//Sprite and position
	e->entity = createEntity();
	//flip_surface(tempSprite->image);
	e->entity->image = tempSprite;
	e->entity->ex = 0;
	e->entity->ey = 0;
	e->entity->image->currSpeed = 0;
	e->entity->image->maxSpeed = 500; //Bigger number means slower animation
	e->entity->image->frame = rand() % 4;
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
				DrawSprite(enemyList[i].entity->image, screen, 550 - enemyPos, 200, enemyList[i].entity->image->frame);
				printInt(enemyList[i].health, c_Red, screen, 615, 200);
			}
			else if (enemyList[i].combat)
			{
				DrawSprite(enemyList[i].entity->image, screen, enemyList[i].entity->ex, enemyList[i].entity->ey, enemyList[i].entity->image->frame);
			}
		}
		else if (!enemyList[i].alive && enemyList[i].fighting)
		{
			DrawSprite(deathSprite, screen, 550, 200, 0);
		}
	}
}

void freeEnemy(Enemy *e)
{
	//FreeSprite(e->entity->image);
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

	if (currFighters > enemyTotal - 1) currFighters = enemyTotal - 1;

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
		createButton(448, 32, 128, 64,  "images/buttons/button.png",(*combatButtonThink), clearMayflySelection);
		setupEnemyCombat();
		setupMayflyCombat();
		changeBackground(combatBack);
		changeBackgroundMusic();

		currentCombat = 0;
		numKilled = 0;
		numLost = 0;
		mayflyPos = 0;
		enemyPos = 0;

		enemyFighter = chooseNextEnemy();

		combatStart->timer = 1; // Time before combat starts
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
	if (enemyTotal == 0)
	{
		gameRoom->roomName = WIN;
		changeBackground(winBack);
	}
	else if (mayflyTotal < 10)
	{
		gameRoom->roomName = LOSE;
		changeBackground(loseBack);
	}
	else
	{
		gameRoom->roomName = MAIN;
		mainButtons();
		changeBackground(mainBack);
	}
	changeBackgroundMusic();
}

void resetFighters()
{
	Event *nextFight = newEvent();

	myFighter->fighting = 0;
	enemyFighter->fighting = 0;
	myFighter = NULL;
	enemyFighter = chooseNextEnemy();
	mayflyPos = 0;
	enemyPos = 0;
	
	nextFight->timer = 10; // time between fights
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
	mayflyPos = 20;
	enemyPos = 0;
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
	nextAction->timer = COMBAT_LENGTH;
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
	int enemyAdv = 0;

	currentFighter = 2;
	mayflyPos = 0;
	enemyPos = 20;
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
	nextAction->timer = COMBAT_LENGTH;
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

Enemy *chooseNextEnemy()
{
	int i;
	int whichEnemy;
	int stillAlive = 0;
	Enemy *fighter = NULL;

	for (i = 0; i <= currFighters; i++)
	{
		if (enemyFighters[i]->alive) stillAlive = 1;
	}

	if (!stillAlive)
	{
		return NULL;
	}

	whichEnemy = rand() % (currFighters + 1);
	while (fighter == NULL)
	{
		if (enemyFighters[whichEnemy]->alive)	fighter = enemyFighters[whichEnemy];
		else									whichEnemy = rand() % (currFighters + 1);
	}
	return fighter;
}

void chooseFighters()
{
	Event *fightResult = newEvent();

	if (enemyFighter == NULL || mayflySelected == 0)
	{
		fightResult->timer = 1500;
		fightResult->think = displayResults;
		fightResult->end = endCombat;
		return;
	}

	if (myFighter == NULL)
	{
		fightResult->timer = 10;
		fightResult->end = chooseFighters;
	}
	else
	{
		enemyFighter->fighting = 1;
		fightResult->timer = 300; //Time before combat
		if		(myFighter->speed >= enemyFighter->speed) fightResult->end = mayflyAttack;
		else if (enemyFighter->speed >  myFighter->speed) fightResult->end = enemyAttack;
	}
}