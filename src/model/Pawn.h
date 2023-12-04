//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

typedef struct Player Player;
typedef struct Board Board;
typedef struct Bar Bar;
typedef struct Point Point;

typedef struct Move {
	int from;
	int by;
} Move;

enum MoveToPoint {
	BLOCKED,
	POSSIBLE,
	CAPTURE,
	NOT_ALLOWED,
};

enum PawnColor {
	PAWN_WHITE,
	PAWN_BLACK,
};

enum MoveStatus {
	MOVE_SUCCESSFUL,
	PAWNS_ON_BAR,
	MOVE_FAILED,
};

typedef struct {
	int ownerId;
	int id;
	bool isHome;
	bool isOnBar;
	PawnColor color;
	short moveDirection;
} Pawn;

int canBeMoved(Board &game, int pointIndex, int moveBy);

MoveToPoint canMoveTo(Board &game, Pawn *pawn, int toIndex);

MoveToPoint determineMoveType(Board &game, int pointIndex, int moveBy);

MoveStatus moveBarToPoint(Board &game, Move move, int indexOnBar);

MoveStatus movePointToPoint(Board &game, Move move);

MoveStatus movePawn(Board &game, Move move);

bool enumToBool(MoveToPoint value);

void movePointToBar(Board &game, Point *point);

int hasPawnsOnBar(Bar &bar, int playerId);

short findMoveDirection(Pawn **pawns, int count, int playerId);

/// Handle Serialization of Pawn object
void serialisePawn(Pawn pawn, uint8_t *buffer, size_t &offset);

Pawn deserializePawn(const uint8_t *buffer, size_t &index);

/// Handle POINTER serialization to a Pawn object
void serialisePawnPointer(Pawn *pawn, uint8_t* buffer, size_t &offset);

Pawn *deserializePawnPointer(Board &board, const uint8_t *buffer, size_t &offset);

#endif //BACKGAMMONGAME_PAWN_H
