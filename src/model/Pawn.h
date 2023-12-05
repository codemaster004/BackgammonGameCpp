//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

#include "cstdint"

#include "History.h"

typedef struct Player Player;
typedef struct Board Board;
typedef struct Bar Bar;
typedef struct Point Point;

typedef struct Move {
	int from;
	int by;
	int movesLeft;
} Move;

enum MoveType {
	BLOCKED,
	POSSIBLE,
	CAPTURE,
	NOT_ALLOWED,
	ESCAPE_BOARD,
};

enum PawnColor {
	PAWN_WHITE,
	PAWN_BLACK,
};

enum MoveStatus {
	MOVE_SUCCESSFUL,
	PAWNS_ON_BAR,
	MOVE_FAILED,
	MOVE_TO_COURT,
};

typedef struct {
	int ownerId;
	int id;
	bool isHome;
	PawnColor color;
	short moveDirection;
} Pawn;

int canBeMoved(Board &game, int pointIndex, int moveBy);

MoveType canMoveTo(Board &game, Pawn *pawn, int toIndex);

MoveType determineMoveType(Board &game, int pointIndex, int moveBy);

MoveStatus moveBarToPoint(Board &game, Move move, int indexOnBar, MoveMade &history);

MoveStatus movePointToPoint(Board &game, Move move, MoveMade &history);

MoveStatus movePawn(Board &game, Move move, MoveMade &history);

void movePointToCourt(Board &game, MoveMade &history, int fromIndex);

bool enumToBool(MoveType value);

void movePointToBar(Board &game, MoveMade &history, int fromIndex, int toIndex);

int hasPawnsOnBar(Bar &bar, int playerId);

short findMoveDirection(Pawn **pawns, int count, int playerId);

/// Handle Serialization of Pawn object
void serialisePawn(Pawn pawn, uint8_t *buffer, size_t &offset);

Pawn deserializePawn(const uint8_t *buffer, size_t &index);

/// Handle POINTER serialization to a Pawn object
void serialisePawnPointer(Pawn *pawn, uint8_t* buffer, size_t &offset);

Pawn *deserializePawnPointer(Board &board, const uint8_t *buffer, size_t &offset);

#endif //BACKGAMMONGAME_PAWN_H
