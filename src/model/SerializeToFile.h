//
// Created by Filip Dabkowski on 24/11/2023.
//

#ifndef BACKGAMMONGAME_SERIALIZETOFILE_H
#define BACKGAMMONGAME_SERIALIZETOFILE_H

void serializeInt(int value, uint8_t *buffer, size_t &offset);
int deserializeInt(const uint8_t *buffer, size_t &index);

int serializeToFile(char *filename);

#endif //BACKGAMMONGAME_SERIALIZETOFILE_H
