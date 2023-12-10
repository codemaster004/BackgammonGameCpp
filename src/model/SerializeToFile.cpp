//
// Created by Filip Dabkowski on 24/11/2023.
//

#include "cstdio"
#include "cstring"

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
					  POINT_SIZE * nPoints + COURT_SIZE * N_PLAYERS + BAR_SIZE + INT_SIZE;

// PRIVATE FUNCTION HEADERS //

// PUBLIC FUNCTION DECLARATIONS //

// PRIVATE FUNCTION DECLARATIONS //

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

void saveToFile(char filename[], Board &game) {
	const char savesDir[] = "../saves/games/";
	auto *bufferTable = new uint8_t[boardSize];

	size_t index = 0;
	serialiseBoard(game, bufferTable, index);
	char *encodedFile = encodeBase64(bufferTable, index);
	delete[] bufferTable;

	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);
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

void loadFromFile(char filename[], Board &game) {
	const char savesDir[] = "../saves/games/";
	int encodedLen = finalEncodedDataLen((int) (boardSize));
	auto *bufferTable = new char[encodedLen];
	size_t index = 0;

	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);
	FILE *file = fopen(path, "r");
	delete[] path;

	int ch;
	while ((ch = fgetc(file)) != EOF) {
		bufferTable[index++] = (char) (ch);
	}
	fclose(file);

	if (index != encodedLen) {
		// TODO: Generate some error for the user to see
	}
	uint8_t *decodedBoard = decodeBase64(bufferTable, encodedLen);
	delete[] bufferTable;

	index = 0;
	deserializeBoard(decodedBoard, index, game);

	delete[] decodedBoard;
}

void saveScores(char filename[], ScorePlayer scores[]) {
	const char savesDir[] = "../saves/";
	auto *buffer = new uint8_t[N_ALL_PLAYERS * PLAYERS_SIZE];

	size_t index = 0;
	for (int i = 0; i < N_ALL_PLAYERS; ++i) {
		serializeScorePlayer(scores[i], buffer, index);
	}
	char *encodedFile = encodeBase64(buffer, index);
	delete[] buffer;

	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);
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

void loadScores(char filename[], UserInterface &ui) {
	const char savesDir[] = "../saves/";
	int encodedLen = finalEncodedDataLen(N_ALL_PLAYERS * PLAYERS_SIZE);
	auto *buffer = new char[encodedLen];
	size_t index = 0;

	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);
	FILE *file = fopen(path, "r");
	delete[] path;

	int ch;
	while ((ch = fgetc(file)) != EOF) {
		buffer[index++] = (char) (ch);
	}
	fclose(file);

	uint8_t *decodedFile = decodeBase64(buffer, encodedLen);
	delete[] buffer;

	index = 0;
	for (auto &playersScore: ui.playersScores)
		playersScore = deserializeScorePlayer(decodedFile, index);


	delete[] decodedFile;
}
