//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_BOARD_H
#define BACKGAMMONGAME_BOARD_H

#include "../configs/GameConfigs.h"
#include "Player.h"
#include "Pawn.h"
#include "Storage.h"

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

void clearDices(int *dices);

void placePawns(Board &gameBoard);

void serialiseBoard(Board &board, uint8_t *buffer, size_t &offset);

Board deserializeBoard(const uint8_t *buffer, size_t &offset);


#endif //BACKGAMMONGAME_BOARD_H
