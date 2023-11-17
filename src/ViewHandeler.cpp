//
// Created by Filip Dabkowski on 12/11/2023.
//
#include <ncurses.h>
#include "cmath"
#include "string"

#include "configs/UIConfigs.h"
#include "ViewHandeler.h"

// TODO: Figure out how this works

const int boardMarginVertical = 5;
const int boardMarginHorizontal = 5;

enum UiColorsId {
	FOREGROUND=1,
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
	drawBorder(0, 2, 20, 10);


//	// Draw the upper part of the board
//	for (int i = 0; i < 12; i++) {
//		mvprintw(2, 4 + i * 4, "V");  // Change coordinates accordingly
//	}
//	// ... Continue for each row required to create the pawns look
//
//	// Draw the lower part of the board
//	for (int i = 0; i < 12; i++) {
//		mvprintw(10, 4 + i * 4, "^");  // Change coordinates accordingly
//	}
//	// ... Continue for each row required to create the pawns look

	// Draw the middle bar
//	attron(COLOR_PAIR(1));
//	mvprintw(6, 20, "[BAR]");
//	attroff(COLOR_PAIR(1));

	// Draw the score and other UI elements similarly using mvprintw

	// Refresh the screen to show changes
	refresh();

	// Wait for user input
	getch();

	// End curses mode
	endwin();
}

short multiplyFloat(short baseColor, float multiplier) {
	return (short) ((float) baseColor * multiplier);
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

void drawBorder(int offsetX, int offsetY, int width, int height) {
	mvaddch(offsetY, offsetX, BORDER_CORNER);
	mvaddch(offsetY, offsetX + width, BORDER_CORNER);
	mvaddch(offsetY + height, offsetX, BORDER_CORNER);
	mvaddch(offsetY + height, offsetX + width, BORDER_CORNER);

	drawLine(BORDER_HORIZON, offsetX + 1, offsetX + width - 1, offsetY, offsetY);
	drawLine(BORDER_HORIZON, offsetX + 1, offsetX + width - 1, offsetY + height, offsetY + height);
	drawLine(BORDER_VERTICAL, offsetX, offsetX, offsetY + 1, offsetY + height - 1);
	drawLine(BORDER_VERTICAL, offsetX + width, offsetX + width, offsetY + 1, offsetY + height - 1);
}

int max(int a, int b) {
	return a > b ? a : b;
}

void drawLine(char symbol, int fromX, int toX, int fromY, int toY) {
	int deltaX = toX - fromX;
	int deltaY = toY - fromY;

	int steps = max(abs(deltaX), abs(deltaY));

	float xIncrement = (float) (deltaX) / (float) (steps);
	float yIncrement = (float) (deltaY) / (float) (steps);

	auto x = (float) (fromX);
	auto y = (float) (fromY);

	for (int i = 0; i <= steps; ++i) {
		mvaddch((int) (round(y)), (int) (round(x)), symbol);
		x += xIncrement;
		y += yIncrement;
	}
}
