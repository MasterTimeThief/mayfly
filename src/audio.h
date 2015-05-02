#ifndef __AUDIO__
#define __AUDIO__

#include "SDL_mixer.h"

Mix_Music *menuMusic;
Mix_Music *draftMusic;
Mix_Music *trainMusic;
Mix_Music *healMusic;
Mix_Music *breedMusic;
Mix_Music *scoutMusic;
Mix_Music *combatMusic;

//The sound effects that will be used
Mix_Chunk *scratch;
Mix_Chunk *high;
Mix_Chunk *med;
Mix_Chunk *low;

void initAudio();
void changeBackgroundMusic();

#endif