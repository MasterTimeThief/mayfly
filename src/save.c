#include "save.h"
#include "entity.h"
#include "mayfly.h"
#include "combat.h"

static SaveData saveList[MAX_SAVE];

extern int maxMayflies;

void saveMayfly(SaveData *temp, Mayfly *m)
{
	if (m->currClass == BELIEVER)		strncpy(temp->className,"believer",10);
	else if (m->currClass == ARCHER)	strncpy(temp->className,"archer",10);
	else if (m->currClass == SOLDIER)	strncpy(temp->className,"soldier",10);

	temp->isFemale = m->isFemale;
	temp->age = m->age;
	temp->x = m->entity->ex;
	temp->y = m->entity->ey;

	temp->health = m->health;
	temp->speed = m->speed;
	temp->strength = m->strength;
	temp->luck = m->luck;

	temp->experience = m->experience;

	temp->alive = m->alive;

	//save_my_data("saves/savefile.dat",&tempMayfly);
}

void saveEnemy(SaveData *temp, Enemy *e)
{
	if (strcmp(e->currClass, "believer"))		strncpy(temp->className,"believer",10);
	else if (strcmp(e->currClass, "archer"))			strncpy(temp->className,"archer",10);
	else if (strcmp(e->currClass, "soldier"))			strncpy(temp->className,"soldier",10);

	temp->isFemale = e->isFemale;
	temp->age = 0;
	temp->x = 0;
	temp->y = 0;

	temp->health = e->health;
	temp->speed = e->speed;
	temp->strength = e->strength;
	temp->luck = e->luck;

	temp->experience = 0;

	temp->alive = e->alive;

	//save_my_data("saves/savefile.dat",&tempMayfly);
}

void saveToList()
{
	int i;
	int count = 0;
	for (i = 0;i < maxMayflies; i++)
	{
		if (mayflyList[i].alive) 
		{
			saveMayfly(&saveList[count], &mayflyList[i]);
			count++;
		}
	}

	if (count < MAX_MAYFLIES - 1)
	{
		saveList[count].alive = -1;
	}

	count = 0;

	for (i = 0; i < MAX_ENEMIES; i++)
	{
		if (enemyList[i].alive) 
		{
			saveEnemy(&saveList[count + MAX_MAYFLIES], &enemyList[i]);
			count++;
		}
	}

	if (count < MAX_ENEMIES - 1)
	{
		saveList[count + MAX_MAYFLIES].alive = -1;
	}

	save_my_data("saves/savefile.dat");
}



void loadMayfly(SaveData *temp, Mayfly *m)
{
	Sprite *tempSprite = (Sprite*)malloc(sizeof(Sprite));
	
	m->isFemale = temp->isFemale;
	
	if (!strcmp(temp->className, "believer"))
	{
		m->currClass = BELIEVER;
		if (m->isFemale) tempSprite = LoadSprite("images/believer_f.png",32,32);
		else				tempSprite = LoadSprite("images/believer_m.png",32,32);
	}
	else if (!strcmp(temp->className, "archer"))
	{
		m->currClass = ARCHER;
		if (m->isFemale) tempSprite = LoadSprite("images/archer_f.png",32,32);
		else				tempSprite = LoadSprite("images/archer_m.png",32,32);
	}
	else if (!strcmp(temp->className, "soldier"))
	{
		m->currClass = SOLDIER;
		if (m->isFemale) tempSprite = LoadSprite("images/soldier_f.png",32,32);
		else				tempSprite = LoadSprite("images/soldier_m.png",32,32);
	}

	m->isFemale = temp->isFemale;
	m->age = temp->age;

	m->health = temp->health;
	m->speed = temp->speed;
	m->strength = temp->strength;
	m->luck = temp->luck;

	m->experience = temp->experience;

	m->alive = temp->alive;
	m->visible = temp->alive;
	m->action = 1;

	//Sprite and position
	m->entity = createEntity();
	m->entity->image = tempSprite;
	m->entity->ex = temp->x;
	m->entity->ey = temp->y;
	m->entity->image->currSpeed = 0;
	m->entity->image->maxSpeed = 45; //Bigger number means slower animation
	m->entity->image->frame = rand() % 4;

	maxMayflies++;
}

void loadEnemy(SaveData *temp, Enemy *e)
{
	Sprite *tempSprite = (Sprite*)malloc(sizeof(Sprite));

	e->isFemale = temp->isFemale;

	if (!strcmp(temp->className, "believer"))
	{
		strncpy(e->currClass,"believer",10);
		if (e->isFemale) tempSprite = LoadSprite("images/believer_f.png",32,32);
		else				tempSprite = LoadSprite("images/believer_m.png",32,32);
	}
	else if (!strcmp(temp->className, "archer"))
	{
		strncpy(e->currClass,"archer",10);
		if (e->isFemale) tempSprite = LoadSprite("images/archer_f.png",32,32);
		else				tempSprite = LoadSprite("images/archer_m.png",32,32);
	}
	else if (!strcmp(temp->className, "soldier"))
	{
		strncpy(e->currClass,"soldier",10);
		if (e->isFemale) tempSprite = LoadSprite("images/soldier_f.png",32,32);
		else				tempSprite = LoadSprite("images/soldier_m.png",32,32);
	}

	e->isFemale = temp->isFemale;

	e->health = temp->health;
	e->speed = temp->speed;
	e->strength = temp->strength;
	e->luck = temp->luck;

	e->alive = temp->alive;

	//Sprite and position
	e->entity = createEntity();
	e->entity->image = tempSprite;
	e->entity->ex = temp->x;
	e->entity->ey = temp->y;
	e->entity->image->currSpeed = 0;
	e->entity->image->maxSpeed = 45; //Bigger number means slower animation
	e->entity->image->frame = rand() % 4;
}

void loadFromList()
{
	int i;

	load_my_data("saves/savefile.dat");
	for (i = 0;i < MAX_MAYFLIES; i++)
	{
		if (saveList[i].alive == -1) break;
		else if (saveList[i].alive)	loadMayfly(&saveList[i], &mayflyList[i]);
	}

	for (i = 0;i < MAX_ENEMIES; i++)
	{
		if (saveList[i + MAX_MAYFLIES].alive == -1) break;
		loadEnemy(&saveList[i + MAX_MAYFLIES], &enemyList[i]);
	}

}

void save_my_data(char *filepath)
{
	int i;
	FILE *savefile;
	printf("SAVING!!!\n");
	savefile = fopen(filepath, "wb");
	fwrite(saveList,sizeof(saveList),1,savefile);
	fclose(savefile);
	for (i = 0;i < MAX_MAYFLIES;i++)
	{
		fprintf(stdout,"mayfly name: %s\n",saveList[i].className);
	}
}

void load_my_data(char *filepath)
{
	FILE *savefile;
	printf("SAVING!!!\n");
	savefile = fopen(filepath, "rb");
	fread(saveList,sizeof(saveList),1,savefile);
	fclose(savefile);
}