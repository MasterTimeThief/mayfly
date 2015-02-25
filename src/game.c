#include "SDL.h"
#include "graphics.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *buffer = NULL;
SDL_Surface *screen = NULL;

//The event structure that will be used
SDL_Event event;

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
    
    //Set the window caption
    SDL_WM_SetCaption( "Mayfly Wars", NULL );
    
    //If everything initialized fine
    return 1;
}

void clean_up()
{
	//Free the loaded image
    SDL_FreeSurface( buffer );
	
	//Quit SDL
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	int done = 0;
	
	//Initialize
    if( init() == -1 )
    {
        return 1;
	}

    //Load image
    buffer = load_image( "images/battle2.png" );
	apply_surface(0,0,buffer,screen);
	//apply_surface(300,300,background,screen);

    //Update Screen
    SDL_Flip( screen );

    //Game Loop
	while(!done)
	{
		//While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
			//If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                done = 1;
            }
		}
	}
	clean_up();

	return 0;
}