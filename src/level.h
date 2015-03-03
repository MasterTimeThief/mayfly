#ifndef __LEVEL__
#define __LEVEL__

/*#include "sprite.h"

#define LEVELNAMELEN 80

typedef enum 
{ 
    MENU,
	VILLAGE,
	HEAL,
	TRAIN,
	BREED,
	DRAFT,
	COMBAT
}Level_Type;

typedef struct
{
	int loaded; //true if there is a level loaded
	char filename[LEVELNAMELEN];
	Sprite *bgImage; //The actual background of the level
	SDL_Rect bounds; //actual bounds for entities in this level
}Level;

void LoadLevel(char *filename);
void DrawLevel();
void CloseLevel();

void InitLevelSystem();*/

#endif