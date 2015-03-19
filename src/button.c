#include "button.h"
#include "mouse.h"
#include "room.h"
#include "mayfly.h"
#include "combat.h"
#include "event.h"

Button *combatButton;
extern Room *gameRoom;
extern	SDL_Surface *screen;
extern char *combatBack;

void createButton()
{
	combatButton = (Button*)malloc(sizeof(Button));
	combatButton->visible = 1;
	combatButton->effect = toCombat;

	combatButton->entity = createEntity();
	combatButton->entity->image = LoadSprite("images/button.png",128,64);
	combatButton->entity->ex = 64;
	combatButton->entity->ey = 416;
}

void buttonThink()
{
	if (gameRoom->roomName == MAIN)
	{
		combatButton->visible = 1;
		if (combatButton->visible) DrawSprite(combatButton->entity->image, screen, combatButton->entity->ex, combatButton->entity->ey, combatButton->entity->frame);
		if (mouseHover(combatButton->entity->ex,  combatButton->entity->ey,  combatButton->entity->image->w,  combatButton->entity->image->h))
		{
			if (clickLeft)
			{
				combatButton->entity->frame = 1;
				if (gameRoom->mode == DRAFT && checkSelected() > 1) (combatButton->effect)();
			}
			else if (!clickLeft) combatButton->entity->frame = 0;
		}
	}
	else combatButton->visible = 0;
}

void toCombat()
{
	gameRoom->roomName = COMBAT;
	setupEnemyCombat();
	setupMayflyCombat();
	changeBackground(gameRoom, combatBack);

	currentCombat = 0;

	Event *combatStart = newEvent();
	combatStart->timer = 1;
	combatStart->end = chooseFighters;
}