//#include "game.h"

#include "SDL.h"
#include "graphics.h"
#include "menu.h"
#include "room.h"

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 576;
int SCREEN_BPP = 32;

Menu *mainMenu = NULL;
Room *gameRoom = NULL;

char *menuBack = "images/battle2.png";
char *mainBack = "images/bgtest2.png";
char *combatBack = "images/battle2.png";

//SDL_Surface *buffer = NULL;
SDL_Surface *screen = NULL;

SDL_Event eventCheck;

int init()
{
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
    
    //If everything initialized fine
    return 1;
}

void clean_up()
{
	//Free the loaded image
	SDL_FreeSurface( gameRoom->background );

	CloseSprites();
	SDL_FreeSurface(mainMenu->message);

	TTF_Quit();
	
	//Quit SDL
	SDL_Quit();
}

void menuMove(int choice)
{
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
			//new game script
		}
		else if (mainMenu->choice == LOAD) 
		{
			gameRoom->roomName = MAIN;
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

	
	//Initialize
    if( init() == -1 )
    {
        return 1;
	}

	gameRoom = createRoom();

    //Load image
    //screen = load_image( "images/battle2.png" );
	updateBackground(gameRoom, menuBack, screen);

	//Menu
	mainMenu = createMenu("fonts/font1.ttf", 30);
	displayMenu(mainMenu);
	//apply_surface(0,0,buffer,screen,NULL);
	apply_surface(0,0,mainMenu->message,screen,NULL);

    //Game Loop
	while(!done)
	{
		if (gameRoom->roomName == MENU)
		{
			//Update Screen
			if (mainMenu->message != NULL && mainMenu->changed == 1)
			{
				/*apply_surface(0,0,buffer,screen,NULL);
				apply_surface(0,0,mainMenu->message,screen,NULL);
				mainMenu->message = NULL;*/

				if (mainMenu->choice == NEW) choicePos = 400;
				else if (mainMenu->choice == LOAD) choicePos = 450;
				else if (mainMenu->choice == EXIT) choicePos = 500;

				//apply_surface(0,0,buffer,screen,NULL);
				updateBackground(gameRoom, menuBack, screen);
				apply_surface(0,0,mainMenu->message,screen,NULL);
				DrawSprite(mainMenu->cursor,screen,750,choicePos, 1);
				mainMenu->changed = 0;
			}
		}
		else if (gameRoom->roomName == MAIN)
		{
			if (gameRoom->filename != mainBack) updateBackground(gameRoom, mainBack, screen);
		}
		else if (gameRoom->roomName == COMBAT)
		{

		}
		else if (gameRoom->roomName == QUIT)
		{
			//Quit the program
            done = 1;
		}
		//screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		//apply_surface(0,0,buffer,screen,NULL);
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
				switch( eventCheck.key.keysym.sym )
				{
					case SDLK_UP: menuMove(1); break;
					case SDLK_DOWN: menuMove(2); break;
					case SDLK_RETURN : menuMove(3); break;
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