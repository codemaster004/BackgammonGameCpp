//
// Created by Filip Dabkowski on 05/12/2023.
//

#include <cstdio>
#include "History.h"
#include "SerializeToFile.h"
#include "../Base64.h"
#include "../ByteContainer.h"

#define MOVE_SIZE (sizeof(int) * 5)

// PRIVATE FUNCTION HEADERS //

/**
* @brief Serializes the history of moves into a buffer.
*
* @param head The head of the moves list, containing the total move order.
* @param buffer Pointer to the buffer where the serialized data will be stored.
* @param offset Reference to the size_t variable indicating the current offset in the buffer.
*/
void serializeHistory(MoveMade &head, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes a series of moves from a buffer into a moves history.
 *
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param limit The size of the serialized data in bytes.
 * @param history Reference to the MoveMade structure where the deserialized history will be stored.
 */
void deserializeHistory(const uint8_t *buffer, size_t limit, MoveMade &history);

/**
 * @brief Serializes a MoveMade structure into a buffer.
 *
 * @param move Pointer to the MoveMade structure to be serialized.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serializeMove(MoveMade *move, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes data from a buffer into a MoveMade structure.
 *
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 * @param move Pointer to the MoveMade structure where the deserialized data will be stored.
 */
void deserializeMove(const uint8_t *buffer, size_t &offset, MoveMade *move);


// PUBLIC FUNCTION DECLARATION //

void addAfter(MoveMade data, MoveMade *lastMove) {
	auto move = new MoveMade[1];   // Dynamically allocate memory for the new move
	*move = data;                   // Copy the provided move data into the new move
	move->prevMove = lastMove->prevMove; // Set the new move's previous move
	lastMove->prevMove = move;      // Insert the new move after 'lastMove'
}

void removeAfter(MoveMade *latestMove) {
	MoveMade *temp = latestMove->prevMove->prevMove; // Store the move before the one to be removed
	delete[] latestMove->prevMove; // Delete the move that comes after 'latestMove'
	latestMove->prevMove = temp; // Bypass the deleted move in the list
	latestMove->moveOrder--; // Decrement the move order
}

void saveHistoryToFile(char filename[], MoveMade &head) {
	// Directory for saving moves
	const char hisDir[] = "../saves/moves/";
	// Allocate buffer for serialization
	auto *bufferTable = new uint8_t[head.moveOrder * MOVE_SIZE];

	size_t index = 0;
	// Serialize the history into the buffer
	serializeHistory(head, bufferTable, index);
	// Encode the buffer to Base64
	char *encodedFile = encodeBase64(bufferTable, index);
	// Free the serialization buffer
	delete[] bufferTable;

	// Create the full file path
	char *path = joinStrings1(hisDir, sizeof(hisDir) - 1, filename, 9);
	FILE *file = fopen(path, "w");
	delete[] path; // Free the path string

	int encodedLen = finalEncodedDataLen((int) (index)); // Calculate the length of the encoded data
	if (file != nullptr) {
		// Write the encoded data to the file
		for (int i = 0; i < encodedLen; ++i) {
			fputc(encodedFile[i], file);
		}

		// Close the file
		fclose(file);
	}

	// Free the encoded file buffer
	delete[] encodedFile;
}

void loadHistoryFromFile(char filename[], MoveMade &head) {
	// Initialize a container to hold the read bytes
	// As we don't know the initial amount of saved moves
	// We need to reallocate memory to account for this
	ByteContainer bufferTable;
	initByteContainer(bufferTable);

	// Directory for saved moves
	const char *hisDir = "../saves/moves/";

	// Create the full file path
	char *path = joinStrings2(hisDir, filename);

	// Open the file for reading
	FILE *file = fopen(path, "r");
	delete[] path;

	// Read each character from the file and add it to the buffer
	int ch;
	while ((ch = fgetc(file)) != EOF) {
		addElement(bufferTable, ch);
	}

	// Close the file
	fclose(file);

	// Flatten the bufferTable into a single char array
	size_t size = bufferTable.dataCount;
	auto transformedTable = new char[size];
	flatten(bufferTable, transformedTable);

	// Destroy the buffer container to free memory
	destroyByteContainer(bufferTable);

	// Decode the read data from Base64
	uint8_t *decoded = decodeBase64(transformedTable, size);

	// Deserialize the decoded history and store it in 'head'
	deserializeHistory(decoded, finalDecodedDataLen((int) (size)), head);
}


// PRIVATE FUNCTIONS DEFINITIONS //

void serializeMove(MoveMade *move, uint8_t *buffer, size_t &offset) {
	// Serialize basic move info
	serializeInt(move->type, buffer, offset);
	serializeInt(move->from, buffer, offset);
	serializeInt(move->to, buffer, offset);
	// Serialize moves to be made for after automatically
	serializeInt(move->moveOrder, buffer, offset);
	// Serialize Pawn ID for cases when it is needed
	serializeInt(move->pawnId, buffer, offset);
}

void deserializeMove(const uint8_t *buffer, size_t &offset, MoveMade *move) {
	// Deserialize basic move info
	move->type = (MoveDirection) (deserializeInt(buffer, offset));
	move->from = deserializeInt(buffer, offset);
	move->to = deserializeInt(buffer, offset);
	// Deserialize moves to be made for after automatically
	move->moveOrder = deserializeInt(buffer, offset);
	// Deserialize Pawn ID for cases when it is needed
	move->pawnId = deserializeInt(buffer, offset);
}

void serializeHistory(MoveMade &head, uint8_t *buffer, size_t &offset) {
	// Start from the most recent move
	MoveMade *element = head.prevMove;
	for (int _ = 0; _ < head.moveOrder; ++_) {
		// Serialize each move
		serializeMove(element, buffer, offset);
		// Move to the previous move in the list
		element = element->prevMove;
	}
}

void deserializeHistory(const uint8_t *buffer, size_t limit, MoveMade &history) {
	size_t index = 0;
	// Start from the head of the history
	MoveMade *tail = &history;

	// Deserialize and add moves until the entire buffer is processed
	while (index < limit / MOVE_SIZE * MOVE_SIZE) {
		MoveMade tempMove = {};
		deserializeMove(buffer, index, &tempMove); // Deserialize a move
		addAfter(tempMove, tail); // Add the move to the history
		history.moveOrder++; // Increment the move order
		tail = tail->prevMove; // Move to the newly added move
	}
}
