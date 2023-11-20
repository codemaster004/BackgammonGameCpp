//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

#include "Board.h"

// TODO: Create functions for checks if points are valid to be moved & to be moved to

enum moveToPoint {
	BLOCKED,
	POSSIBLE,
	CAPTURE
};

bool canBeMoved(Board *game, int pointIndex, int moveBy);

moveToPoint canMoveTo(Board *game, int fromIndex, int toIndex);

bool enumToBool(moveToPoint value);

#endif //BACKGAMMONGAME_PAWN_H
