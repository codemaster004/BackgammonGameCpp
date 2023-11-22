//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_HANDELER_H
#define BACKGAMMONGAME_HANDELER_H

#include "../viewModel/UserInterface.h"
#include "../Utility.h"

void setColourTheme(short baseRed = 0, short baseGreen = 166, short baseBlue = 0);

void handleBoardOutline(Placement space);

void handlePieces(Placement space);

void handleBar();

void handleDices(Placement space, Pos center);

void drawBar(int offsetX, int offsetY, int height);

void handleMenu(MenuElement *options, int length, int selected, int centerX, int offsetY);

void handleIndexes(char **indexes, MenuMode drawMode, int digits, Placement pos);

void uiStaff(UserInterface *ui, const int *menuSelected, int *dice1, int *dice2);

#endif //BACKGAMMONGAME_HANDELER_H
