#ifndef _GRAPHICS_
#define _GRAPHICS_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sprite.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_BPP;

extern SDL_Color c_Black;
extern SDL_Color c_White;

extern TTF_Font *mayFont;

SDL_Surface *load_image( char* filename );
SDL_Surface *renderText(TTF_Font *font, char *message, SDL_Color color);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);

#endif