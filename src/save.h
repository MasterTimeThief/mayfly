#ifndef __SAVE__
#define __SAVE__

#include <jansson.h>

#define MAX_SAVE	140

typedef struct SaveData_S
{
	char  className[10];
	int	isFemale; //0 Male, 1 Female
	
	int age;
	int x,y;

	int	health;
	int	speed;
	int	strength;
	int	luck;

	int	soldierExp;
	int	archerExp;
	int	believerExp;

	int alive;
}SaveData;

void save_my_data(char *filepath);
void load_my_data(char *filepath);
void saveToList();
void loadFromList();

#endif