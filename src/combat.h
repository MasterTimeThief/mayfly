#ifndef __COMBAT__
#define __COMBAT__

#include "mayfly.h"
#include "graphics.h"

#define	MAX_ENEMIES	100

typedef struct Enemy_T
{
	Entity *entity;
	//className currClass;
	char currClass[10];
	int	isFemale; //0 Male, 1 Female

	int	health;
	int	speed;
	int	strength;
	int	luck;

	int alive;
	int combat;		//Are they in the combat room?
	int fighting;	//Are they in combat right now?
	//void (*think)(Entity *e);
}Enemy;

extern int currentCombat;
//extern Mayfly *mayflyFighters[15];
//extern Enemy *enemyFighters[15];
Mayfly *mayflyFighters[15];
Enemy *enemyFighters[15];
extern Enemy enemyList[MAX_ENEMIES];
void initEnemyList();
Enemy *newEnemy();
void setupEnemy(Enemy *m);
void createEnemy();

void displayEnemies();

void closeEnemies();

void toCombat();
void resetCombat();
void setupEnemyCombat();
void enemyThinkAll(Room *r);
void enemyThink();
void mayflyAttack();
void enemyAttack();
void mayflyFightChoose(Mayfly *m);

void chooseFighters();

#endif