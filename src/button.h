#ifndef __BUTTON__
#define __BUTTON__

#include "entity.h"

#define MAX_BUTTONS	15

typedef struct Button_T
{
	Entity *entity;
	int visible;	//Also checks if this is in use
	void (*think)(int index);
	void (*effect)();
}Button;

extern Button buttonList[MAX_BUTTONS];

void combatButtonThink(int index);
void draftButtonThink(int index);
void menuButtonThink(int index);
void trainButtonThink(int index);
void mainButtonThink(int index);
void sortButtonThink(int index);
void toHeal();
void toTrain();
void toBreed();
void toScout();
void mainButtons();
void editButtons();
void createButton(int x, int y, int w, int h, char *filename, void (*think)(int index), void (*effect)());
void buttonThink();

void editButtonThink(int index);
void healthUp();
void healthDown();
void speedUp();
void speedDown();
void strengthUp();
void strengthDown();
void luckUp();
void luckDown();
void ageUp();
void ageDown();

#endif