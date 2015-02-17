#include "entity.h"

Entity *CreateEntity(Sprite *spr, int intx, int inty)
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
			entityList[i] = temp;
			break;
		}
	}

	return temp;
}

void FreeEntity(Entity *e)
{
	e->filled = 0;
}