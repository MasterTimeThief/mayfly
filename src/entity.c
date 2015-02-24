#include "entity.h"

Entity *CreateEntity(Sprite *spr, int intx, int inty, Ent_Type className)
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

			//Check className
			if (className == MAYFLY)
			{
				temp->health = (rand() % 10) + 11;	//10-20
				temp->speed = (rand() % 10) + 1;	//1-10
				temp->strength = (rand() % 10) + 1;	//1-10
				temp->luck = (rand() % 10) + 1;		//1-10

				temp->age = 0;

				temp->soldierExp = 0;
				temp->archerExp = 0;
				temp->believerExp = 0;
			}
			else if (className == ENEMY)
			{
				temp->health = (rand() % 10) + 11;	//10-20
				temp->speed = (rand() % 10) + 1;	//1-10
				temp->strength = (rand() % 10) + 1;	//1-10
				temp->luck = (rand() % 10) + 1;		//1-10
			}
			else if (className == BUTTON)
			{
				//possible size variables
				//Implement button function?
			}
			else if (className == GUI)
			{
				//possible size variables
				temp->filledGui = 0;
			}
			break;
		}
	}

	return temp;
}

void FreeEntity(Entity *e)
{
	e->filled = 0;
}