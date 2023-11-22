//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>

#include "../configs/UIConfigs.h"
#include "Handeler.h"
#include "Drawing.h"


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

Pos initCenter(Placement space) {
	return Pos{
		.x=space.min.x + (space.max.x - space.min.x) / 2,
		.y=space.min.y + (space.max.y - space.min.y) / 2
	};
}

// TODO: Implement UI generating
void uiStaff(UserInterface *ui, const int *menuSelected, int *dice1, int *dice2) {

	auto *testMenu = new MenuElement[N_MENU_OPTIONS];
	for (int i = 0; i < N_MENU_OPTIONS; ++i) {
		testMenu[i] = MenuElement{.id=i, .value=menuOptions[i]};
	}

	// TODO: Separete functionS
	Placement boardSpace = initBoard();
	attron(COLOR_PAIR(FOREGROUND));

	handleBoardOutline(boardSpace);
	handleBar();
	handlePieces(boardSpace);

	Pos boardCenter = initCenter(boardSpace);

	// Dies
	Placement diceSpace = initDice(boardSpace);
	handleDices(diceSpace, boardCenter);

	// Indexes
	uint digits = nDigits(nPoints, 10);
	char **indexes = new char *[nPoints];
	for (int i = 0; i < nPoints; ++i) {
		indexes[i] = numberToString(i, (int) (digits));
	}
	handleIndexes(indexes, DEFAULT, (int) (digits), boardSpace);

	attroff(COLOR_PAIR(FOREGROUND));

	handleMenu(testMenu, N_MENU_OPTIONS, *menuSelected, boardCenter.x,
			   boardSpace.max.y + MENU_TOP_SPACING);

	// Refresh the screen to show changes
	refresh();

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

void handleIndexes(char **indexes, MenuMode drawMode, int digits, Placement pos) {
	revertTable(indexes, indexes + nPoints / 2);

	int start = pos.min.x + BORDER_WIDTH + pieceSpacing / 2;
	for (int i = 0; i < N_BOARDS; ++i) {
		int change = pieceWidth * POINTS_PER_BOARD + pieceSpacing * (POINTS_PER_BOARD - 1);
		drawSpacedText(start, start + change, pos.max.y + 1, pieceSpacing, (int) (digits),
					   &indexes[i * POINTS_PER_BOARD], POINTS_PER_BOARD);
		drawSpacedText(start, start + change, pos.min.y - 1, pieceSpacing, (int) (digits),
					   &indexes[nPoints / 2 + i * POINTS_PER_BOARD], POINTS_PER_BOARD);
		start += change + BORDER_WIDTH * 2 + BAR_WIDTH + pieceSpacing / 2 * 2;
	}
}

// TODO: DIVIDE
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
