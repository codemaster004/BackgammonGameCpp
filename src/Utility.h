//
// Created by Filip Dabkowski on 17/11/2023.
//

#ifndef BACKGAMMONGAME_UTILITY_H
#define BACKGAMMONGAME_UTILITY_H

typedef unsigned int uint;

char* reverseString(const char* str);

short multiply(short baseColor, float multiplier);

short capAt(short value, short max);

int max(int a, int b);

int min(int a, int b);

uint len(const char *text);

uint nDigits(int n, int base);

uint maxInBase(int n, int base);

char* numberToString(int number, uint width);

int stringToNumber(const char *str);

void revertTable(char **from, char **to);

void joinStrings0(char *&buffer, const char **data, int count);

char *joinStrings1(const char *string1, uint len1, const char *string2, uint len2);

char *joinStrings2(const char*string1, const char *string2);

void initTable(int *&table, int count, int value);

void resizeTable(int *&table, int &size, int increase=1);

#endif //BACKGAMMONGAME_UTILITY_H
