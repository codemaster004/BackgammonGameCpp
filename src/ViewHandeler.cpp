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
#include "ViewModel.h"

// TODO: Figure out how this works

// TODO: Implement UI generating
void uiStaff(const int *menuSelected, int *dice1, int *dice2) {

	auto *testMenu = new MenuElement[N_MENU_OPTIONS];
	for (int i = 0; i < N_MENU_OPTIONS; ++i) {
		testMenu[i] = MenuElement{.id=i, .value=menuOptions[i]};
	}

	// TODO: Separete functionS
	// TODO: add CORDS
	attron(COLOR_PAIR(FOREGROUND));
	int boardWidth = POINTS_PER_BOARD * pieceWidth + pieceSpacing * (POINTS_PER_BOARD - 1) + pieceSpacing / 2 * 2;
	int boardHeight = PAWNS_PER_POINT * 2 + pieceSpacing;

	int borders = BORDER_WIDTH * 2;
	int endOfBoard = boardOffsetX;
	for (int _ = 0; _ < N_BOARDS; ++_) {
		drawBorders(endOfBoard, boardOffsetY,
					boardWidth + borders, boardHeight + borders);
		drawPieces(endOfBoard + BORDER_WIDTH, boardOffsetY + BORDER_WIDTH);
		endOfBoard += (boardWidth + borders + 1);
	}
	for (int i = N_BOARDS - 1; i > 0; --i)
		drawBar(boardOffsetX + (boardWidth + borders) * i, boardOffsetY, boardHeight + borders);

	// TODO: Dies
	drawLine(borderHorizon, endOfBoard - 1, endOfBoard + DICE_WIDTH - 1, boardOffsetY, boardOffsetY);
	drawLine(borderHorizon, endOfBoard - 1, endOfBoard + DICE_WIDTH - 1, boardOffsetY + boardHeight + 1,
			 boardOffsetY + boardHeight + 1);
	// middle one
	drawLine(borderHorizon, endOfBoard - 1, endOfBoard + DICE_WIDTH - 1,
			 boardOffsetY + boardHeight / 2 - 2 + 1, boardOffsetY + boardHeight / 2 - 2 + 1);
	drawLine(borderHorizon, endOfBoard - 1, endOfBoard + DICE_WIDTH - 1,
			 boardOffsetY + boardHeight / 2 + 1, boardOffsetY + boardHeight / 2 + 1);
	drawLine(borderHorizon, endOfBoard - 1, endOfBoard + DICE_WIDTH - 1,
			 boardOffsetY + boardHeight / 2 + DICE_WIDTH, boardOffsetY + boardHeight / 2 + DICE_WIDTH);
	// vertical
	drawLine(borderVertical, endOfBoard + DICE_WIDTH - 1, endOfBoard + DICE_WIDTH - 1, boardOffsetY, boardOffsetY + boardHeight + 1);

	mvprintw(boardOffsetY, endOfBoard + DICE_WIDTH - 1, borderCorner);
	mvprintw(boardOffsetY + boardHeight + 1, endOfBoard + DICE_WIDTH - 1, borderCorner);

	// center +
	mvprintw(boardOffsetY + boardHeight / 2 + 1 - 2, endOfBoard + DICE_WIDTH - 1, borderCorner);
	mvprintw(boardOffsetY + boardHeight / 2 + 1 - 2, endOfBoard - 2, borderCorner);
	mvprintw(boardOffsetY + boardHeight / 2 + 1, endOfBoard + DICE_WIDTH - 1, borderCorner);
	mvprintw(boardOffsetY + boardHeight / 2 + 1, endOfBoard - 2, borderCorner);
	mvprintw(boardOffsetY + boardHeight / 2 + 1 + 2, endOfBoard + DICE_WIDTH - 1, borderCorner);
	mvprintw(boardOffsetY + boardHeight / 2 + 1 + 2, endOfBoard - 2, borderCorner);
	mvaddch(boardOffsetY + boardHeight / 2 + 1 + 1, endOfBoard + DICE_WIDTH / 2 - 1, *dice1 + 48);
	mvaddch(boardOffsetY + boardHeight / 2, endOfBoard + DICE_WIDTH / 2 - 1, *dice2 + 48);
	attroff(COLOR_PAIR(FOREGROUND));

	drawMenu(testMenu, N_MENU_OPTIONS, *menuSelected, boardOffsetX + (endOfBoard - boardOffsetX) / 2,
			 boardOffsetY + boardHeight + MENU_TOP_SPACING);


	move(0, 0);
	// Refresh the screen to show changes
	refresh();

	/// CLEAR MEMORY!!!
	free(testMenu);
}

void setColourTheme(short baseRed, short baseGreen, short baseBlue) {
//	init_color(COLOUR_MAIN, 0, 700, 0);
//	init_color(COLOUR_MAIN_DARK, 0, 500, 0);
//	init_color(COLOUR_MAIN_LIGHT, 0, 1000, 0);
//	return;


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

	drawLine(borderHorizon, corTL.x + 1, corTR.x - 1, corTL.y, corTR.y);
	drawLine(borderHorizon, corBL.x + 1, corBR.x - 1, corBL.y, corBR.y);
	drawLine(borderVertical, corTL.x, corBL.x, corTL.y + 1, corBL.y - 1);
	drawLine(borderVertical, corTR.x, corBR.x, corTR.y + 1, corBR.y - 1);
}

void drawPiece(const char *symbol, int offsetX, int offsetY) {
	int totalOffsetX = offsetX + pieceSpacing / 2;
	int totalOffsetY = offsetY;
	for (int i = 0; i < PAWNS_PER_POINT; i += 2) {
		drawLine(symbol, totalOffsetX, totalOffsetX, totalOffsetY, totalOffsetY + PAWNS_PER_POINT - 1);
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

	// TODO: change <= to <
	for (int i = 0; i <= steps; ++i) {
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

void drawSpacedText(int minX, int maxX, int offsetY, const int spacing, const int count, const int len, char **text) {

	int center = (maxX - minX) / 2;
	int textWidth = count * (len - 1) + spacing * (count - 1);
	int startPoint = minX + center - textWidth / 2;
	if (startPoint < minX)
		return;

	int totalOffset = startPoint;
//	for (const char *text: elements) {
//		mvprintw(offsetY, totalOffset, "%s", text);
//		totalOffset += maxLen + spacing;
//	}
}

// TODO: DIVIDE
void drawMenu(MenuElement *options, int length, int selected, int centerX, int offsetY) {
	uint menuRealLen = OPTION_SPACING * (length - 1);
	for (int i = 0; i < length; ++i) {
		menuRealLen += len(options[i].value);
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
		startingPoint += (int) (len(options[i].value)) + OPTION_SPACING;
	}
}
