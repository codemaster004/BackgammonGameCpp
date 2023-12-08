//
// Created by Filip Dabkowski on 05/12/2023.
//

#include <cstdio>
#include "History.h"
#include "SerializeToFile.h"
#include "../Base64.h"
#include "../ByteContainer.h"

#define MOVE_SIZE (sizeof(int) * 5)


void addAfter(MoveMade data, MoveMade *lastMove) {
	auto move = new MoveMade [1];
	*move = data;
	move->prevMove = lastMove->prevMove;
	lastMove->prevMove = move;
}

void removeAfter(MoveMade *latestMove) {
	MoveMade *temp = latestMove->prevMove->prevMove;
	delete[] latestMove->prevMove;
	latestMove->prevMove = temp;
	latestMove->moveOrder--;
}

void serializeMove(MoveMade *move, uint8_t *buffer, size_t &offset) {
	serializeInt(move->type, buffer, offset);
	serializeInt(move->from, buffer, offset);
	serializeInt(move->to, buffer, offset);
	serializeInt(move->moveOrder, buffer, offset);
	serializeInt(move->pawnId, buffer, offset);
}

void deserializeMove(const uint8_t *buffer, size_t &offset, MoveMade *move) {
	move->type = (MoveDirection) (deserializeInt(buffer, offset));
	move->from = deserializeInt(buffer, offset);
	move->to = deserializeInt(buffer, offset);
	move->moveOrder = deserializeInt(buffer, offset);
	move->pawnId = deserializeInt(buffer, offset);
}

void serializeHistory(MoveMade &head, uint8_t *buffer, size_t &offset) {
	MoveMade *element = head.prevMove;
	for (int _ = 0; _ < head.moveOrder; ++_) {
		serializeMove(element, buffer, offset);
		element = element->prevMove;
	}
}

void deserializeHistory(const uint8_t *buffer, size_t limit, MoveMade &history) {
	size_t index = 0;
	MoveMade *tail = &history;
	while (index < limit / MOVE_SIZE * MOVE_SIZE) {
		MoveMade tempMove = {};
		deserializeMove(buffer, index, &tempMove);
		addAfter(tempMove, tail);
		history.moveOrder++;
		tail = tail->prevMove;
	}
}

void saveHistoryToFile(char filename[], MoveMade &head) {
	const char hisDir[] = "../moves/";
	auto *bufferTable = new uint8_t[head.moveOrder*MOVE_SIZE];

	size_t index = 0;
	serializeHistory(head, bufferTable, index);
	char *encodedFile = encodeBase64(bufferTable, index);
	delete[] bufferTable;

	char *path = joinStrings(hisDir, sizeof(hisDir) - 1, filename, 9);
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

void loadHistoryFromFile(char filename[], MoveMade &head) {
	ByteContainer bufferTable;
	initByteContainer(bufferTable);
	const char *hisDir = "../moves/";

	char *path = joinStrings(hisDir, filename);
	FILE *file = fopen(path, "r");
	delete[] path;

	int ch;
	while ((ch = fgetc(file)) != EOF) {
		addElement(bufferTable, ch);
	}

	fclose(file);
	size_t size = bufferTable.dataCount;
	auto transformedTable = new char [size];
	flatten(bufferTable, transformedTable);

	destroyByteContainer(bufferTable);

	uint8_t *decoded = decodeBase64(transformedTable, size);

	deserializeHistory(decoded, finalDecodedDataLen((int)(size)), head);

}
