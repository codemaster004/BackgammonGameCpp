//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>
#include "cmath"
#include "string"
#include "cstdio"

#include "configs/UIConfigs.h"
#include "configs/GameConfigs.h"
#include "ViewHandeler.h"
#include "Utility.h"
#include "ViewModel.h"

// TODO: Figure out how this works

// TODO: Implement UI generating
void uiStaff(const int *menuSelected, int *dice1, int *dice2) {

	auto *testMenu = new MenuElement[N_MENU_OPTIONS];
	for (int i = 0; i < N_MENU_OPTIONS; ++i) {
		testMenu[i] = MenuElement{.id=i, .value=menuOptions[i]};
	}

	// TODO: Separete functionS
	Pos boardStart = {.x=boardOffsetX, .y=boardOffsetY};
	Pos boardEnd = {.x=boardOffsetX, .y=boardOffsetY};
	attron(COLOR_PAIR(FOREGROUND));
	int boardWidth = POINTS_PER_BOARD * pieceWidth + pieceSpacing * (POINTS_PER_BOARD - 1) + pieceSpacing / 2 * 2;
	int boardHeight = PAWNS_PER_POINT * 2 + pieceSpacing;

	int borders = BORDER_WIDTH * 2;
	boardEnd.y = boardHeight + borders;
//	int endOfBoard = boardOffsetX;
	for (int _ = 0; _ < N_BOARDS; ++_) {
		drawBorders(boardEnd.x, boardStart.y,
					boardWidth + borders, boardEnd.y);
		drawPieces(boardEnd.x + BORDER_WIDTH, boardStart.y + BORDER_WIDTH);
		boardEnd.x += (boardWidth + borders + BAR_WIDTH);
	}
	for (int i = N_BOARDS - 1; i > 0; --i)
		drawBar(boardOffsetX + (boardWidth + borders) * i, boardOffsetY, boardHeight + borders);
	boardEnd.x -= BAR_WIDTH;

	Pos boardCenter = {
		.x=boardStart.x + (boardEnd.x - boardStart.x) / 2,
		.y=boardStart.y + (boardEnd.y - boardStart.y) / 2
	};

	// Dies
	// TODO: Rewrite with for N_DICES
	drawBorders(boardEnd.x - 1, boardStart.y, DICE_WIDTH + borders, boardHeight + borders);
	drawBorders(boardEnd.x - 1, boardCenter.y - DICE_HEIGHT - BORDER_WIDTH,
				DICE_WIDTH + borders, 2 * DICE_HEIGHT + 3 * BAR_WIDTH);
	drawBorders(boardEnd.x - 1, boardCenter.y, DICE_WIDTH + borders, BORDER_WIDTH);
	mvaddch(boardOffsetY + boardHeight / 2 + 1 + 1, boardEnd.x + DICE_WIDTH / 2, *dice1 + 48);
	mvaddch(boardOffsetY + boardHeight / 2, boardEnd.x + DICE_WIDTH / 2, *dice2 + 48);

	// Indexes
	int totalNumbers = nPoints * N_PLAYERS;
	uint digits = nDigits(totalNumbers, 10);
	char **indexes = new char *[totalNumbers];
	for (int i = 0; i < totalNumbers; ++i) {
		indexes[i] = numberToString(i, (int) (digits));
	}
	revertTable(indexes, indexes + nPoints);

	int start = boardStart.x + BORDER_WIDTH + pieceSpacing / 2;
	for (int i = 0; i < N_BOARDS; ++i) {
		int change = pieceWidth * POINTS_PER_BOARD + pieceSpacing * (POINTS_PER_BOARD - 1);
		drawSpacedText(start, start + change, boardEnd.y, pieceSpacing, (int) (digits),
					   &indexes[i * POINTS_PER_BOARD], POINTS_PER_BOARD);
		start += change + borders + BAR_WIDTH + pieceSpacing / 2 * 2;
	}

	attroff(COLOR_PAIR(FOREGROUND));

	drawMenu(testMenu, N_MENU_OPTIONS, *menuSelected, boardCenter.x,
			 boardEnd.y + MENU_TOP_SPACING);

	move(0, 0);
	// Refresh the screen to show changes
	refresh();

	/// CLEAR MEMORY!!!
	free(testMenu);

	for (int i = 0; i < totalNumbers; ++i) {
		delete[] indexes[i];
	}
	delete[] indexes;
}

void setColourTheme(short baseRed, short baseGreen, short baseBlue) {
	float nMultiplier = 1000.0 / 255.0;
	short nRed = multiplyFloat(baseRed, nMultiplier, 1000.0),
		nGreen = multiplyFloat(baseGreen, nMultiplier, 1000.0),
		nBlue = multiplyFloat(baseBlue, nMultiplier, 1000.0);
	init_color(COLOUR_MAIN, nRed, nGreen, nBlue);
	// TODO: Conversion in a function
	init_color(COLOUR_MAIN_DARK,
			   multiplyFloat(nRed, (1 - colorDiff), 1000.0),
			   multiplyFloat(nGreen, (1 - colorDiff), 1000.0),
			   multiplyFloat(nBlue, (1 - colorDiff), 1000.0));
	init_color(COLOUR_MAIN_LIGHT,
			   multiplyFloat(nRed, (1 + colorDiff), 1000.0),
			   multiplyFloat(nGreen, (1 + colorDiff), 1000.0),
			   multiplyFloat(nBlue, (1 + colorDiff), 1000.0));
}

void drawBorders(int offsetX, int offsetY, int width, int height) {
	Pos corTL = {.x=offsetX, .y=offsetY},
		corTR = {.x=offsetX + width - 1, .y=offsetY},
		corBL = {.x=offsetX, .y=offsetY + height - 1},
		corBR = {.x=offsetX + width - 1, .y=offsetY + height - 1};
	mvprintw(corTL.y, corTL.x, borderCorner);
	mvprintw(corTR.y, corTR.x, borderCorner);
	mvprintw(corBL.y, corBL.x, borderCorner);
	mvprintw(corBR.y, corBR.x, borderCorner);

	drawLine(borderHorizon, corTL.x + 1, corTR.x, corTL.y, corTR.y);
	drawLine(borderHorizon, corBL.x + 1, corBR.x, corBL.y, corBR.y);
	drawLine(borderVertical, corTL.x, corBL.x, corTL.y + 1, corBL.y);
	drawLine(borderVertical, corTR.x, corBR.x, corTR.y + 1, corBR.y);
}

void drawPiece(const char *symbol, int offsetX, int offsetY) {
	int totalOffsetX = offsetX + pieceSpacing / 2;
	int totalOffsetY = offsetY;
	for (int i = 0; i < PAWNS_PER_POINT; i += 2) {
		drawLine(symbol, totalOffsetX, totalOffsetX, totalOffsetY, totalOffsetY + PAWNS_PER_POINT);
		totalOffsetX += (pieceWidth + pieceSpacing) * 2;
	}
}

void drawLine(const char *symbol, int fromX, int toX, int fromY, int toY) {
	int deltaX = toX - fromX;
	int deltaY = toY - fromY;

	int steps = max(abs(deltaX), abs(deltaY));

	float xIncrement = (float) (deltaX) / (float) (steps);
	float yIncrement = (float) (deltaY) / (float) (steps);

	auto x = (float) (fromX);
	auto y = (float) (fromY);

	for (int i = 0; i < steps; ++i) {
		mvprintw((int) (round(y)), (int) (round(x)), "%s", symbol);
		x += xIncrement;
		y += yIncrement;
	}
}

void drawPieces(int offsetX, int offsetY) {
	char *reversePiece = reverseString(piece1);
	drawPiece(reversePiece, offsetX, offsetY);
	reversePiece = reverseString(piece2);
	drawPiece(reversePiece, offsetX + pieceWidth + pieceSpacing, offsetY);
	drawPiece(piece1, offsetX + pieceWidth + pieceSpacing,
			  offsetY + PAWNS_PER_POINT + pieceSpacing);
	drawPiece(piece2, offsetX, offsetY + PAWNS_PER_POINT + pieceSpacing);
}

void drawBar(int offsetX, int offsetY, int height) {
	mvprintw(offsetY, offsetX, borderCorner);
	drawLine(borderVertical, offsetX, offsetX,
			 offsetY + 1, offsetY + height - 1);
	mvprintw(offsetY + height - 1, offsetX, borderCorner);

	mvprintw(offsetY + (height) / 2, offsetX - (int) (sizeof(barLabel)) / 2 + 1, barLabel);
}

void drawSpacedText(int minX, int maxX, int offsetY, const int spacing, const int len, char **text, const int count) {
	int center = minX + (maxX - minX) / 2;
	int textWidth = count * (len) + spacing * (count - 1);
	int startPoint = center - textWidth / 2;
	if (startPoint < minX)
		return;

	for (int i = 0; i < count; ++i) {
		mvprintw(offsetY, startPoint, "%s", text[i]);
		startPoint += len + spacing;
	}
}

// TODO: DIVIDE
void drawMenu(MenuElement *options, int length, int selected, int centerX, int offsetY) {
	uint menuRealLen = OPTION_SPACING * (length - 1);
	for (int i = 0; i < length; ++i) {
		menuRealLen += len(options[i].value) - 1;
	}

	int startingPoint = centerX - (int) (menuRealLen / 2);
	for (int i = 0; i < length; ++i) {
		if (options[i].id == selected) {
			attron(COLOR_PAIR(FOREGROUND_LIGHT));
			mvprintw(offsetY, startingPoint, "%s", options[i].value);
			attroff(COLOR_PAIR(FOREGROUND_LIGHT));
		} else {
			attron(COLOR_PAIR(FOREGROUND_DARK));
			mvprintw(offsetY, startingPoint, "%s", options[i].value);
			attroff(COLOR_PAIR(FOREGROUND_DARK));
		}
		startingPoint += (int) (len(options[i].value) - 1) + OPTION_SPACING;
	}
}
