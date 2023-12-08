//
// Created by Filip Dabkowski on 22/11/2023.
//

#include <ncurses.h>
#include "cmath"

#include "../configs/UIConfigs.h"
#include "../model/Board.h"
#include "Drawing.h"

void printColor(UiColorsId color, int x, int y, const char *text) {
	attron(COLOR_PAIR(color));
	mvprintw(y, x, "%s", text);
	attroff(COLOR_PAIR(color));
}

void printColor(UiColorsId color, int x, int y, char text) {
	attron(COLOR_PAIR(color));
	mvprintw(y, x, "%c", text);
	attroff(COLOR_PAIR(color));
}

void drawLine(char symbol, Placement pos) {
	int deltaX = pos.max.x - pos.min.x;
	int deltaY = pos.max.y - pos.min.y;

	int steps = max(abs(deltaX), abs(deltaY));

	float xIncrement = (float) (deltaX) / (float) (steps);
	float yIncrement = (float) (deltaY) / (float) (steps);

	auto x = (float) (pos.min.x);
	auto y = (float) (pos.min.y);

	for (int i = 0; i < steps; ++i) {
		mvprintw((int) (y), (int) (x), "%c", symbol);
		x += xIncrement;
		y += yIncrement;
	}
}

void drawLineString(const char symbol[], Placement pos) {
	int deltaX = pos.max.x - pos.min.x;
	int deltaY = pos.max.y - pos.min.y;

	int steps = max(abs(deltaX), abs(deltaY));

	float xIncrement = (float) (deltaX) / (float) (steps);
	float yIncrement = (float) (deltaY) / (float) (steps);

	auto x = (float) (pos.min.x);
	auto y = (float) (pos.min.y);

	for (int i = 0; i < steps; ++i) {
		mvprintw((int) (y), (int) (x), "%s", symbol);
		x += xIncrement;
		y += yIncrement;
	}
}

void drawLinePawn(Pawn pawn, Placement pos) {
	const char pawn1[] = { ")(" };
	const char pawn2[] = { "[]" };

	for (int y = pos.min.y; y < pos.max.y; y++) {
		if (pawn.color == PAWN_WHITE) {
			printColor(BACKGROUND_LIGHT, pos.min.x, y, pawn1);
		} else if (pawn.color == PAWN_BLACK) {
			printColor(BACKGROUND_DARK, pos.min.x, y, pawn2);
		}
	}
}

void drawCorners(Placement pos) {
	mvprintw(pos.min.y, pos.min.x, "%c", borderCorner);
	mvprintw(pos.min.y, pos.max.x, "%c", borderCorner);
	mvprintw(pos.max.y, pos.min.x, "%c", borderCorner);
	mvprintw(pos.max.y, pos.max.x, "%c", borderCorner);
}

void drawBorders(Placement pos) {
	drawLine(borderHorizon, Placement{pos.min.x, pos.min.y, pos.max.x, pos.min.y});
	drawLine(borderHorizon, Placement{pos.min.x, pos.max.y, pos.max.x, pos.max.y});
	drawLine(borderVertical, Placement{pos.min.x, pos.min.y, pos.min.x, pos.max.y});
	drawLine(borderVertical, Placement{pos.max.x, pos.min.y, pos.max.x, pos.max.y});
	drawCorners(pos);
}

void drawPiece(const char *symbol, int offsetX, int offsetY) {
	int totalOffset = offsetX + pieceSpacing / 2;
	for (int i = 0; i < POINTS_PER_BOARD; i += 2) {
		Placement offset = {
			.min = Pos{.x = totalOffset, .y = offsetY},
			.max = Pos{.x = totalOffset, .y = offsetY + POINT_HEIGHT}
		};

		drawLineString(symbol, offset);
		totalOffset += (pieceWidth + pieceSpacing) * 2;
	}
}

void drawPieces(int offsetX, int offsetY) {
	const char piece1[] = { "/\\" };
	const char piece2[] = { "''" };

	char *reversePiece = reverseString(piece1);
	drawPiece(reversePiece, offsetX, offsetY);

	reversePiece = reverseString(piece2);
	drawPiece(reversePiece, offsetX + pieceWidth + pieceSpacing, offsetY);

	drawPiece(piece1, offsetX + pieceWidth + pieceSpacing,
			  offsetY + POINT_HEIGHT + pieceSpacing);
	drawPiece(piece2, offsetX, offsetY + POINT_HEIGHT + pieceSpacing);
}

void drawCenteredText(Placement pos, int spacing, int len, char **text, int count, UiColorsId *colors, int nColors) {
	int center = pos.min.x + (pos.max.x - pos.min.x) / 2;
	int textWidth = count * (len) + spacing * (count - 1);
	int startPoint = center - textWidth / 2;
	if (startPoint < pos.min.x)
		return;

	for (int i = 0; i < count; ++i) {
		printColor(colors[i % nColors], startPoint, pos.min.y, text[i]);
		startPoint += len + spacing;
	}
}

void drawSpreadText(Placement pos, char **text, int count, int selected) {
	int textSpace = (pos.max.x - pos.min.x) / count;
	Placement tempPos = pos;
	tempPos.max.x = tempPos.min.x + textSpace;

	auto *colors = new UiColorsId [count];
	for (int i = 0; i < count; ++i) {
		if (selected < 0) {
			colors[i] = FOREGROUND;
		} else {
			colors[i] = i == selected ? FOREGROUND_LIGHT : FOREGROUND_DARK;
		}
	}

	for (int i = 0; i < count; ++i) {
		uint textLen = len(text[i]);
		drawCenteredText(tempPos, 0, (int) (textLen), text + i, 1, &colors[i], 1);
		moveSpace(tempPos, Pos{textSpace});
	}
	delete[] colors;
}

void setColor(short colorName, short nRed, short nGreen, short nBlue, float diff) {
	init_color(colorName,
			   capAt(multiply(nRed, diff), 1000),
			   capAt(multiply(nGreen, diff), 1000),
			   capAt(multiply(nBlue, diff), 1000));
}

void setColourTheme(short baseRed, short baseGreen, short baseBlue) {
	float nMultiplier = 1000.0 / 255.0;
	short nRed = capAt(multiply(baseRed, nMultiplier), 1000),
		nGreen = capAt(multiply(baseGreen, nMultiplier), 1000),
		nBlue = capAt(multiply(baseBlue, nMultiplier), 1000);
	setColor(COLOUR_MAIN, nRed, nGreen, nBlue, 1);
	setColor(COLOUR_MAIN_DARK, nRed, nGreen, nBlue, (1 - colorDiff));
	setColor(COLOUR_MAIN_LIGHT, nRed, nGreen, nBlue, (1 + colorDiff));
}

void drawVertically(Pos pos, const char *text, UiColorsId color) {
	int length = (int)(len(text));
	for (int i = 0; i < length; ++i) {
		printColor(color, pos.x, pos.y + i, text[i]);
	}
}

void drawVerticalInfo(Pos pos, const char *label, int value, UiColorsId color, bool force) {
	if (value > 0 || force) {
		char *text = joinStrings(label, numberToString(value, 2));
		drawVertically(pos, text, color);
		delete[] text;
	}
}

// TODO: SHORTEN
void drawBar(int offsetX, int offsetY, int height, int onBar[2], int selected) {
	const char barLabel[] = {"[BAR]"};

	attron(COLOR_PAIR(FOREGROUND));
	mvprintw(offsetY, offsetX, "%c", borderCorner);
	drawLine(borderVertical, Placement{offsetX, offsetY + 1,
									   offsetX, offsetY + height - 1});
	mvprintw(offsetY + height - 1, offsetX, "%c", borderCorner);

	drawVerticalInfo({offsetX, offsetY + BORDER_WIDTH}, "WHT ", onBar[0]);
	drawVerticalInfo({offsetX, offsetY + (height) / 2 + BORDER_WIDTH + 1}, "BLC ", onBar[1]);

	UiColorsId color = FOREGROUND;
	if (onBar[0] || onBar[1] && selected >= 0) {
		color = selected == nPoints ? FOREGROUND_LIGHT : FOREGROUND_DARK;
	}
	printColor(color, offsetX - (int) (sizeof(barLabel)) / 2 + 1, offsetY + (height) / 2, barLabel);
}
