//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_VIEWHANDELER_H
#define BACKGAMMONGAME_VIEWHANDELER_H

#include "ViewModel.h"
#include "Utility.h"

void setColourTheme(short baseRed = 0, short baseGreen = 166, short baseBlue = 0);

void handleBoardOutline(Placement space);

void handlePieces(Placement space);

void handleBar();

void handleDices(Placement space, Pos center);

void drawLine(const char *symbol, Placement pos);

void drawCorners(Placement pos);

void drawBorders(Placement pos);

void drawPiece(const char *symbol, int offsetX, int offsetY);

void drawPieces(int offsetX, int offsetY);

void drawBar(int offsetX, int offsetY, int height);

void drawSpacedText(int minX, int maxX, int offsetY, int spacing, int len, char **text, int count);

void drawMenu(MenuElement *options, int length, int selected, int centerX, int offsetY);

void drawIndexes(char **indexes, MenuMode drawMode, int digits, Placement pos);

void uiStaff(UserInterface *ui, const int *menuSelected, int *dice1, int *dice2);

#endif //BACKGAMMONGAME_VIEWHANDELER_H
