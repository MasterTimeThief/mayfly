#include "audio.h"
#include "room.h"

Mix_Music *menuMusic = NULL;
Mix_Music *draftMusic = NULL;
Mix_Music *trainMusic = NULL;
Mix_Music *healMusic = NULL;
Mix_Music *breedMusic = NULL;
Mix_Music *scoutMusic = NULL;
Mix_Music *combatMusic = NULL;
Mix_Music *winMusic = NULL;
Mix_Music *loseMusic = NULL;

void initAudio()
{
	menuMusic	= Mix_LoadMUS("sounds/menuMusic.wav");
	draftMusic	= Mix_LoadMUS("sounds/draftMusic.wav");
	trainMusic	= Mix_LoadMUS("sounds/trainMusic.wav");
	healMusic	= Mix_LoadMUS("sounds/healMusic.wav");
	breedMusic	= Mix_LoadMUS("sounds/breedMusic.wav");
	scoutMusic	= Mix_LoadMUS("sounds/scoutMusic.wav");
	combatMusic = Mix_LoadMUS("sounds/combatMusic.wav");
	editMusic	= Mix_LoadMUS("sounds/editMusic.wav");
	winMusic	= Mix_LoadMUS("sounds/winMusic.wav");;
	loseMusic	= Mix_LoadMUS("sounds/loseMusic.wav");;
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
	else if (gameRoom->roomName == COMBAT || gameRoom->roomName == MULTI)
	{
		Mix_PlayMusic( combatMusic, -1 );
	}
	else if (gameRoom->roomName == EDIT)
	{
		Mix_PlayMusic( editMusic, -1 );
	}
	else if (gameRoom->roomName == WIN)
	{
		Mix_PlayMusic( winMusic, 0 );
	}
	else if (gameRoom->roomName == LOSE)
	{
		Mix_PlayMusic( loseMusic, -1 );
	}
}