#ifndef __ENTITY__
#define __ENTITY__

#include "graphics.h"

#define MAX_ENTITIES 200

typedef struct Entity_T
{
	Sprite *image;		/*pointer to the actual image in memory*/
	int ex, ey;			/*keep track of where the entity is on screen*/
	int restx, resty;		/*Where are they when they're resting*/
	int inUse;			/*If 0, empty entity, else it exists*/
	int display;			/*Should it be on screen. 0 if no, 1 if yes*/
	//int frame,currSpeed,maxSpeed;				/*animation*/
}Entity;

//extern Entity entityList[MAX_ENTITIES];

void initEntityList();
Entity *newEntity();
void setupEntity(Entity *e, Sprite *spr, int intx, int inty);
Entity *createEntity();
void freeEntity(Entity *e);
void closeEntities();

#endif