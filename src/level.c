/*#include <string.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"
#include "sprite.h"
#include "level.h"
extern SDL_Surface *screen;

static Level __level;

void LoadLevel(char *filename)
{
	FILE *levelfile = NULL;
	int w,h;
	char buffer[128];
	char bgimagepath[128];
	Level_Type level;
	Sprite *temp;
	levelfile = fopen(filename,"r");
	

	if (levelfile == NULL)
	{
		fprintf(stdout,"LoadLevel: warning, filename is NULL\n");
		return;
	}
	while(fscanf(levelfile, "%s", buffer) != EOF)
	{
		if (strncmp(buffer, "image:",128) == 0)
		{
			fscanf(levelfile, "%s", bgimagepath);
		}
		else if (strncmp(buffer, "width:",128) == 0)
		{
			fscanf(levelfile, "%s", &w);
		}
		else if (strncmp(buffer, "height:",128) == 0)
		{
			fscanf(levelfile, "%s", &h);
		}
		else if (strncmp(buffer, "level:",128) == 0)
		{
			fscanf(levelfile, "%s", &level);
		}
	}
	fclose(levelfile);
	temp = LoadSprite(bgimagepath, w, h);
	if (!temp)
	{
		fprintf(stdout,"LoadLevel: ERROR, could NOT (FUCK YOU CARAMEL) open sprite file\n");
		return;
	}
	CloseLevel();
	__level.bgImage = temp;
	__level.loaded = 1;
}

void DrawLevel()
{
	if (!__level.loaded) return;
	if (__level.bgImage)
	{
		DrawSprite(__level.bgImage,screen,0,0,0);
	}
}

void CloseLevel()
{
	if (!__level.loaded) return;
	if (__level.bgImage)
	{
		FreeSprite(__level.bgImage);
		__level.bgImage = NULL;
	}
}

void InitLevelSystem()
{
	memset(&__level,0,sizeof(Level));
	atexit(CloseLevel);
}

void MayflyGrid()
{

}*/