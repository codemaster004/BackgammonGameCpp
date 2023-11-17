//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_VIEWHANDELER_H
#define BACKGAMMONGAME_VIEWHANDELER_H

void setColourTheme(short baseRed = 0, short baseGreen = 166, short baseBlue = 0);

void drawLine(char symbol, int fromX, int toX, int fromY, int toY);

void drawBorder(int offsetX, int offsetY, int width, int height);

void uiStaff();

#endif //BACKGAMMONGAME_VIEWHANDELER_H
