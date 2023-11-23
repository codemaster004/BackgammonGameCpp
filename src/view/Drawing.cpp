//
// Created by Filip Dabkowski on 22/11/2023.
//

#include <ncurses.h>
#include "cmath"

#include "../configs/UIConfigs.h"
#include "Drawing.h"

void printColor(UiColorsId color, int x, int y, const char *text) {
	attron(COLOR_PAIR(color));
	mvprintw(y, x, "%s", text);
	attroff(COLOR_PAIR(color));
}

void drawLine(const char *symbol, Placement pos) {
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

void drawCorners(Placement pos) {
	mvprintw(pos.min.y, pos.min.x, borderCorner);
	mvprintw(pos.min.y, pos.max.x, borderCorner);
	mvprintw(pos.max.y, pos.min.x, borderCorner);
	mvprintw(pos.max.y, pos.max.x, borderCorner);
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
	for (int i = 0; i < PAWNS_PER_POINT; i += 2) {
		Placement offset = {
			.min = Pos{.x = totalOffset, .y = offsetY},
			.max = Pos{.x = totalOffset, .y = offsetY + PAWNS_PER_POINT}
		};
		// placement
		drawLine(symbol, offset);
		totalOffset += (pieceWidth + pieceSpacing) * 2;
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

void drawSpacedText(Placement pos, int spacing, int len, char **text, int count) {
	int center = pos.min.x + (pos.max.x - pos.min.x) / 2;
	int textWidth = count * (len) + spacing * (count - 1);
	int startPoint = center - textWidth / 2;
	if (startPoint < pos.min.x)
		return;

	for (int i = 0; i < count; ++i) {
		mvprintw(pos.min.y, startPoint, "%s", text[i]);
		startPoint += len + spacing;
	}
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
