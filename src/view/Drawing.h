//
// Created by Filip Dabkowski on 22/11/2023.
//

#ifndef BACKGAMMONGAME_DRAWING_H
#define BACKGAMMONGAME_DRAWING_H

#include "../configs/UIConfigs.h"

void printColor(UiColorsId color, int x, int y, const char *text);

void drawLine(const char *symbol, Placement pos);

void drawCorners(Placement pos);

void drawBorders(Placement pos);

void drawPiece(const char *symbol, int offsetX, int offsetY);

void drawPieces(int offsetX, int offsetY);

void drawCenteredText(Placement pos, int spacing, int len, char **text, int count, UiColorsId *colors, int nColors);

void drawSpreadText(Placement pos, char **text, int count);

void drawLine(Pawn pawn, Placement pos);

void setColourTheme(short baseRed = 0, short baseGreen = 166, short baseBlue = 0);

#endif //BACKGAMMONGAME_DRAWING_H
