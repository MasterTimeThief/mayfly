#include "entity.h"

Entity entityList[MAX_ENTITIES];
int entityTotal;

void initEntityList()
{
	int x;
	entityTotal = 0;
	memset(entityList,0,sizeof(Entity) * MAX_ENTITIES);
	for(x = 0; x < MAX_ENTITIES; x++)
	{ 
		entityList[x].image = NULL;
	}
}

Entity *newEntity()
{
	int i;
	if (entityTotal + 1 > MAX_ENTITIES)
	{
		fprintf(stderr, "Maximum Entities Reached.\n");
        exit(1);	
	}
	for (i = 0;i < entityTotal; i++)
	{
		if (!entityList[i].inUse)
		{
			entityList[i].inUse = 1;
			return &entityList[i];	
		}
	}
	entityList[entityTotal].inUse = 1;
	return &entityList[entityTotal++];
}

void setupEntity(Entity *e)
{
	e->image = NULL;
	e->ex = 0;
	e->ey = 0;
	e->display = 1;
	e->frame = 0;
	e->currSpeed = 0;
	e->maxSpeed = 0;
}

Entity *createEntity()
{
	Entity *e= newEntity();
	setupEntity(e);
	return e;
}

void freeEntity(Entity *e)
{
	e->inUse = 0;
}