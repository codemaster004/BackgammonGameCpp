//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>

#include "../configs/UIConfigs.h"
#include "Handeler.h"
#include "Drawing.h"


void generateBasicBoard(UserInterface *ui, const int *menuSelected, int *dice1, int *dice2) {
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
	handleIndexes(indexes, DEFAULT, (int) (digits), ui->space.indexesTop, ui->space.indexesBottom);
	attroff(COLOR_PAIR(FOREGROUND));

//	handleMenu(testMenu, N_MENU_OPTIONS, *menuSelected, boardCenter.x,
//			   boardSpace.max.y + MENU_TOP_SPACING);

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

void handleIndexes(char **indexes, MenuMode drawMode, int digits, Placement pos1, Placement pos2) {
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

void handleMenu(MenuElement *options, int length, int selected, int centerX, int offsetY) {
	uint menuRealLen = OPTION_SPACING * (length - 1);
	for (int i = 0; i < length; ++i) {
		menuRealLen += len(options[i].value) - 1;
	}

	int startingPoint = centerX - (int) (menuRealLen / 2);
	for (int i = 0; i < length; ++i) {
		if (options[i].id == selected) {
			printColor(FOREGROUND_LIGHT, startingPoint, offsetY, options[i].value);
		} else {
			printColor(FOREGROUND_DARK, startingPoint, offsetY, options[i].value);
		}
		startingPoint += (int) (len(options[i].value) - 1) + OPTION_SPACING;
	}
}
