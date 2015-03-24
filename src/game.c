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
	
	SDL_FreeSurface(mainMenu->message);

	TTF_Quit();
	
	//Quit SDL
	SDL_Quit();
}

void modeChange(Room *r)
{
	if (r->mode == BREED)		r->mode = DRAFT;
	else if (r->mode == DRAFT)	r->mode = HEAL;
	else if (r->mode == HEAL)	r->mode = TRAIN;
	else if (r->mode == TRAIN)	r->mode = SCOUT;
	else if (r->mode == SCOUT)	r->mode = BREED;
}

void menuMove(int choice)
{
	int i;
	if (choice == 1) // UP
	{
		if		(mainMenu->choice == NEW) mainMenu->choice = EXIT;
		else if (mainMenu->choice == LOAD) mainMenu->choice = NEW;
		else if (mainMenu->choice == EXIT) mainMenu->choice = LOAD;
	}
	else if (choice == 2) // DOWN
	{
		if		(mainMenu->choice == NEW) mainMenu->choice = LOAD;
		else if (mainMenu->choice == LOAD) mainMenu->choice = EXIT;
		else if (mainMenu->choice == EXIT) mainMenu->choice = NEW;
	}
	else if (choice == 3) // ENTER
	{
		if		(mainMenu->choice == NEW)
		{
			gameRoom->roomName = MAIN;
			createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
			changeBackground(gameRoom, mainBack);
			//new game script
			for (i = 0; i < STARTING_MAYFLY; i++)
			{
				createMayfly();
			}
			//displayMayflies(screen);
		}
		else if (mainMenu->choice == LOAD) 
		{
			gameRoom->roomName = MAIN;
			createButton(64, 416, 128, 64, "images/button.png", (*combatButtonThink), toCombat);
			changeBackground(gameRoom, mainBack);
			//load game script
		}
		else if (mainMenu->choice == EXIT) gameRoom->roomName = QUIT;
	}
	displayMenu(mainMenu);
}

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

	//Menu
	mainMenu = createMenu("fonts/menuFont.ttf", 30);
	displayMenu(mainMenu);
	apply_surface(0,0,mainMenu->message,screen,NULL);

    //Game Loop
	while(!done)
	{
		if (gameRoom->roomName == MENU)
		{
			//Update Screen
			if (mainMenu->message != NULL && mainMenu->changed == 1)
			{
				if (mainMenu->choice == NEW) choicePos = 400;
				else if (mainMenu->choice == LOAD) choicePos = 450;
				else if (mainMenu->choice == EXIT) choicePos = 500;

				updateBackground(gameRoom, screen);
				apply_surface(0,0,mainMenu->message,screen,NULL);
				DrawSprite(mainMenu->cursor,screen,750,choicePos, 1);
				mainMenu->changed = 0;
			}
		}
		else if (gameRoom->roomName == MAIN)
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
			//If a key was pressed
            if( eventCheck.type == SDL_KEYDOWN )
            {
				if (gameRoom->roomName == MENU)
				{
					switch( eventCheck.key.keysym.sym )
					{
						case SDLK_UP:		menuMove(1); break;
						case SDLK_DOWN:		menuMove(2); break;
						case SDLK_SPACE:
						case SDLK_RETURN :	menuMove(3); break;
					}
				}
				else if (gameRoom->roomName == MAIN)
				{
					switch( eventCheck.key.keysym.sym )
					{
						case SDLK_SPACE:	
							modeChange(gameRoom);
							clearMayflySelection();
							break;
						/*case SDLK_1:		//menuMove(2); break;
						case SDLK_2:		//menuMove(3); break;
						case SDLK_3:		//menuMove(3); break;*/
					}
				}
			}
			//If the user has Xed out the window
            else if( eventCheck.type == SDL_QUIT )
            {
                //Quit the program
                done = 1;
            }
		}
	}
	clean_up();

	return 0;
}