#include "game.h"

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
	if (choice == 1)
	{
		if		(mainMenu->choice == NEW) mainMenu->choice = EXIT;
		else if (mainMenu->choice == LOAD) mainMenu->choice = NEW;
		else if (mainMenu->choice == EXIT) mainMenu->choice = LOAD;
	}
	else if (choice == 2)
	{
		if		(mainMenu->choice == NEW) mainMenu->choice = LOAD;
		else if (mainMenu->choice == LOAD) mainMenu->choice = EXIT;
		else if (mainMenu->choice == EXIT) mainMenu->choice = NEW;
	}
	/*else if (choice == "enter")
	{

	}*/
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
    //buffer = load_image( "images/battle2.png" );
	changeBackground(gameRoom, menuBack, screen);

	//Menu
	cursor = LoadSprite("images/testsprite2.png",32,32);
	mainMenu = createMenu("fonts/font1.ttf", 30, cursor);
	displayMenu(mainMenu);
	//apply_surface(0,0,buffer,screen,NULL);
	apply_surface(0,0,mainMenu->message,screen,NULL);

    //Game Loop
	while(!done)
	{
		if (gameRoom->roomName == MENU)
		{
			//Update Screen
			if (mainMenu->message != NULL)
			{
				/*apply_surface(0,0,buffer,screen,NULL);
				apply_surface(0,0,mainMenu->message,screen,NULL);
				mainMenu->message = NULL;*/

				if (mainMenu->choice == NEW) choicePos = 400;
				else if (mainMenu->choice == LOAD) choicePos = 450;
				else if (mainMenu->choice == EXIT) choicePos = 500;

				//apply_surface(0,0,buffer,screen,NULL);
				//changeBackground(gameRoom, menuBack, screen);
				apply_surface(0,0,mainMenu->message,screen,NULL);
				DrawSprite(mainMenu->cursor,screen,750,choicePos, 0);
			}
		}
		else if (gameRoom->roomName == MAIN)
		{
			if (gameRoom->filename != mainBack) changeBackground(gameRoom, mainBack, screen);
		}
		else if (gameRoom->roomName == COMBAT)
		{

		}
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
					//case SDLK_ : message = leftMessage; break;
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