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

void serializeInt(int value, uint8_t *buffer, size_t &offset) {
	// Copy the integer value into the buffer at the current offset
	std::memcpy(buffer + offset, &value, sizeof(int));
	offset += sizeof(int);
}

int deserializeInt(const uint8_t *buffer, size_t &index) {
	int value;
	// Copy data from the buffer into the integer variable at the current index
	std::memcpy(&value, buffer + index, sizeof(int));
	index += sizeof(int);
	return value;
}

void saveToFile(char filename[], Board &game) {
	// Directory for saving game states
	const char savesDir[] = "../saves/games/";
	// Allocate buffer for serialization
	auto *bufferTable = new uint8_t[boardSize];

	size_t index = 0;
	// Serialize the game board into the buffer
	serialiseBoard(game, bufferTable, index);

	// Encode the buffer to Base64
	char *encodedFile = encodeBase64(bufferTable, index);
	// Free the serialization buffer
	delete[] bufferTable;

	// Create the full file path
	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);

	// Open the file for writing
	FILE *file = fopen(path, "w");
	delete[] path;

	// Calculate the length of the encoded data
	int encodedLen = finalEncodedDataLen((int) (index));
	// Write the encoded data to the file if file opening was successful
	if (file != nullptr) {
		for (int i = 0; i < encodedLen; ++i) {
			fputc(encodedFile[i], file);
		}

		// Close the file
		fclose(file);
	}

	// Free the encoded file buffer
	delete[] encodedFile;
}

void loadFromFile(char filename[], Board &game) {
	// Directory for saved games
	const char savesDir[] = "../saves/games/";
	// Calculate the expected length of the encoded data
	int encodedLen = finalEncodedDataLen((int) (boardSize));

	size_t index = 0;
	// Allocate buffer for the encoded data
	auto *bufferTable = new char[encodedLen];

	// Create the full file path
	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);
	// Open the file for reading
	FILE *file = fopen(path, "r");
	delete[] path;

	int ch;
	// Read each character from the file and store it in the buffer
	while ((ch = fgetc(file)) != EOF) {
		bufferTable[index++] = (char) (ch);
	}
	fclose(file);

	// Check if the read data length matches the expected length
	if (index != encodedLen) {
		// TODO: Generate some error for the user to see
	}
	// Decode the read data from Base64
	uint8_t *decodedBoard = decodeBase64(bufferTable, encodedLen);
	// Free the encoded data buffer
	delete[] bufferTable;

	index = 0;
	// Deserialize the decoded data into the game board
	deserializeBoard(decodedBoard, index, game);

	// Deserialize the decoded data into the game board
	delete[] decodedBoard;
}

void saveScores(char filename[], ScorePlayer scores[]) {
	// Directory for saving scores
	const char savesDir[] = "../saves/";

	// Allocate buffer for serialization
	auto *buffer = new uint8_t[N_ALL_PLAYERS * PLAYERS_SIZE];

	size_t index = 0;
	// Serialize the scores of each player into the buffer
	for (int i = 0; i < N_ALL_PLAYERS; ++i) {
		serializeScorePlayer(scores[i], buffer, index);
	}

	// Encode the buffer to Base64
	char *encodedFile = encodeBase64(buffer, index);
	delete[] buffer;

	// Create the full file path
	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);
	// Open the file for writing
	FILE *file = fopen(path, "w");
	delete[] path;

	// Calculate the length of the encoded data
	int encodedLen = finalEncodedDataLen((int) (index));

	// Write the encoded data to the file if file opening was successful
	if (file != nullptr) {
		for (int i = 0; i < encodedLen; ++i) {
			fputc(encodedFile[i], file);
		}

		// Close the file
		fclose(file);
	}

	// Free the encoded file buffer
	delete[] encodedFile;
}

void loadScores(char filename[], UserInterface &ui) {
	// Directory for saved scores
	const char savesDir[] = "../saves/";

	// Calculate the expected length of the encoded data
	int encodedLen = finalEncodedDataLen(N_ALL_PLAYERS * PLAYERS_SIZE);

	size_t index = 0;
	// Allocate buffer for the encoded data
	auto *buffer = new char[encodedLen];

	// Create the full file path
	char *path = joinStrings1((char *) (savesDir), sizeof(savesDir) - 1, filename, len(filename) - 1);
	// Open the file for reading
	FILE *file = fopen(path, "r");
	delete[] path;

	// Read each character from the file and store it in the buffer
	int ch;
	while ((ch = fgetc(file)) != EOF) {
		buffer[index++] = (char) (ch);
	}
	// Close the file
	fclose(file);

	// Decode the read data from Base64
	uint8_t *decodedFile = decodeBase64(buffer, encodedLen);
	delete[] buffer;

	index = 0;
	// Deserialize the decoded data into the players' scores
	for (auto &playersScore: ui.playersScores)
		playersScore = deserializeScorePlayer(decodedFile, index);


	// Free the decoded data buffer
	delete[] decodedFile;
}

// PRIVATE FUNCTION DECLARATIONS //
