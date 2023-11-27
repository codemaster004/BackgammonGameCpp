//
// Created by Filip Dabkowski on 24/11/2023.
//

#ifndef BACKGAMMONGAME_SERIALIZETOFILE_H
#define BACKGAMMONGAME_SERIALIZETOFILE_H

#include "Board.h"

void serializeInt(int value, uint8_t *buffer, size_t &offset);

int deserializeInt(const uint8_t *buffer, size_t &index);

void serializeToFile(char filename[], Board &game);

Board loadFromFile(char filename[]);

#endif //BACKGAMMONGAME_SERIALIZETOFILE_H