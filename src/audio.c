#include "audio.h"
#include "room.h"

Mix_Music *menuMusic = NULL;
Mix_Music *draftMusic = NULL;
Mix_Music *trainMusic = NULL;
Mix_Music *healMusic = NULL;
Mix_Music *breedMusic = NULL;
Mix_Music *scoutMusic = NULL;
Mix_Music *combatMusic = NULL;

//The sound effects that will be used
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

void initAudio()
{
	menuMusic = Mix_LoadMUS("sounds/menuMusic.wav");
	draftMusic = Mix_LoadMUS("sounds/draftMusic.wav");
	trainMusic = Mix_LoadMUS("sounds/trainMusic.wav");
	healMusic = Mix_LoadMUS("sounds/healMusic.wav");
	breedMusic = Mix_LoadMUS("sounds/breedMusic.wav");
	scoutMusic = Mix_LoadMUS("sounds/scoutMusic.wav");
	combatMusic = Mix_LoadMUS("sounds/combatMusic.wav");
	editMusic = Mix_LoadMUS("sounds/editMusic.wav");

	//The sound effects that will be used
	scratch = Mix_LoadWAV("sounds/sfx.wav");;
	high = Mix_LoadWAV("sounds/sfx.wav");;
	med = Mix_LoadWAV("sounds/sfx.wav");;
	low = Mix_LoadWAV("sounds/sfx.wav");;
}

void changeBackgroundMusic()
{
	if (Mix_PlayingMusic()) Mix_HaltMusic();
	
	if (gameRoom->roomName == MENU)
	{
		Mix_PlayMusic( menuMusic, -1 );
	}
	else if (gameRoom->roomName == MAIN)
	{
		switch (gameRoom->mode)
		{
		case DRAFT:	
			Mix_PlayMusic( draftMusic, -1 );
			break;
		case TRAIN:	
			Mix_PlayMusic( trainMusic, -1 );
			break;
		case HEAL:	
			Mix_PlayMusic( healMusic, -1 );
			break;
		case SCOUT:	
			Mix_PlayMusic( scoutMusic, -1 );
			break;
		case BREED:	
			Mix_PlayMusic( breedMusic, -1 );
			break;
		}
	}
	else if (gameRoom->roomName == COMBAT)
	{
		Mix_PlayMusic( combatMusic, -1 );
	}
	else if (gameRoom->roomName == EDIT)
	{
		Mix_PlayMusic( editMusic, -1 );
	}
}