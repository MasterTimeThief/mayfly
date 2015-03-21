#ifndef __MAYFLY__
#define __MAYFLY__

#include "entity.h"
#include "room.h"

#define MAX_MAYFLIES	40

typedef enum
{
	BELIEVER,
	SOLDIER,
	ARCHER
}className;

//soldier beats archer
//archer beats believer
//believer beats soldier

typedef struct Mayfly_T
{
	Entity *entity;
	className currClass;
	int	isFemale; //0 Male, 1 Female
	
	int age;

	int	health;
	int	speed;
	int	strength;
	int	luck;

	int	soldierExp;
	int	archerExp;
	int	believerExp;

	int	visible; //Turn invisible on combat
	int alive;
	int selected;

	int fighting;
	int action; //0 if action is used up
	int cx, cy;	//combat positions

	//void (*think)(Entity *e);
}Mayfly;

extern int mayflyTotal;
extern Mayfly mayflyList[MAX_MAYFLIES];
extern int mayflyPositions[15][2];

void initMayflyList();
void setupMayflyCombatPositions();

Mayfly *newMayfly();
void setupMayfly(Mayfly *m);
void createMayfly();
void createMayflyOffspring(Mayfly *m1, Mayfly *m2);

void mayflyCombatPosition(Mayfly *m, int index);
void setupMayflyCombat();

void newMayflies();
void displayMayflies();
void mayflyAfterCombat();
void freeMayfly(Mayfly *m);
void closeMayflies();
void clearMayflySelection();
int	 checkSelected();
void setupMayflyOffspring(Mayfly *child);
void mayflyAllThink(Room *r);
void mayflyThink(Mayfly *m);

#endif