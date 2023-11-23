//
// Created by Filip Dabkowski on 23/11/2023.
//

#include "Space.h"
#include "../configs/UIConfigs.h"


Placement initBoard() {
	Placement space{.min={OFFSET_X, OFFSET_Y}};
	space.max = {space.min.x + N_BOARDS * (boardWidth + borders + BAR_WIDTH) - BAR_WIDTH - 1,
				 space.min.y + boardHeight + borders - 1};
	return space;
}

Placement initDice(Placement board) {
	return Placement{
		.min={board.max.x, board.min.y},
		.max={board.max.x + DICE_WIDTH + borders - 1, board.max.y},
	};
}

Placement initIndex(Placement board, int offset) {
	return Placement{
		.min={
			board.min.x + BORDER_WIDTH + pieceSpacing / 2,
			offset,
		},
		.max={
			board.max.x - BORDER_WIDTH - pieceSpacing / 2,
		}
	};
}

Pos initCenter(Placement space) {
	return Pos{
		.x=space.min.x + (space.max.x - space.min.x) / 2,
		.y=space.min.y + (space.max.y - space.min.y) / 2
	};
}

void moveSpace(Placement *space, Pos by) {
	space->min.x += by.x;
	space->min.y += by.y;
	space->max.y += by.y;
	space->max.x += by.x;
}
