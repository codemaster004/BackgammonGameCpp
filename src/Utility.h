//
// Created by Filip Dabkowski on 17/11/2023.
//

#ifndef BACKGAMMONGAME_UTILITY_H
#define BACKGAMMONGAME_UTILITY_H

typedef unsigned int uint;

typedef struct {
	int x;
	int y;
} Pos;

char* reverseString(const char* str);

short multiplyFloat(short baseColor, float multiplier, short max);

int max(int a, int b);

uint len(const char *text);

#endif //BACKGAMMONGAME_UTILITY_H
