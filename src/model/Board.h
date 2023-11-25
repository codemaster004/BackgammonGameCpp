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
	Pawn *pawns[PAWNS_PER_POINT];
} Point;

typedef struct {
	int pawnsInside;
	Pawn *pawns[totalPawns];
} Bar;

typedef struct {
	int pawnsInside;
	Pawn *pawns[PAWNS_PER_PLAYER];
	Player *owner;
} Court;

typedef struct Board {
	Player players[N_PLAYERS];
	Pawn pawns[totalPawns];
	Point points[nPoints];
	Court courts[N_PLAYERS];
	Bar bar;
	int dices[N_DICES];
	int currentPlayerId;
} Board;

void setClearBoard(Board &gameBoard);

void placePawns(Board &gameBoard);


#endif //BACKGAMMONGAME_BOARD_H
