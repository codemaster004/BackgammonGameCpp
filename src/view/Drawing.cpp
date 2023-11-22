//
// Created by Filip Dabkowski on 22/11/2023.
//

#include <ncurses.h>
#include "cmath"

#include "../configs/UIConfigs.h"
#include "Drawing.h"

void printColor(UiColorsId color, int x, int y, const char *text) {
	attron(COLOR_PAIR(FOREGROUND_LIGHT));
	mvprintw(y, x, "%s", text);
	attroff(COLOR_PAIR(FOREGROUND_LIGHT));
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

void drawSpacedText(int minX, int maxX, int offsetY, int spacing, int len, char **text, int count) {
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
