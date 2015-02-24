#ifndef __MAYFLY__
#define __MAYFLY__

#include "entity.h"

typedef struct Mayfly_T
{
  Entity *entity = CreateEntity();
  int	health;
  int	speed;
  int	strength;
  int	luck;

  int	soldierExp;
  int	archerExp;
  int	believerExp;

  //genetics flags or arrays
}Mayfly;

void CreateMayfly

#endif