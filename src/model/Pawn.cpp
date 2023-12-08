//
// Created by Filip Dabkowski on 12/11/2023.
//
// Maybe keep pawn color as enum?

#include "cstring"
#include "cstdint"

#include "Pawn.h"
#include "SerializeToFile.h"
#include "History.h"
#include "Storage.h"


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

	// We check all the index Pawn can be moved by player from pos A to B
	return destinationIndex;
}

/*
 * function checks for what kind of move will happen
 * 	BLOCKED - can not move to this Point
 * 	POSSIBLE - can move to this Point
 * 	CAPTURE - can move and a Capture will happen
 */
MoveType canMoveTo(Board &game, Pawn *pawn, int toIndex) {
	// Consider the destination indexes are already check
	int destinationSize = game.points[toIndex].pawnsInside;
	// Moving to full Point
	if (destinationSize == PAWNS_PER_POINT)
		return BLOCKED;
	// Check for empty destination Point
	if (destinationSize == 0)
		return POSSIBLE;
	// Check if Point is occupied by same player, by now we know the point has at least one empty slot
	if (game.points[toIndex].pawns[0]->ownerId == pawn->ownerId)
		return POSSIBLE;
	// Check if Point is blocked by opponent
	if (destinationSize > CAPTURE_THRESHOLD)
		return BLOCKED;
	// We know Point has an opponents Pawn, but we can Capture it
	return CAPTURE;
}

Pawn *getPawn(Board &game, int id) {
	for ( auto &pawn : game.pawns)
		if (pawn.id == id)
			return &pawn;
	return nullptr;
}

MoveType determineMoveType(Board &game, int pointIndex, int moveBy) {
	int destination = canBeMoved(game, pointIndex, moveBy);
	if (destination < 0 || destination > nPoints) {
		if (pawnsOnHomeBoard(game) >= ESCAPE_THRESHOLD && game.points[pointIndex].pawns[0]->isHome) {
			return ESCAPE_BOARD;
		} else {
			return NOT_ALLOWED;
		}
	}
	return canMoveTo(game, game.points[pointIndex].pawns[0], destination);
}

bool enumToBool(MoveType value) {
	return !(value == BLOCKED || value == NOT_ALLOWED);
}

int hasPawnsOnBar(Bar &bar, int playerId) {
	int count = 0;
	for (int i = 0; i < bar.pawnsInside; ++i)
		if (bar.pawns[i]->ownerId == playerId)
			count++;
	return count;
}

short findMoveDirection(Pawn **pawns, int count, int playerId) {
	for (int i = 0; i < count; ++i)
		if (pawns[i]->ownerId == playerId)
			return pawns[i]->moveDirection;
	return 0;
}

MoveStatus moveBarToPoint(Board &game, Move move, int indexOnBar, MoveMade &history) {
	if (!removingFromBar(game, move))
		return PAWNS_ON_BAR;

	short direction = findMoveDirection(game.bar.pawns, game.bar.pawnsInside, game.currentPlayerId);
	int toIndex = (int)((move.from + move.by * direction) % nPoints);
	if (direction > 0)
		toIndex--;

	MoveType moveType = canMoveTo(game, game.bar.pawns[indexOnBar], toIndex);
	if (!enumToBool(moveType))
		return MOVE_FAILED;

	if (moveType == CAPTURE)
		movePointToBar(game, history, toIndex);

	moveBarToPoint(game, history, indexOnBar, toIndex);

	return MOVE_SUCCESSFUL;
}

void checkNewPoint(Point *toPoint, int pointIndex) {
	toPoint->pawns[toPoint->pawnsInside - 1]->isHome = isHomeBoard(pointIndex, nPoints, toPoint->pawns[toPoint->pawnsInside - 1]->moveDirection);
}

MoveStatus movePointToPoint(Board &game, Move move, MoveMade &history) {
	MoveType moveType = determineMoveType(game, move.from, move.by);
	if (!enumToBool(moveType))
		return MOVE_FAILED;

	short direction = game.points[move.from].pawns[0]->moveDirection;
	int toIndex = move.from + move.by * direction;

	if (moveType == ESCAPE_BOARD) {
		movePointToCourt(game, history, move.from);
		return MOVE_TO_COURT;
	}

	int additionalMove = 0;
	if (moveType == CAPTURE && ++additionalMove) {
		movePointToBar(game, history, toIndex);
	}

	movePointToPoint(game, history, move.from, toIndex, additionalMove);
	return MOVE_SUCCESSFUL;
}

MoveStatus handlePawnMovement(Board &game, Move move, MoveMade &history) {
	int indexOnBar = hasPawnsOnBar(game);
	if (indexOnBar >= 0) {
		return moveBarToPoint(game, move, indexOnBar, history);
	} else {
		return movePointToPoint(game, move, history);
	}
}

void reverseMove(Board &game, MoveMade &head) {
	if (!head.moveOrder)
		return;

	Pawn *pawn = getPawn(game, head.prevMove->pawnId);
	if (pawn != nullptr)
		game.currentPlayerId = pawn->ownerId;

	MoveMade *tempMove = head.prevMove;
	int totalMoves = tempMove->moveOrder;
	for (int i = 0; i <= totalMoves; ++i) {
		switch (tempMove->type) {
			case POINT_TO_POINT:
				movePointToPoint(game, head, tempMove->to, tempMove->from, -1);
				break;
			case POINT_TO_BAR:
				moveBarToPoint(game, head, tempMove->to, tempMove->from, -1);
				break;
			case BAR_TO_POINT:
				movePointToBar(game, head, tempMove->to, -1);
				break;
			case POINT_TO_COURT:
				break;
			case COURT_TO_POINT:
				break;
		}
		tempMove = tempMove->prevMove;
		removeAfter(&head);
	}
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
