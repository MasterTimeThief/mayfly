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
  Ent_Type className;	/*Seperates entities by type, to allow different functions*/

  /*Mayfly / Enemy variables*/
  int	health;
  int	speed;
  int	strength;
  int	luck;
  
  int	age;

  int	soldierExp;
  int	archerExp;
  int	believerExp;

  /*button / gui variables*/
  int sizex,sizey; //Size of the button
  //May use sprite size in future
  int filledGui; //0 if empty, 1 if filled

}Entity;

Entity *entityList[MAX_ENTITIES];

Entity *CreateEntity();
void FreeEntity(Entity e);



#endif