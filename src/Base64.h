//
// Created by Filip Dabkowski on 26/11/2023.
//

#ifndef BACKGAMMONGAME_BASE64_H
#define BACKGAMMONGAME_BASE64_H

#include "cstring"
#include "cstdint"

const int baseSize = 64;
const char base64table[baseSize] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
	'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

int createBitMask(int numberOfUpBits, int rightOffset);

int indexOf(const char *searchTable, char searchElement, int maxElement);

int finalEncodedDataLen(int len);

int finalDecodedDataLen(int len);

int toNumber(const uint8_t *byteBlock, int blockSize = 3);

void toChar(uint8_t *data, int &offset, int block, int padding);

void toBase64(int block, char *finalData, int &offset, int padding);

int fromBase64(const char *base, int &padding);

uint8_t *decodeBase64(const char *base64String, size_t length);

char *encodeBase64(uint8_t *data, size_t length);

#endif //BACKGAMMONGAME_BASE64_H
