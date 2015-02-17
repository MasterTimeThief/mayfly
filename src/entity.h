#ifndef __ENTITY__
#define __ENTITY__

#include "graphics.h"

enum Ent_Type 
{ 
    MAYFLY, 
    GUI 
};

#define MAX_ENTITIES 100

typedef struct Entity_T
{
  Sprite *image;		/*pointer to the actual image in memory*/
  int ex, ey;			/*keep track of where the entity is on screen*/
  int filled;			/*If 0, empty entity, else it exists*/
  //Ent_Type className;	/*Seperates entities by type, to allow different functions*/
}Entity;

Entity *entityList[MAX_ENTITIES];

Entity *CreateEntity();
void FreeEntity(Entity e);

#endif