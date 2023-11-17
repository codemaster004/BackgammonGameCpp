//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>
#include "cmath"
#include "string"

#include "configs/UIConfigs.h"
#include "configs/GameConfigs.h"
#include "ViewHandeler.h"
#include "Utility.h"

// TODO: Figure out how this works

const int boardMarginVertical = 5;
const int boardMarginHorizontal = 5;

enum UiColorsId {
	FOREGROUND = 1,
	FOREGROUND_LIGHT,
	FOREGROUND_DARK,
	BACKGROUND_LIGHT,
	BACKGROUND_DARK
};

// TODO: Implement UI generating
void uiStaff() {
	initscr();

	start_color();

	setColourTheme();

	// Define colors
	init_pair(FOREGROUND, COLOUR_MAIN, COLOR_BLACK);
	init_pair(FOREGROUND_LIGHT, COLOUR_MAIN_LIGHT, COLOR_BLACK);
	init_pair(FOREGROUND_DARK, COLOUR_MAIN_DARK, COLOR_BLACK);
	init_pair(BACKGROUND_LIGHT, COLOR_BLACK, COLOUR_MAIN_LIGHT);
	init_pair(BACKGROUND_DARK, COLOR_BLACK, COLOUR_MAIN_DARK);

	attron(COLOR_PAIR(FOREGROUND));
	int boardWidth = PIECE_PER_BOARD * pieceWidth + pieceSpacing * (PIECE_PER_BOARD - 1) + pieceSpacing / 2 * 2;
	int boardHeight = PIECE_PER_BOARD * 2 + pieceSpacing;
	drawBorders(boardOffsetX, boardOffsetY, boardWidth + BORDER_WIDTH * 2, boardHeight + BORDER_WIDTH * 2);

	char *reversePiece = reverseString(piece1);
	drawPieces(reversePiece, boardOffsetX + BORDER_WIDTH, boardOffsetY + BORDER_WIDTH);
	reversePiece = reverseString(piece2);
	drawPieces(reversePiece, boardOffsetX + BORDER_WIDTH + pieceWidth + pieceSpacing, boardOffsetY + BORDER_WIDTH);

	// Refresh the screen to show changes
	refresh();

	// Wait for user input
	getch();

	// End curses mode
	endwin();
}

void setColourTheme(short baseRed, short baseGreen, short baseBlue) {
	float nMultiplier = 1000.0 / 255.0;
	short nRed = multiplyFloat(baseRed, nMultiplier),
		nGreen = multiplyFloat(baseGreen, nMultiplier),
		nBlue = multiplyFloat(baseBlue, nMultiplier);
	init_color(COLOUR_MAIN, nRed, nGreen, nBlue);
	// TODO: Conversion in a function
	init_color(COLOUR_MAIN_DARK,
			   multiplyFloat(nRed, (1 - colorContrast)),
			   multiplyFloat(nGreen, (1 - colorContrast)),
			   multiplyFloat(nBlue, (1 - colorContrast)));
	init_color(COLOUR_MAIN_LIGHT,
			   multiplyFloat(nRed, (1 + colorContrast)),
			   multiplyFloat(nGreen, (1 + colorContrast)),
			   multiplyFloat(nBlue, (1 + colorContrast)));
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

	drawLine(borderHorizon, corTL.x + 1, corTR.x - 1, corTL.y, corTR.y);
	drawLine(borderHorizon, corBL.x + 1, corBR.x - 1, corBL.y, corBR.y);
	drawLine(borderVertical, corTL.x, corBL.x, corTL.y + 1, corBL.y - 1);
	drawLine(borderVertical, corTR.x, corBR.x, corTR.y + 1, corBR.y - 1);
}

void drawPieces(const char *symbol, int offsetX, int offsetY) {
	int totalOffsetX = offsetX + pieceSpacing / 2;
	int totalOffsetY = offsetY;
	for (int i = 0; i < PIECE_PER_BOARD; i += 2) {
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

	for (int i = 0; i <= steps; ++i) {
		mvprintw((int) (round(y)), (int) (round(x)), "%s", symbol);
		x += xIncrement;
		y += yIncrement;
	}
}
