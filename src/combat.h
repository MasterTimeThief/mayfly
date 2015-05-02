#ifndef __COMBAT__
#define __COMBAT__

#include "mayfly.h"
#include "graphics.h"

#define	MAX_ENEMIES	30

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

int enemyTotal;
extern int currentCombat;
Mayfly *mayflyFighters[15];
Enemy *enemyFighters[15];
Enemy enemyList[MAX_ENEMIES];

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