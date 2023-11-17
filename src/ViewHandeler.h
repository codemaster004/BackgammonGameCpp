//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_VIEWHANDELER_H
#define BACKGAMMONGAME_VIEWHANDELER_H

void setColourTheme(short baseRed = 0, short baseGreen = 166, short baseBlue = 0);

void drawLine(const char *symbol, int fromX, int toX, int fromY, int toY);

void drawPieces(int offsetX, int offsetY);

void drawPiece(const char *symbol, int offsetX, int offsetY);

void drawBorders(int offsetX, int offsetY, int width, int height);

void uiStaff();

#endif //BACKGAMMONGAME_VIEWHANDELER_H
