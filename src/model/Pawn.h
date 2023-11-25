//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

#include "Player.h"

typedef struct Board Board;

enum moveToPoint {
	BLOCKED,
	POSSIBLE,
	CAPTURE
};

enum PawnColor {
	PAWN_WHITE,
	PAWN_BLACK,
};

typedef struct {
	Player *owner;
	int id;
	bool isHome;
	bool isOnBar;
	PawnColor color;
	short moveDirection;
} Pawn;

bool canBeMoved(Board &game, int pointIndex, int moveBy);

moveToPoint canMoveTo(Board &game, int fromIndex, int toIndex);

bool enumToBool(moveToPoint value);

#endif //BACKGAMMONGAME_PAWN_H
