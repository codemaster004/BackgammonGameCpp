//
// Created by Filip Dabkowski on 17/11/2023.
//

#include "algorithm"

#include "Utility.h"

char *reverseString(const char *str) {
	unsigned long length = std::strlen(str);
	char *reversed = new char[length + 1];

	std::copy(str, str + length + 1, reversed);
	std::reverse(reversed, reversed + length);

	return reversed;
}

short multiply(short baseColor, float multiplier) {
	auto res = (short) ((float) baseColor * multiplier);
	return res;
}

short capAt(short value, short max) {
	return value > max ? max : value;
}

int max(int a, int b) {
	return a > b ? a : b;
}

uint len(const char *text) {
	uint length = 0;
	while (true)
		if (text[length++] == 0)
			break;
	return length;
}

uint nDigits(int n, int base) {
	uint count = 1;
	uint i = 1;
	while (n / count) {
		count *= base;
		i++;
	}
	return --i;
}

uint maxInBase(int n, int base) {
	uint count = 1;
	while (n / count)
		count *= base;
	return count / base;
}

char *numberToString(int number, int width) {
	char *str = new char[width + 1];
	str[width] = '\0';

	for (int i = width - 1; i >= 0; --i) {
		str[i] = (char) ('0' + (number % 10));
		number /= 10;
	}

	return str;
}

void revertTable(char **from, char **to) {
	for (int i = 0; i < (to - from) / 2; ++i) {
		char *temp = from[i];
		from[i] = *(to - i - 1);
		*(to - i - 1) = temp;
	}
}
