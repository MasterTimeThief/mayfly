#ifndef __MULTI__
#define __MULTI__

#include "mayfly.h"
#include "combat.h"

Mayfly player1List[10];
Mayfly player2List[10];
int player1Positions[10][2];
int player2Positions[10][2];

void beginMultiplayer();
void setupMayflyMultiplayerPositions();
void initPlayerList(Mayfly list[]);
void initPlayerList(Mayfly list[], int player);
Mayfly *newMultiplayerMayfly(Mayfly list[]);
void setupMultiplayerMayfly(Mayfly *m);
void createMultiplayerMayfly(Mayfly list[]);
void multiplayerPositions(Mayfly *m, int index, int player);
void changePlayer2Sprites();

void toMultiCombat();
void endMultiCombat();
void displayMultiAttack();
void displayMultiResults();
void chooseMultiplayerFighters();
void resetMultiFighters();
void p1Attack();
void p2Attack();
void multiplayerThink();

#endif