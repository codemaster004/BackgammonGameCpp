//
// Created by Filip Dabkowski on 12/11/2023.
//
// Maybe keep pawn color as enum?

#include "cstring"
#include "cstdint"

#include "Pawn.h"
#include "Board.h"
#include "SerializeToFile.h"


int canBeMoved(Board &game, int pointIndex, int moveBy) {
	// Index out of range
	if (pointIndex >= nPoints || pointIndex < 0)
		return -1;
	// Moving Pawn from an empty Point
	if (game.points[pointIndex].pawnsInside == 0)
		return -1;
	// Moving not your Pawn
	if (game.points[pointIndex].pawns[0]->ownerId != game.currentPlayerId)
		return -1;

	// Handle move in both direction based on Pawn "Color"
	short direction = game.points[pointIndex].pawns[0]->moveDirection;
	int destinationIndex = pointIndex + moveBy * direction;
	// Moving outside of range
	if (destinationIndex >= nPoints || destinationIndex < 0)
		return -1;

	// We check all the index Pawn can be moved by player from pos A to B
	return destinationIndex;
}

/*
 * function checks for what kind of move will happen
 * 	BLOCKED - can not move to this Point
 * 	POSSIBLE - can move to this Point
 * 	CAPTURE - can move and a Capture will happen
 */
MoveToPoint canMoveTo(Board &game, int fromIndex, int toIndex) {
	// Consider the destination indexes are already check
	int destinationSize = game.points[toIndex].pawnsInside;
	// Moving to full Point
	if (destinationSize == PAWNS_PER_POINT)
		return BLOCKED;
	// Check for empty destination Point
	if (destinationSize == 0)
		return POSSIBLE;
	// Check if Point is occupied by same player, by now we know the point has at least one empty slot
	if (game.points[toIndex].pawns[0]->ownerId == game.points[fromIndex].pawns[0]->ownerId)
		return POSSIBLE;
	// Check if Point is blocked by opponent
	if (destinationSize > CAPTURE_THRESHOLD)
		return BLOCKED;
	return CAPTURE; // We know Point has an opponents Pawn, but we can Capture it
}

MoveToPoint determineMoveType(Board &game, int pointIndex, int moveBy) {
	int destination = canBeMoved(game, pointIndex, moveBy);
	if (destination < 0) {
		return NOT_ALLOWED;
	}
	return canMoveTo(game, pointIndex, destination);
}

bool enumToBool(MoveToPoint value) {
	return !(value == BLOCKED || value == NOT_ALLOWED);
}

void pawnCapture(Board &game, Point *point) {
	for (int i = 0; i < CAPTURE_THRESHOLD; ++i) {
		game.bar.pawns[game.bar.pawnsInside++] = point->pawns[i];
	}
	point->pawnsInside -= CAPTURE_THRESHOLD;
}

/// Handle Serialization of Pawn object
void serialisePawn(Pawn pawn, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &pawn, sizeof(Pawn));
	offset += sizeof(Pawn);
}

Pawn deserializePawn(const uint8_t *buffer, size_t &index) {
	Pawn pawn;
	std::memcpy(&pawn, buffer + index, sizeof(Pawn));
	index += sizeof(Pawn);
	return pawn;
}

/// Handle POINTER serialization to a Pawn object
void serialisePawnPointer(Pawn *pawn, uint8_t* buffer, size_t &offset) {
	if (pawn == nullptr){
		serializeInt(-1, buffer, offset);
	} else {
		serializeInt(pawn->id, buffer, offset);
	}
}

Pawn *deserializePawnPointer(Board &board, const uint8_t *buffer, size_t &offset) {
	int id = deserializeInt(buffer, offset);
	if (id == -1)
		return nullptr;
	for (Pawn &pawn : board.pawns)
		if (pawn.id == id)
			return &pawn;
	return nullptr;
}
