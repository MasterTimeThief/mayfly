#ifndef __EVENT__
#define __EVENT__

typedef struct Event_T
{
  void (*think)();
  void (*end)();

  int timer;
}Event;

extern Event eventList[10];

void initEventList();
Event *newEvent();
void runEvents();
void placeholderFunction();

#endif