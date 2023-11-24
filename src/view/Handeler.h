//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_HANDELER_H
#define BACKGAMMONGAME_HANDELER_H

#include "../viewModel/UserInterface.h"
#include "../Utility.h"

void handleBoardOutline(Placement space);

void handlePieces(Placement space);

void handleBar();

void handleDices(Placement space, Pos center);

void drawBar(int offsetX, int offsetY, int height);

void handleMenu(MenuElement *options, int optionCount, int selected, Pos center);

void handleIndexes(char **indexes, int digits, Placement pos1, Placement pos2);

void generateBasicBoard(UserInterface *ui, const int *menuSelected, int *dice1, int *dice2);

void generateInteractiveUI(UserInterface *ui, const int *menuSelected);

void handlePawnPlacement(Board *game, Placement space);

#endif //BACKGAMMONGAME_HANDELER_H
