#ifndef __BUTTON__
#define __BUTTON__

#include "entity.h"

#define MAX_BUTTONS	10

typedef struct Button_T
{
	Entity *entity;
	int visible;	//Also checks if this is in use
	void (*think)(int index);
	void (*effect)();
}Button;

extern Button buttonList[MAX_BUTTONS];

void combatButtonThink(int index);
void menuButtonThink(int index);
void trainButtonThink(int index);
void mainButtonThink(int index);
void toHeal();
void toTrain();
void toBreed();
void toScout();
void mainButtons();
void createButton(int x, int y, int w, int h, char *filename, void (*think)(), void (*effect)());
void buttonThink();

#endif