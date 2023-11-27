//
// Created by Filip Dabkowski on 24/11/2023.
//

#include <cstdio>
#include "cstring"
#include "cstdint"

#include "../configs/GameConfigs.h"
#include "SerializeToFile.h"
#include "../Base64.h"
#include "Board.h"

#define PLAYERS_SIZE sizeof(Player) * N_PLAYERS
#define PAWNS_SIZE sizeof(Pawn) * totalPawns
#define INT_SIZE sizeof(int)
#define POINT_SIZE (INT_SIZE * PAWNS_PER_POINT + INT_SIZE)
#define COURT_SIZE (INT_SIZE * PAWNS_PER_PLAYER + INT_SIZE * 2)
#define BAR_SIZE (INT_SIZE * totalPawns + INT_SIZE)
#define DICES_SIZE INT_SIZE * N_DICES

const int boardSize = PLAYERS_SIZE + PAWNS_SIZE + INT_SIZE + DICES_SIZE +
					  POINT_SIZE * nPoints + COURT_SIZE * N_PLAYERS + BAR_SIZE;

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

void serializeToFile(char filename[], Board &game) {
	auto *bufferTable = new uint8_t[boardSize];

	size_t index = 0;
	serialiseBoard(game, bufferTable, index);
	char *encodedFile = encodeBase64(bufferTable, index);
	delete[] bufferTable;

	char *path = joinStrings((char *)(savesDir), sizeof(savesDir) - 1, filename, 9);
	FILE *file = fopen(path, "w");
	delete[] path;

	int encodedLen = finalEncodedDataLen((int) (index));
	if (file != nullptr) {
		for (int i = 0; i < encodedLen; ++i) {
			fputc(encodedFile[i], file);
		}

		fclose(file);
	}

	delete[] encodedFile;
}

Board loadFromFile(char filename[]) {
	int encodedLen = finalEncodedDataLen((int) (boardSize));
	auto *bufferTable = new char[encodedLen];
	size_t index = 0;

	char *path = joinStrings((char *)(savesDir), sizeof(savesDir) - 1, filename, 9);
	FILE *file = fopen(path, "w");
	delete[] path;

	int ch;
	while ((ch = fgetc(file)) != EOF) {
		bufferTable[index++] = (char) (ch);
	}
	if (index != encodedLen) {
		// TODO: Generate some error for the user to see
	}
	uint8_t *decodedBoard = decodeBase64(bufferTable, encodedLen);
	delete[] bufferTable;

	index = 0;
	Board savedGame = deserializeBoard(decodedBoard, index);

	delete[] decodedBoard;

	return savedGame;
}
