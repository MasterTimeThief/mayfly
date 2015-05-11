#include "multi.h"
#include "event.h"
#include "audio.h"

int p1Alive;
int p2Alive;

extern	SDL_Surface *screen;
extern char *combatBack;

Mayfly *player1Fighter;
Mayfly *player2Fighter;

int currentMultiDamage, currentMultiFighter, currentMultiCrit;
int p1Pos, p2Pos;

void beginMultiplayer()
{
	memset(player1List,0,sizeof(Mayfly) * 10);
	memset(player2List,0,sizeof(Mayfly) * 10);
	setupMayflyMultiplayerPositions();

	initPlayerList(player1List, 1);
	initPlayerList(player2List, 2);

	changePlayer2Sprites();

	p1Alive = 1;
	p2Alive = 1;

	toMultiCombat();
}

void setupMayflyMultiplayerPositions()
{
	int i;
	for (i = 0;i < 10; i++)
	{
		//X coordinates
		if		(i < 5)		
		{
			player1Positions[i][0] = 96;
			player2Positions[i][0] = 396;
		}
		else if (i < 10)	
		{
			player1Positions[i][0] = 288;
			player2Positions[i][0] = 588;
		}

		//Y coordinates
		if		(i % 5 == 0)
		{
			player1Positions[i][1] = 96;
			player2Positions[i][1] = 96;
		}
		
		else if (i % 5 == 1)	
		{
			player1Positions[i][1] = 192;
			player2Positions[i][1] = 192;
		}
		else if (i % 5 == 2)	
		{
			player1Positions[i][1] = 288;
			player2Positions[i][1] = 288;
		}
		else if (i % 5 == 3)	
		{
			player1Positions[i][1] = 384;
			player2Positions[i][1] = 384;
		}
		else if (i % 5 == 4)	
		{
			player1Positions[i][1] = 480;
			player2Positions[i][1] = 480;
		}
	}
}

void initPlayerList(Mayfly list[], int player)
{
	int i;
	for (i = 0; i < 10; i++)
	{
		createMultiplayerMayfly(list);
		//define position here
		multiplayerPositions(&list[i], i, player);
	}
}

Mayfly *newMultiplayerMayfly(Mayfly list[])
{
	int i;
	for (i = 0;i < 10; i++)
	{
		if (!list[i].alive)
		{
			list[i].alive = 1;
			return &list[i];	
		}
	}
	/*mayflyList[maxMayflies].alive = 1;
	return &mayflyList[maxMayflies++];*/
}

void setupMultiplayerMayfly(Mayfly *m)
{
	Sprite *tempSprite = (Sprite*)malloc(sizeof(Sprite));
	int i;

	int tempClass;

	//Class and Gender
	tempClass = rand() % 3;
	m->isFemale = rand() % 2;

	//Load proper sprite
	if (tempClass == 0)
	{
		m->currClass = BELIEVER;
		if (m->isFemale) tempSprite = LoadSprite("images/people/believer_f.png",32,32);
		else				tempSprite = LoadSprite("images/people/believer_m.png",32,32);
	}
	else if (tempClass == 1)
	{
		m->currClass = ARCHER;
		if (m->isFemale) tempSprite = LoadSprite("images/people/archer_f.png",32,32);
		else				tempSprite = LoadSprite("images/people/archer_m.png",32,32);
	}
	else if (tempClass == 2)
	{
		m->currClass = SOLDIER;
		if (m->isFemale) tempSprite = LoadSprite("images/people/soldier_f.png",32,32);
		else				tempSprite =LoadSprite("images/people/soldier_m.png",32,32);
	}

	m->health =		rand() % 10 + 11;
	m->strength =	rand() % 10 + 1;
	m->speed =		rand() % 10 + 1;
	m->luck =		rand() % 10 + 1;

	m->fighting = 0;
	m->visible	= 1;
	m->selected = 0;
	m->cx		= 0;
	m->cy		= 0;

	//Sprite and position
	m->entity = createEntity();
	m->entity->image = tempSprite;
	m->entity->image->currSpeed = 0;
	m->entity->image->maxSpeed = 300; //Bigger number means slower animation
	m->entity->image->frame = rand() % 4;
}

void createMultiplayerMayfly(Mayfly list[])
{
	Mayfly *m = newMultiplayerMayfly(list);
	if (m != NULL)
	{
		setupMultiplayerMayfly(m);
	}
}

void multiplayerPositions(Mayfly *m, int index, int player)
{
	if (player == 1)
	{
		m->entity->ex = player1Positions[index][0];
		m->entity->ey = player1Positions[index][1];
	}
	else if (player == 2)
	{
		m->entity->ex = player2Positions[index][0];
		m->entity->ey = player2Positions[index][1];
	}
}

void changePlayer2Sprites()
{
	int i;
	for (i = 0;i < 10; i++)
	{
		if (player2List[i].currClass == BELIEVER)
		{
			if (player2List[i].isFemale) player2List[i].entity->image = LoadSprite("images/people/believer_f_e.png",32,32);
			else						 player2List[i].entity->image = LoadSprite("images/people/believer_m_e.png",32,32);
		}
		else if (player2List[i].currClass == ARCHER)
		{
			if (player2List[i].isFemale) player2List[i].entity->image = LoadSprite("images/people/archer_f_e.png",32,32);
			else						 player2List[i].entity->image = LoadSprite("images/people/archer_m_e.png",32,32);
		}
		else if (player2List[i].currClass == SOLDIER)
		{
			if (player2List[i].isFemale) player2List[i].entity->image = LoadSprite("images/people/soldier_f_e.png",32,32);
			else						 player2List[i].entity->image = LoadSprite("images/people/soldier_m_e.png",32,32);
		}
	}
}

void toMultiCombat()
{
	Event *combatStart = newEvent();
	changeBackground(combatBack);
	changeBackgroundMusic();

	/*currentCombat = 0;
	numKilled = 0;
	numLost = 0;
	mayflyPos = 0;
	enemyPos = 0;*/

	combatStart->timer = 1; // Time before combat starts
	combatStart->end = chooseMultiplayerFighters;
}

void endMultiCombat()
{
	//CHANGE EVERYTHING
	
	
	
	/*int i;
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
	changeBackgroundMusic();*/
}

void displayMultiAttack()
{
	if (currentMultiCrit > 0) printString("CRITICAL HIT",	c_Red, screen, 400, 150);//print the word CRITICAL HIT

	if (currentMultiFighter == 1) //mayfly attack
	{
		printString("Player 1 attacked for    !",	c_Black, screen, 400, 250);
		printInt(currentMultiDamage, c_Red, screen, 645, 250);
	}
	else if (currentMultiFighter == 2) //enemy attack
	{
		printString("Player 2 attacked for     !",	c_Black, screen, 400, 250);
		printInt(currentMultiDamage, c_Red, screen, 645, 250);
	}
}

void displayMultiResults()
{
	if (!p2Alive)		printString("Player 1 Wins!",	c_Black, screen, 400, 200);
	else if (!p1Alive)	printString("Player 2 Wins!",	c_Black, screen, 400, 250);
}

void chooseMultiplayerFighters()
{
	Event *fightResult = newEvent();

	if (p1Alive == 0 || p2Alive == 0)
	{
		fightResult->timer = 1500;
		fightResult->think = displayMultiResults;
		fightResult->end = endMultiCombat;
		return;
	}

	if (player1Fighter == NULL || player2Fighter == NULL)
	{
		fightResult->timer = 10;
		fightResult->end = chooseMultiplayerFighters;
	}
	else
	{
		player1Fighter->fighting = 1;
		player2Fighter->fighting = 1;

		fightResult->timer = 300; //Time before combat
		if		(player1Fighter->speed >= player2Fighter->speed) fightResult->end = p1Attack;
		else if (player2Fighter->speed >= player1Fighter->speed) fightResult->end = p2Attack;
	}
}

void resetMultiFighters()
{
	Event *nextFight = newEvent();

	player1Fighter->fighting = 0;
	player2Fighter->fighting = 0;
	player1Fighter = NULL;
	player2Fighter = NULL;
	p1Pos = 0;
	p2Pos = 0;
	
	nextFight->timer = 10; // time between fights
	nextFight->end = chooseFighters;
}

void p1Attack()
{
	Event *nextAction = newEvent();
	int tempCrit;
	int myAdv = 0;

	currentMultiFighter = 1;
	p1Pos = 20;
	p2Pos = 0;
	if		(player1Fighter->currClass == BELIEVER && player2Fighter->currClass == SOLDIER) myAdv = rand() % 4;
	else if (player1Fighter->currClass == SOLDIER && player2Fighter->currClass == ARCHER)	 myAdv = rand() % 4;
	else if (player1Fighter->currClass == ARCHER && player2Fighter->currClass == BELIEVER)  myAdv = rand() % 4;
	
	tempCrit = rand() % 30;
	currentMultiCrit = 0;
	if (tempCrit < player1Fighter->luck) currentMultiCrit = rand() % 4 + 1;

	currentMultiDamage = player1Fighter->strength + myAdv + currentMultiCrit;
	player2Fighter->health -= currentMultiDamage;
	nextAction->think = displayMultiAttack;
	

	if (player2Fighter->health <= 0)
	{
		freeMayfly(player2Fighter);
		nextAction->end = resetMultiFighters;
	}
	else
	{
		nextAction->end = p2Attack;
	}
	nextAction->timer = 300;
}

void p2Attack()
{
	Event *nextAction = newEvent();
	int tempCrit;
	int myAdv = 0;

	currentMultiFighter = 2;
	p1Pos = 0;
	p2Pos = 20;
	if		(player2Fighter->currClass == BELIEVER && player1Fighter->currClass == SOLDIER) myAdv = rand() % 4;
	else if (player2Fighter->currClass == SOLDIER && player1Fighter->currClass == ARCHER)	 myAdv = rand() % 4;
	else if (player2Fighter->currClass == ARCHER && player1Fighter->currClass == BELIEVER)  myAdv = rand() % 4;
	
	tempCrit = rand() % 30;
	currentMultiCrit = 0;
	if (tempCrit < player2Fighter->luck) currentMultiCrit = rand() % 4 + 1;

	currentMultiDamage = player2Fighter->strength + myAdv + currentMultiCrit;
	player1Fighter->health -= currentMultiDamage;
	nextAction->think = displayMultiAttack;
	

	if (player1Fighter->health <= 0)
	{
		freeMayfly(player1Fighter);
		nextAction->end = resetMultiFighters;
	}
	else
	{
		nextAction->end = p1Attack;
	}
	nextAction->timer = 300;
}

void player1Think()
{
	int i;
	p1Alive = 0;
	for (i = 0;i < 10; i++)
	{
		if (player1List[i].alive) p1Alive = 1;
	}
}

void player2Think()
{
	int i;
	p2Alive = 0;
	for (i = 0;i < 10; i++)
	{
		if (player2List[i].alive) p2Alive = 1;
	}
}

void multiplayerThink()
{
	player1Think();
	player2Think();
}
