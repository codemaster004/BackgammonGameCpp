//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>

#include "../configs/UIConfigs.h"
#include "Handeler.h"
#include "Drawing.h"


Placement initMenu(Pos center, MenuElement *values, int nElements) {
	uint realLen = OPTION_SPACING * (nElements - 1);
	for (int i = 0; i < nElements; ++i) {
		realLen += len(values[i].value) - 1;
	}
	return Placement{
		.min={
			.x=center.x - (int) (realLen / 2),
			.y=center.y
		},
		.max {.x=center.x + (int) (realLen / 2)}
	};
}

void generateBasicBoard(UserInterface *ui) {
	attron(COLOR_PAIR(FOREGROUND));

	handleBoardOutline(ui->space.board);
	handleBar();
	handlePieces(ui->space.board);

	// Dies
	handleDices(ui->space.dice, ui->space.boardCenter);

	attroff(COLOR_PAIR(FOREGROUND));
}

void generateInteractiveUI(UserInterface *ui, const int *menuSelected) {
	auto *testMenu = new MenuElement[N_MENU_OPTIONS];
	for (int i = 0; i < N_MENU_OPTIONS; ++i) {
		testMenu[i] = MenuElement{.id=i, .value=menuOptions[i]};
	}

	// Indexes
	uint digits = nDigits(nPoints, 10);
	char **indexes = new char *[nPoints];
	for (int i = 0; i < nPoints; ++i) {
		indexes[i] = numberToString(i, (int) (digits));
	}
	attron(COLOR_PAIR(FOREGROUND));
	handleIndexes(indexes, (int) (digits), ui->space.indexesTop, ui->space.indexesBottom);
	attroff(COLOR_PAIR(FOREGROUND));

	handleMenu(testMenu, N_MENU_OPTIONS, *menuSelected,
			   Pos{ui->space.boardCenter.x, ui->space.board.max.y + MENU_TOP_SPACING});

	handlePawnPlacement(&ui->board, ui->space.board);

	/// CLEAR MEMORY!!!
	for (int i = 0; i < nPoints; ++i) {
		delete[] indexes[i];
	}
	delete[] indexes;
	delete[] testMenu;

}

void handleBoardOutline(Placement space) {
	space.max.y = space.min.y + boardHeight + borders - 1;
	for (int i = 0; i < N_BOARDS; ++i) {
		space.max.x = space.min.x + boardWidth + borders - 1;
		drawBorders(space);

		space.min.x += boardWidth + borders + BAR_WIDTH;
	}
}

void handlePieces(Placement space) {
	space.min.x += BORDER_WIDTH;
	space.min.y += BORDER_WIDTH;
	for (int i = 0; i < N_BOARDS; ++i) {
		drawPieces(space.min.x + (boardWidth + borders + BAR_WIDTH) * i, space.min.y);
	}
}

void handleBar() {
	for (int i = N_BOARDS - 1; i > 0; --i)
		drawBar(OFFSET_X + (boardWidth + borders) * i, OFFSET_Y, boardHeight + borders);
}

void handleDices(Placement space, Pos center) {
	drawBorders(space);

	int dicesHeight = DICE_HEIGHT * N_DICES + (N_DICES + 1) * BORDER_WIDTH;
	space.min.y = center.y - dicesHeight / 2;
	for (int i = 0; i < N_DICES; ++i) {
		space.max.y = space.min.y + DICE_HEIGHT + 2 * BAR_WIDTH - 1;
		drawBorders(space);
		mvaddch(space.min.y + BORDER_WIDTH, space.min.x + BORDER_WIDTH + DICE_WIDTH / 2, 0 + 48);
		space.min.y += DICE_HEIGHT + BORDER_WIDTH;
	}
}

void drawBar(int offsetX, int offsetY, int height) {
	mvprintw(offsetY, offsetX, borderCorner);
	drawLine(borderVertical, Placement{offsetX, offsetY + 1,
									   offsetX, offsetY + height - 1});
	mvprintw(offsetY + height - 1, offsetX, borderCorner);

	mvprintw(offsetY + (height) / 2, offsetX - (int) (sizeof(barLabel)) / 2 + 1, barLabel);
}

void handleIndexes(char **indexes, int digits, Placement pos1, Placement pos2) {
	revertTable(indexes, indexes + nPoints / 2);

	for (int i = 0; i < N_BOARDS; ++i) {
		// TODO: this value as default with for one
		pos1.max.x = pos1.min.x + pieceWidth * POINTS_PER_BOARD + pieceSpacing * (POINTS_PER_BOARD - 1);
		pos2.max.x = pos1.max.x;
		drawSpacedText(pos2, pieceSpacing, (int) (digits),
					   &indexes[i * POINTS_PER_BOARD], POINTS_PER_BOARD);
		drawSpacedText(pos1, pieceSpacing, (int) (digits),
					   &indexes[nPoints / 2 + i * POINTS_PER_BOARD], POINTS_PER_BOARD);
		// TODO: create a fucntion to handle moving by offset
		pos1.min.x = pos1.max.x + BORDER_WIDTH * 2 + BAR_WIDTH + pieceSpacing / 2 * 2;
		pos2.min.x = pos1.min.x;
	}
}

void handleMenu(MenuElement *options, int optionCount, int selected, Pos center) {
	Placement menuSpace = initMenu(center, options, optionCount);

	for (int i = 0; i < optionCount; ++i) {
		if (options[i].id == selected) {
			printColor(FOREGROUND_LIGHT, menuSpace.min.x, menuSpace.min.y, options[i].value);
		} else {
			printColor(FOREGROUND_DARK, menuSpace.min.x, menuSpace.min.y, options[i].value);
		}
		menuSpace.min.x += (int) (len(options[i].value) - 1) + OPTION_SPACING;
	}
}

void handlePawnPlacement(Board *game, Placement space) {
	space.min.x += BORDER_WIDTH + pieceSpacing / 2;
	space.min.y += BORDER_WIDTH;
	space.max.x = space.min.x;
	int change = pieceWidth + pieceSpacing;
	for (int i = 0; i < N_BOARDS; ++i) {
		for (int j = 0; j < POINTS_PER_BOARD; ++j) {
			int indexTop = POINTS_PER_BOARD * N_BOARDS + i * POINTS_PER_BOARD + j;
			int count = game->points[indexTop].pawnsInside;
			if (count) {
				space.max.y = space.min.y + count;
				drawLine(*game->points[indexTop].pawns[0], space);
			}

			int indexBot = POINTS_PER_BOARD * N_BOARDS - (i * POINTS_PER_BOARD + j) - 1;
			count = game->points[indexBot].pawnsInside;
			if (count) {
				moveSpace(&space, Pos {0, boardHeight - count});
				drawLine(*game->points[indexBot].pawns[0], space);
				moveSpace(&space, Pos {0, - boardHeight + count});
			}

//			drawLine(pawn1, space);
			moveSpace(&space, Pos{change, 0});
		}
		moveSpace(&space, Pos{pieceSpacing / 2 * 2 + borders + BAR_WIDTH - pieceSpacing, 0});
//		int totalOffset = offsetX + pieceSpacing / 2;
//		for (int i = 0; i < PAWNS_PER_POINT; i += 2) {
//			Placement offset = {
//				.min = Pos{.x = totalOffset, .y = offsetY},
//				.max = Pos{.x = totalOffset, .y = offsetY + PAWNS_PER_POINT}
//			};
//			// placement
//			drawLine(symbol, offset);
//			totalOffset += (pieceWidth + pieceSpacing) * 2;
//		}
	}
}
