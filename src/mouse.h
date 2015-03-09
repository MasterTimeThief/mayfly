#ifndef __MOUSE__
#define __MOUSE__

#include "SDL.h"

extern int clickLeft;
extern int clickRight;
extern int mx;
extern int my;

void mouseInit();
void mouseThink();
int mouseHover(int x, int y, int w, int h);

#endif