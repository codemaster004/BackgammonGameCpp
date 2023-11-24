//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_BOARD_H
#define BACKGAMMONGAME_BOARD_H

#include "../configs/GameConfigs.h"
#include "Player.h"


enum PawnColor {
	PAWN_WHITE,
	PAWN_BLACK,
};

typedef struct {
	int ownerId;
	int id;
	bool isHome;
	bool isOnBar;
	PawnColor color;
	short moveDirection;
} Pawn;

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
	int *ownerId;
} Court;

typedef struct {
	Point points[nPoints];
	Bar bar;
	Court courts[N_PLAYERS];
	Player players[N_PLAYERS];
	int currentPlayerId;
} Board;

void setClearBoard(Board *gameBoard);

void setupGame(Board *gameBoard, Pawn player1[], Pawn player2[]);



#endif //BACKGAMMONGAME_BOARD_H
