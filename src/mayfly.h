#ifndef __MAYFLY__
#define __MAYFLY__

#include "entity.h"

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
	int		isFemale; //0 Male, 1 Female
	
	int	health;
	int	speed;
	int	strength;
	int	luck;

	int	soldierExp;
	int	archerExp;
	int	believerExp;

	int	visible;
	int inUse;
	//genetics flags or arrays
}Mayfly;

extern Mayfly mayflyList[MAX_MAYFLIES];
extern Mayfly mayflySelect[15];

void initMayflyList();

Mayfly *newMayfly();
void setupMayfly(Mayfly *m);
void createMayfly();
void createMayflyOffspring(Mayfly *m1, Mayfly *m2);

void newMayflies();
void displayMayflies(SDL_Surface *screen);
void freeMayfly(Mayfly *m);
void closeMayflies();

void mayflyAllThink();
void mayflyThink(Mayfly *m);

#endif