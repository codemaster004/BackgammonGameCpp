//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_CONTROLLER_H
#define BACKGAMMONGAME_CONTROLLER_H

#include "model/Board.h"

void handleGame(Board *game);

void inputController(int input, Board *game, int *menu, bool *gameEnded, int *dice1, int *dice2, int *inputtedNumber);

void movePawn(Board *game, int fromIndex, int moveBy);

#endif //BACKGAMMONGAME_CONTROLLER_H
