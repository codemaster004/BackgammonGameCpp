//
// Created by Filip Dabkowski on 17/11/2023.
//

#include "algorithm"

char* reverseString(const char* str) {
	unsigned long length = std::strlen(str);
	char* reversed = new char[length + 1];

	std::copy(str, str + length + 1, reversed);
	std::reverse(reversed, reversed + length);

	return reversed;
}

short multiplyFloat(short baseColor, float multiplier, short max) {
	short res = (short) ((float) baseColor * multiplier);
	return res > max ? max : res;
}

int max(int a, int b) {
	return a > b ? a : b;
}
