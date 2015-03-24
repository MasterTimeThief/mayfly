#include "SDL.h"
#include "graphics.h"

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 576;
int SCREEN_BPP = 32;

SDL_Color c_Black = { 0, 0, 0 };
SDL_Color c_White = { 255, 255, 255 };
SDL_Color c_Red = { 255, 0, 0 };

TTF_Font *mayFont;

SDL_Surface *load_image( char* filename ) 
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

	//Load the image
    loadedImage = IMG_Load( filename );

	//If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        
        
    }
	//Free the old image
    SDL_FreeSurface( loadedImage );

	//Return the optimized image
    return optimizedImage;
}

SDL_Surface *renderText(TTF_Font *font, char *message, SDL_Color color)
{
	return TTF_RenderText_Blended( font, message, color );
}

void printString(char *message, SDL_Color color, SDL_Surface *dest, int x, int y)
{
	SDL_Surface *temp;
	
	temp = renderText( mayFont, message, color );
	apply_surface(x,y,temp,dest,NULL);
	SDL_FreeSurface(temp);
}

void printInt(int message, SDL_Color color, SDL_Surface *dest, int x, int y)
{
	SDL_Surface *temp;
	char		tempString[3];
	
	sprintf(tempString, "%i", message);
	temp = renderText( mayFont, tempString, color );
	apply_surface(x,y,temp,dest,NULL);
	SDL_FreeSurface(temp);
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

	//Blit the surface
    SDL_BlitSurface( source, clip, destination, &offset );
}