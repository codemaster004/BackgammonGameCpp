//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

#include "Board.h"

enum moveToPoint {
	BLOCKED,
	POSSIBLE,
	CAPTURE
};

int getOwner(Board *game, int pawnId);

short getMvDir(Board *game, int pawnId);

Pawn getPawn(Board *game, int pawnId);

bool canBeMoved(Board *game, int pointIndex, int moveBy);

moveToPoint canMoveTo(Board *game, int fromIndex, int toIndex);

bool enumToBool(moveToPoint value);

#endif //BACKGAMMONGAME_PAWN_H
