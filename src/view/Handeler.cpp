//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>

#include "../configs/UIConfigs.h"
#include "Handeler.h"
#include "Drawing.h"


Placement initMenuSpace(Pos center, MenuElement values[], int nElements) {
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

Placement initTextSpace(Pos center, char **values, int nElements) {
	uint realLen = OPTION_SPACING * (nElements - 1);
	for (int i = 0; i < nElements; ++i) {
		realLen += len(values[i]) - 1;
	}
	return Placement{
		.min={
			.x=center.x - (int) (realLen / 2),
			.y=center.y
		},
		.max {.x=center.x + (int) (realLen / 2)}
	};
}

void generateHeader(UserInterface &ui) {
	const char *title[3] = {ui.gameName, ui.authorName, ui.authorId};

	Placement textSpace = ui.space.board;
	textSpace.min.y = ui.space.board.min.y - 3;
	textSpace.max.x += DICE_WIDTH + borders;
	drawSpreadText(textSpace, (char **) (title), 3);
}

void generateBasicBoard(UserInterface &ui) {
	generateHeader(ui);

	attron(COLOR_PAIR(FOREGROUND));

	handleBoardOutline(ui.space.board);
	handleBar();
	handlePieces(ui.space.board);

	// Dies
	handleDices(ui.space.dice, ui.space.boardCenter, ui.board.dices);

	attroff(COLOR_PAIR(FOREGROUND));
}

void generateInteractiveUI(UserInterface &ui) {

	// Indexes
	uint digits = nDigits(nPoints, 10);
	char **indexes = new char *[nPoints];
	for (int i = 0; i < nPoints; ++i) {
		indexes[i] = numberToString(i, (int) (digits));
	}

	handleIndexes(indexes, ui.pickedIndex, (int) (digits), ui.space.indexesTop, ui.space.indexesBottom);

	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});

	handlePawnPlacement(ui.board, ui.space.board);

	/// CLEAR MEMORY!!!
	for (int i = 0; i < nPoints; ++i) {
		delete[] indexes[i];
	}
	delete[] indexes;
//	delete[] testMenu;

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

void handleDices(Placement space, Pos center, int *dices) {
	drawBorders(space);

	int dicesHeight = DICE_HEIGHT * N_DICES + (N_DICES + 1) * BORDER_WIDTH;
	space.min.y = center.y - dicesHeight / 2;
	for (int i = 0; i < N_DICES; ++i) {
		space.max.y = space.min.y + DICE_HEIGHT + 2 * BAR_WIDTH - 1;
		drawBorders(space);
		mvaddch(space.min.y + BORDER_WIDTH, space.min.x + BORDER_WIDTH + DICE_WIDTH / 2, dices[i] + 48);
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

int generateColorsForIndexes(char **text, int count, int pickedIndex, UiColorsId *&colors) {
	int index = -1;
	for (int i = 0; i < count; ++i) {
		if (stringToNumber(text[i]) == pickedIndex) {
			index = i;
		}
	}
	if (pickedIndex == -1) {
		colors = new UiColorsId [1];
		colors[0] = FOREGROUND;
		return 1;
	} else if (index < 0) {
		colors = new UiColorsId [1];
		colors[0] = FOREGROUND_DARK;
		return 1;
	} else {
		colors = new UiColorsId [count];
		for (int i = 0; i < count; ++i) {
			colors[i] = i == index ? FOREGROUND_LIGHT : FOREGROUND_DARK;
		}
		return count;
	}
}

// TODO: REWRITE
void handleIndexes(char **indexes, int pickedIndex, int digits, Placement posTop, Placement posBot) {
	revertTable(indexes, indexes + nPoints / 2);

	for (int i = 0; i < N_BOARDS; ++i) {
		// TODO: this value as default with for one
		posTop.max.x = posTop.min.x + pieceWidth * POINTS_PER_BOARD + pieceSpacing * (POINTS_PER_BOARD - 1);
		posBot.max.x = posTop.max.x;

		UiColorsId *colors = nullptr;

		int nColours = generateColorsForIndexes(&indexes[i * POINTS_PER_BOARD], POINTS_PER_BOARD, pickedIndex, colors);
		drawCenteredText(posBot, pieceSpacing, (int) (digits),
						 &indexes[i * POINTS_PER_BOARD], POINTS_PER_BOARD, colors, nColours);
		delete[] colors;

		colors = nullptr;
		nColours = generateColorsForIndexes(&indexes[nPoints / 2 + i * POINTS_PER_BOARD], POINTS_PER_BOARD, pickedIndex, colors);
		drawCenteredText(posTop, pieceSpacing, (int) (digits),
						 &indexes[nPoints / 2 + i * POINTS_PER_BOARD], POINTS_PER_BOARD, colors, nColours);
		delete[] colors;

		posTop.min.x = posTop.max.x + BORDER_WIDTH * 2 + BAR_WIDTH + pieceSpacing / 2 * 2;
		posBot.min.x = posTop.min.x;
	}
}

void handleMenu(Menu menu, Pos center) {
	Placement menuSpace = initMenuSpace(center, menu.elements, menu.elementsCount);

	for (int i = 0; i < menu.elementsCount; ++i) {
		if (menu.selected == -1) {
			printColor(FOREGROUND, menuSpace.min.x, menuSpace.min.y, menu.elements[i].value);
		} else {
			if (menu.elements[i].id == menu.selected) {
				printColor(FOREGROUND_LIGHT, menuSpace.min.x, menuSpace.min.y, menu.elements[i].value);
			} else {
				printColor(FOREGROUND_DARK, menuSpace.min.x, menuSpace.min.y, menu.elements[i].value);
			}
		}

		menuSpace.min.x += (int) (len(menu.elements[i].value) - 1) + OPTION_SPACING;
	}
}

void handlePawnPlacement(Board &game, Placement space) {
	space.min.x += BORDER_WIDTH + pieceSpacing / 2;
	space.min.y += BORDER_WIDTH;
	space.max.x = space.min.x;
	int change = pieceWidth + pieceSpacing;
	for (int i = 0; i < N_BOARDS; ++i) {
		for (int j = 0; j < POINTS_PER_BOARD; ++j) {
			int indexTop = POINTS_PER_BOARD * N_BOARDS + i * POINTS_PER_BOARD + j;
			int count = game.points[indexTop].pawnsInside;
			if (count) {
				space.max.y = space.min.y + count;
				drawLine(*game.points[indexTop].pawns[0], space);
			}

			int indexBot = POINTS_PER_BOARD * N_BOARDS - (i * POINTS_PER_BOARD + j) - 1;
			count = game.points[indexBot].pawnsInside;
			if (count) {
				moveSpace(space, Pos {0, boardHeight - count});
				drawLine(*game.points[indexBot].pawns[0], space);
				moveSpace(space, Pos {0, - boardHeight + count});
			}

//			drawLine(pawn1, space);
			moveSpace(space, Pos{change, 0});
		}
		moveSpace(space, Pos{pieceSpacing / 2 * 2 + borders + BAR_WIDTH - pieceSpacing, 0});
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
