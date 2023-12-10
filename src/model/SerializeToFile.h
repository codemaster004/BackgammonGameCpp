//
// Created by Filip Dabkowski on 24/11/2023.
//

#ifndef BACKGAMMONGAME_SERIALIZETOFILE_H
#define BACKGAMMONGAME_SERIALIZETOFILE_H

#include "Board.h"
#include "../viewModel/UserInterface.h"

void serializeInt(int value, uint8_t *buffer, size_t &offset);

int deserializeInt(const uint8_t *buffer, size_t &index);

void saveToFile(char filename[], Board &game);

void loadFromFile(char filename[], Board &game);

void saveScores(char filename[], ScorePlayer scores[]);

void loadScores(char filename[], UserInterface &ui);

#endif //BACKGAMMONGAME_SERIALIZETOFILE_H
