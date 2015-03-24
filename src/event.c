#include "event.h"

Event eventList[10];

void initEventList()
{
	int i;
	for (i = 0;i < 10;i++)
	{
		eventList[i].end = placeholderFunction;
		eventList[i].think = placeholderFunction;
		eventList[i].timer = 0;
	}
}

Event *newEvent()
{
	int i;
	for (i = 0;i < 10;i++)
	{
		if (eventList[i].timer == 0) return &eventList[i];
	}
}

void runEvents()
{
	int i;
	for (i = 0;i < 10;i++)
	{
		if (eventList[i].timer > 0)
		{
			if (eventList[i].timer == 1)
			{
				(*eventList[i].end)();
				eventList[i].end = placeholderFunction;
				eventList[i].think = placeholderFunction;
			}
			else
			{
				(*eventList[i].think)();
			}
			eventList[i].timer--;
		}
	}
}

void placeholderFunction()
{
	return;
}