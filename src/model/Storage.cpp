//
// Created by Filip Dabkowski on 25/11/2023.
//

#include "cstring"
#include "cstdint"

#include "Storage.h"
#include "SerializeToFile.h"
#include "Player.h"

void serialisePoint(Point point, uint8_t *buffer, size_t &offset) {
	serializeInt(point.pawnsInside, buffer, offset);
	for (auto &pawn : point.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
}

Point deserializePoint(Board &board, const uint8_t *buffer, size_t &index) {
	Point point;
	point.pawnsInside = deserializeInt(buffer, index);
	for (auto &pawn : point.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	return point;
}

void serialiseCourt(Court court, uint8_t *buffer, size_t &offset) {
	serializeInt(court.pawnsInside, buffer, offset);
	for (auto &pawn : court.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
	serialisePlayerPointer(court.owner, buffer, offset);
}

Court deserializeCourt(Board &board, const uint8_t *buffer, size_t &index) {
	Court court;
	court.pawnsInside = deserializeInt(buffer, index);
	for (auto &pawn : court.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	court.owner = deserializePlayerPointer(board, buffer, index);
	return court;
}

void serialiseBar(Bar bar, uint8_t *buffer, size_t &offset) {
	serializeInt(bar.pawnsInside, buffer, offset);
	for (auto &pawn : bar.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
}

Bar deserializeBar(Board &board, const uint8_t *buffer, size_t &index) {
	Bar bar;
	bar.pawnsInside = deserializeInt(buffer, index);
	for (auto &pawn : bar.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	return bar;
}

