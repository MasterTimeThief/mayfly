#include "entity.h"

Entity *createEntity(Sprite *spr, int intx, int inty, Ent_Type className)
{
	Entity *temp;

	int i = 0;
	for (i = 0; i <= MAX_ENTITIES; i++)
	{
		if (entityList[i]->filled != 1)
		{
			temp->image = spr;
			temp->ex = intx;
			temp->ey = inty;
			temp->filled = 1;
			temp->display = 1;
			entityList[i] = temp;

			break;
		}
	}

	return temp;
}

void loadEntity(Entity *e)
{
	if (e->display)
	{

	}
}

void freeEntity(Entity *e)
{
	e->filled = 0;
}