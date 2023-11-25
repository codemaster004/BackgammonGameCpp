//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_BOARD_H
#define BACKGAMMONGAME_BOARD_H

#include "../configs/GameConfigs.h"
#include "Player.h"
#include "Pawn.h"


typedef struct {
	int pawnsInside;
	int pawnsId[PAWNS_PER_POINT];
} Point;

typedef struct {
	int pawnsInside;
	int pawnsId[totalPawns];
} Bar;

typedef struct {
	int pawnsInside;
	int pawnsId[PAWNS_PER_PLAYER];
	int ownerId;
} Court;

typedef struct {
	Pawn pawns[PAWNS_PER_PLAYER];
} PlayerPawns;

typedef struct Board {
	Point points[nPoints];
	PlayerPawns pawnGroups[N_PLAYERS];
	Bar bar;
	Court courts[N_PLAYERS];
	int dices[N_DICES];
	Player players[N_PLAYERS];
	int currentPlayerId;
} Board;

void setClearBoard(Board *gameBoard);

void placePawns(Board *gameBoard);


#endif //BACKGAMMONGAME_BOARD_H
