#ifndef __SPRITE__
#define __SPRITE__

#define MaxSprites    255

int NumSprites;

typedef struct Sprite_T
{
  SDL_Surface *image;		/*pointer to the actual image in memory*/
  char filename[30];			/*the name of the file, keep them short*/
  int w, h;					/*the width and height of the frames of the sprites, not the file itself*/
  int framesperline;			/*default is 16*/
  int used;					/*used by the maintanence functions to keep track of how many times a single sprite is being used*/
}Sprite;


Sprite SpriteList[MaxSprites];
Sprite *Msprite;
void InitSpriteList();
void FreeSprite(Sprite *img);
Sprite *LoadSprite(char* filename,int sizex, int sizey);		/*simplest file to load*/
void updateSprite(Sprite *img);
void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame);
void CloseSprites();	/*call this before you exit to make sure all the memory that your program used is given back to your OS*/

#endif