#ifndef __ENTITY__
#define __ENTITY__

#include "graphics.h"

typedef enum  
{ 
    MAYFLY,
	ENEMY,
	BUTTON,
    GUI 
}Ent_Type;

#define MAX_ENTITIES 100

typedef struct Entity_T
{
  Sprite *image;		/*pointer to the actual image in memory*/
  int ex, ey;			/*keep track of where the entity is on screen*/
  int filled;			/*If 0, empty entity, else it exists*/
  int display;			/*Should it be on screen. 0 if no, 1 if yes*/
}Entity;

Entity *entityList[MAX_ENTITIES];

Entity *createEntity(Sprite *spr, int intx, int inty, Ent_Type className);
void loadEntity(Entity *e);
void freeEntity(Entity *e);

#endif