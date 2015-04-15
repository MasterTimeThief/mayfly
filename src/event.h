#ifndef __EVENT__
#define __EVENT__
#include <stdlib.h>
typedef struct Event_T
{
  void (*think)();
  void (*end)();

  int timer;
}Event;


void initEventList();
Event *newEvent();
void runEvents();
void placeholderFunction();

#endif