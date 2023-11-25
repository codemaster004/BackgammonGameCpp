//
// Created by Filip Dabkowski on 24/11/2023.
//

#include <cstdio>
#include "cstring"
#include "cstdint"

#include "../configs/GameConfigs.h"
#include "SerializeToFile.h"

void serializeInt(int value, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &value, sizeof(int));
	offset += sizeof(int);
}

int deserializeInt(const uint8_t *buffer, size_t &index) {
	int value;
	std::memcpy(&value, buffer + index, sizeof(int));
	index += sizeof(int);
	return value;
}

int serializeToFile(char *filename) {


	return 0;
}