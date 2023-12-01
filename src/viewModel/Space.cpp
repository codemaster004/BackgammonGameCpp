//
// Created by Filip Dabkowski on 23/11/2023.
//

#include "Space.h"
#include "../configs/UIConfigs.h"


Placement initGameSpace() {
	Placement space{.min={OFFSET_X, OFFSET_Y}};
	space.max = {space.min.x + N_BOARDS * (boardWidth + borders + BAR_WIDTH) - BAR_WIDTH + DICE_WIDTH + BORDER_WIDTH - 1,
				 space.min.y + boardHeight + borders - 1};

	return space;
}

Placement initBoard(Placement game) {
	Placement space = game;
	space.min = {space.min.x + BOARD_OFFSET_X,
				 space.min.y + BOARD_OFFSET_Y + HEADER_OFFSET + INDEX_OFFSET + TEXT_HEIGHT * 2};
	space.max = {space.min.x + boardWidth + borders - 1,
				 space.min.y + boardHeight + borders - 1};
	return space;
}

Placement initDice(Placement game) {
	Placement space{
		.min={game.max.x - DICE_WIDTH - borders + 1,
			  game.min.y + HEADER_OFFSET + INDEX_OFFSET + TEXT_HEIGHT * 2},
	};
	space.max={game.max.x, space.min.y + boardHeight + borders - 1};
	return space;
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

Pos initCenter(Placement boardSpace, Placement gameSpace) {
	return Pos{
		.x=gameSpace.min.x + (gameSpace.max.x - gameSpace.min.x) / 2,
		.y=boardSpace.min.y + (boardSpace.max.y - boardSpace.min.y) / 2
	};
}

void moveSpace(Placement &space, Pos by) {
	space.min.x += by.x;
	space.min.y += by.y;
	space.max.y += by.y;
	space.max.x += by.x;
}

int width(Placement space) {
	return 0;
}

int height(Placement space) {
	return 0;
}
