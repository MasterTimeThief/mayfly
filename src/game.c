//#include "game.h"

#include <time.h>
#include "SDL.h"
#include "graphics.h"
#include "menu.h"
#include "room.h"
#include "mayfly.h"
#include "mouse.h"
#include "button.h"
#include "combat.h"
#include "event.h"

int STARTING_MAYFLY = 20;

char *menuBack = "images/menu2.png";
char *mainBack = "images/main2.png";
char *combatBack = "images/battle.png";

SDL_Surface *screen = NULL;
SDL_Event eventCheck;

int clickLeft, clickRight, mx, my, stopClick;

int init()
{
	/* initialize random seed: */
	srand ( time(NULL) );
	
	//Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return -1;    
    }
    
    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return -1;    
    }

	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return -1;    
    }
    
    //Set the window caption
    SDL_WM_SetCaption( "Mayfly Wars", NULL );

	initEventList();
	initMayflyList();
	initEntityList();
	initEnemyList();
	mouseInit();
    
    //If everything initialized fine
    return 1;
}

void allThink()
{
	mouseThink();
	mayflyAllThink(gameRoom);
	enemyThinkAll(gameRoom);
	roomThink(gameRoom);
	buttonThink();
	runEvents();
}

void clean_up()
{
	//Free the loaded image
	SDL_FreeSurface( gameRoom->background );
	
	closeEntities();
	closeSprites();

	TTF_Quit();
	
	//Quit SDL
	SDL_Quit();
}

/*void modeChange(Room *r)
{
	if (r->mode == BREED)		r->mode = DRAFT;
	else if (r->mode == DRAFT)	r->mode = HEAL;
	else if (r->mode == HEAL)	r->mode = TRAIN;
	else if (r->mode == TRAIN)	r->mode = SCOUT;
	else if (r->mode == SCOUT)	r->mode = BREED;
}*/

int main( int argc, char* args[] )
{
	int done = 0;
	int choicePos;
	int spriteLoop;
	
	//Initialize
    if( init() == -1 )
    {
        return 1;
	}

	gameRoom = createRoom();

    //Load image
	updateBackground(gameRoom, screen);

	//Menu buttons
	createButton(768, 384, 96, 32, "images/newButton.png", (*menuButtonThink), menuNew);
	createButton(768, 448, 96, 32, "images/loadButton.png", (*menuButtonThink), menuLoad);
	createButton(768, 512, 96, 32, "images/exitButton.png", (*menuButtonThink), menuExit);

    //Game Loop
	while(!done)
	{
		if (gameRoom->roomName == MAIN)
		{
			updateBackground(gameRoom, screen);
			displayMayflies();
		}
		else if (gameRoom->roomName == COMBAT)
		{
			updateBackground(gameRoom, screen);
			displayMayflies();
			displayEnemies();
		}
		else if (gameRoom->roomName == QUIT)
		{
			//Quit the program
            done = 1;
		}

		//Run think functions
		allThink();

        if( SDL_Flip( screen ) == -1 )
        {
            return 1;    
        }

		//While there's an event to handle
        while( SDL_PollEvent( &eventCheck ) )
        {
			//If the user has Xed out the window
            if( eventCheck.type == SDL_QUIT )
            {
                //Quit the program
                done = 1;
            }
		}
	}
	clean_up();

	return 0;
}