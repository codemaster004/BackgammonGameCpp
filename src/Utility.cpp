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

int min(int a, int b) {
	return a < b ? a : b;
}

uint len(const char *text) {
	uint length = 0;
	while (true)
		if (text[length++] == 0)
			break;
	return length;
}

uint nDigits(int n, int base) {
	if (!n) return 1;

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

char *numberToString(int number, uint width) {
	char *str = new char[width + 1];
	str[0] = '0'; // Default value
	str[width] = '\0';

	for (int i = width - 1; i >= 0; --i) {
		str[i] = (char) ('0' + (number % 10));
		number /= 10;
	}

	return str;
}

int stringToNumber(const char *str) {
	int result = 0;
	int i = 0;
	while (str[i] != '\0') {
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	return result;
}

void revertTable(char **from, char **to) {
	for (int i = 0; i < (to - from) / 2; ++i) {
		char *temp = from[i];
		from[i] = *(to - i - 1);
		*(to - i - 1) = temp;
	}
}

void joinStrings0(char *&buffer, const char **data, int count) {
	int index = 0;
	for (int i = 0; i < count; ++i) {

		uint length = len(data[i]);
		for (int j = 0; j < length - 1; ++j)
			buffer[index++] = data[i][j];

	}
}

char *joinStrings1(const char *string1, uint len1, const char *string2, uint len2) {
	char *fullString = new char [len1 + len2];
	for (int i = 0; i < len1; ++i) {
		fullString[i] = string1[i];
	}
	for (int i = 0; i < len2; ++i) {
		fullString[len1 + i] = string2[i];
	}
	return fullString;
}

char *joinStrings2(const char*string1, const char *string2) {
	uint len1 = len(string1);
	uint len2 = len(string2);
	return joinStrings1(string1, (int) (len1) - 1, string2, (int) (len2) - 1);
}

void initTable(int *&table, int count, int value) {
	for (int i = 0; i < count; ++i) {
		table[i] = value;
	}
}

void resizeTable(int *&table, int &size, int increase) {
	int newSize = size + increase;
	// Create a new array with size increased by 1
	int *newArray = new int[newSize];

	// Copy the elements from the old array to the new array
	for (int i = 0; i < size; ++i) {
		newArray[i] = table[i];
	}

	// Initialize the new element
	for (int i = size; i < newSize; ++i) {
		newArray[i] = 0;
	}

	// Delete the old array
	delete[] table;

	// Update the original array pointer and size
	table = newArray;
	size = newSize;
}
