//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_HANDELER_H
#define BACKGAMMONGAME_HANDELER_H

#include "../viewModel/UserInterface.h"
#include "../Utility.h"

void handleBoardOutline(Placement space);

void handlePieces(Placement space);

void handleBar(Bar bar, int selected);

void handleDices(Placement space, Pos center, int *dices);

void handleMenu(Menu menu, Pos center);

void handleIndexes(char **indexes, int pickedIndex, int digits, Placement posTop, Placement posBot);

void handleMessages(UserInterface &ui);

void generateBasicBoard(UserInterface &ui);

void generateInteractiveUI(UserInterface &ui);

void handlePawnPlacement(Board &game, Placement space);

#endif //BACKGAMMONGAME_HANDELER_H
