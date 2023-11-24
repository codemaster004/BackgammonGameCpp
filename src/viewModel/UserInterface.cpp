//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "UserInterface.h"

UserInterface initUI() {
	UserInterface ui = UserInterface{
		.authorId=197712,
		.menuMode=DEFAULT,
		.space=GameSpace{initBoard(),
		}
	};
	Placement board = ui.space.board;
	ui.space.boardCenter = initCenter(board);
	ui.space.dice = initDice(board);
	ui.space.indexesTop = initIndex(board, board.min.y - 1);
	ui.space.indexesBottom = initIndex(board, board.max.y + 1);

	return ui;
}
