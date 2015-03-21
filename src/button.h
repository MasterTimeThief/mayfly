#ifndef __BUTTON__
#define __BUTTON__

#include "entity.h"

typedef struct Button_T
{
	Entity *entity;
	int visible;
	void (*effect)();
}Button;

extern Button *combatButton;

void createButton();
void buttonThink();

#endif