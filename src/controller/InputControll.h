//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_INPUTCONTROLL_H
#define BACKGAMMONGAME_INPUTCONTROLL_H

#include "../model/Board.h"
#include "../viewModel/UserInterface.h"

void gameSetUp(Board &game);

void resetMenuTo(UserInterface &ui, MenuMode mode);

void newGameController(int input, UserInterface &ui);

void gamePlayController(int input, UserInterface &ui);

void pickDiceController(int input, UserInterface &ui);

void pickPointController(int input, UserInterface &ui);

void inputController(int input, UserInterface &ui);

int movePawn(Board &game, Move move);

#endif //BACKGAMMONGAME_INPUTCONTROLL_H
