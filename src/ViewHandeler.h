//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_VIEWHANDELER_H
#define BACKGAMMONGAME_VIEWHANDELER_H

#include "ViewModel.h"

void setColourTheme(short baseRed = 0, short baseGreen = 166, short baseBlue = 0);

void drawLine(const char *symbol, int fromX, int toX, int fromY, int toY);

void drawPieces(int offsetX, int offsetY);

void drawPiece(const char *symbol, int offsetX, int offsetY);

void drawBar(int offsetX, int offsetY, int height);

void drawBorders(int offsetX, int offsetY, int width, int height);

void drawSpacedText(int minX, int maxX, int offsetY, const int spacing, const int len, char **text, const int count);

void drawMenu(MenuElement *options, int length, int selected, int centerX, int offsetY);

void uiStaff(const int *menuSelected, int *dice1, int *dice2);

#endif //BACKGAMMONGAME_VIEWHANDELER_H
