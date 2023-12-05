//
// Created by Filip Dabkowski on 05/12/2023.
//

#include <cstdio>
#include "History.h"
#include "SerializeToFile.h"
#include "../Base64.h"


void addAfter(MoveMade data, MoveMade *lastMove) {
	auto move = new MoveMade [1];
	*move = data;
	move->prevMove = lastMove->prevMove;
	lastMove->prevMove = move;
	lastMove->moveOrder++;
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

void serializeHistory(MoveMade head, uint8_t *buffer, size_t &offset) {
	MoveMade *element = head.prevMove;
	for (int _ = 0; _ < head.moveOrder; ++_) {
		serializeMove(element, buffer, offset);
		element = element->prevMove;
	}
}

void saveHistoryToFile(char filename[], MoveMade head) {
	auto *bufferTable = new uint8_t[head.moveOrder*4];

	size_t index = 0;
	serializeHistory(head, bufferTable, index);
	char *encodedFile = encodeBase64(bufferTable, index);
	delete[] bufferTable;

	FILE *file = fopen("../games/History.txt", "w");

	int encodedLen = finalEncodedDataLen((int) (index));
	if (file != nullptr) {
		for (int i = 0; i < encodedLen; ++i) {
			fputc(encodedFile[i], file);
		}

		fclose(file);
	}

	delete[] encodedFile;

}