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

typedef struct {
	Pos min;
	Pos max;
} Placement;

char* reverseString(const char* str);

short multiply(short baseColor, float multiplier);

short capAt(short value, short max);

int max(int a, int b);

uint len(const char *text);

uint nDigits(int n, int base);

uint maxInBase(int n, int base);

char* numberToString(int number, int width);

void revertTable(char **from, char **to);

#endif //BACKGAMMONGAME_UTILITY_H
