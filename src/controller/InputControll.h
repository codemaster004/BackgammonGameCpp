//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_INPUTCONTROLL_H
#define BACKGAMMONGAME_INPUTCONTROLL_H

#include "../model/Board.h"

void gameSetUp(Board &game);

void inputController(int input, Board &game, int &menuSelected, bool &gameEnded, int &inputtedNumber);

void movePawn(Board &game, int fromIndex, int moveBy);

#endif //BACKGAMMONGAME_INPUTCONTROLL_H
