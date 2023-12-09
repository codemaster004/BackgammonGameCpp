//
// Created by Filip Dabkowski on 12/11/2023.
//
// Maybe keep pawn color as enum?

#include "cstring"

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

void resizeTable(int *&table, int &size, int increase=1) {
	int newSize = size + increase;
	// Create a new array with size increased by 1
	int *newArray = new int[newSize];

	// Copy the elements from the old array to the new array
	for (int i = 0; i < size; ++i) {
		newArray[i] = table[i];
	}

	// Initialize the new element
	for (int i = size; i < newSize; ++i) {
		newArray[i] = 0;
	}

	// Delete the old array
	delete[] table;

	// Update the original array pointer and size
	table = newArray;
	size = newSize;
}

int *findPlayerIndexes(Board &game, int playerId, int &count, int limit=0) {
	auto indexes = new int [count];
	for (int i = 0; i < nPoints; ++i) {
		Point point = game.points[i];
		if (!limit || point.pawnsInside <= limit) {
			if (point.pawnsInside > 0 && point.pawns[0]->ownerId == playerId) {
				resizeTable(indexes, count, 1);
				indexes[count - 1] = i;
			}
		}
	}

	return indexes;
}

void initTable(int *&table, int count, int value) {
	for (int i = 0; i < count; ++i) {
		table[i] = value;
	}
}

int *forceCapture(Board &game, int moveBy, int direction, int &count) {
	// search for all indexes of the player pawns
	int playerCount = 0;
	int *playerIndexes = findPlayerIndexes(game, game.currentPlayerId, playerCount);

	// find opponents indexes with < CAPTURE THRESHOLD
	int opponentCount = 0;
	int *opponentIndexes = findPlayerIndexes(game, (game.currentPlayerId + 1) % N_PLAYERS, opponentCount, CAPTURE_THRESHOLD);

	// allocate here maybe return count
	auto capturingMoves = new int [count];

	for (int i = 0; i < playerCount; ++i) {
		for (int j = 0; j < opponentCount; ++j) {
			// Checking all possibilities
			if (playerIndexes[i] + moveBy * direction == opponentIndexes[j]) {
				resizeTable(capturingMoves, count, 1);
				capturingMoves[count - 1] = playerIndexes[i];
			}
		}
	}

	delete[] playerIndexes;
	delete[] opponentIndexes;

	return capturingMoves;
}

MoveType checkForCapture(const int *indexes, int count, int picked) {
	if (count > 0) {
		// capturing a pawn is possible in this move
		bool forced = false;
		for (int i = 0; i < count; ++i)
			if (indexes[i] == picked)
				forced = true;
		delete[] indexes;

		if (!forced)
			// the player does not want to capture
			return CAPTURE_POSSIBLE;
	}
	return POSSIBLE;
}

bool statusToBool(MoveStatus type) {
	return type == MOVE_SUCCESSFUL || type == MOVE_TO_COURT;
}

int minHomeIndex(int fromIndex) {
	if (fromIndex >= nPoints / 2) {
		return nPoints - POINTS_PER_BOARD;
	} else {
		return 0;
	}
}

void checkForEscape(Board &game, int &minIndex, bool &canEscape, int &minPoint, int &maxPoint, int change) {
	for (int i = minIndex; i < minIndex + POINTS_PER_BOARD; ++i)
		if (hasPawnsOnPoint(game, i)) {
			if (i + change == nPoints)
				canEscape = true;
			if (i < minPoint)
				minPoint = i;
			if (i > maxPoint)
				maxPoint = i;
		}
}

MoveType checkForcingEscape(Board &game, int fromIndex, int toIndex) {
	if (!canBeEscaping(game, fromIndex))
		return toIndex < nPoints && toIndex >= 0 ? POSSIBLE : NOT_ALLOWED;

	if (toIndex == nPoints || toIndex == -1)
		return ESCAPE_BOARD;

	int change = toIndex - fromIndex;

	int minIndex = minHomeIndex(fromIndex);
	bool canEscape = false;
	int minPoint = nPoints, maxPoint = -1;
	checkForEscape(game, minIndex, canEscape, minPoint, maxPoint, change);
	if (canEscape)
		return ESCAPE_POSSIBLE;

	if (toIndex < 0 || toIndex > nPoints) {
		if (fromIndex >= nPoints / 2) {
			return fromIndex == minPoint ? POSSIBLE : ESCAPE_POSSIBLE;
		} else {
			return fromIndex == maxPoint ? POSSIBLE : ESCAPE_POSSIBLE;
		}
	}
	return POSSIBLE;
}

MoveType determineMoveType(Board &game, int pointIndex, int moveBy) {
	int destination = canBeMoved(game, pointIndex, moveBy);
	MoveType escapeMode = checkForcingEscape(game, pointIndex, destination);
	if (escapeMode != POSSIBLE)
		return escapeMode;

	int captureCount = 0;
	int *captureIndexes = forceCapture(game, moveBy, pointIndex > destination ? -1 : 1, captureCount);
	MoveType forceMove = checkForCapture(captureIndexes, captureCount, pointIndex);
	if (forceMove == CAPTURE_POSSIBLE)
		return CAPTURE_POSSIBLE;

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

MoveStatus handleMoving(Board &game, MoveMade &history, MoveType moveType, Move move, int toIndex) {
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

// TODO: Shorten
MoveStatus movePointToPoint(Board &game, Move move, MoveMade &history) {
	MoveType moveType = determineMoveType(game, move.from, move.by);
	if (!enumToBool(moveType))
		return MOVE_FAILED;
	if (moveType == CAPTURE_POSSIBLE)
		return FORCE_CAPTURE;
	if (moveType == ESCAPE_POSSIBLE)
		return  FORCE_ESCAPE;

	short direction = game.points[move.from].pawns[0]->moveDirection;
	int toIndex = move.from + move.by * direction;

	return handleMoving(game, history, moveType, move, toIndex);
}

MoveStatus handlePawnMovement(Board &game, Move move, MoveMade &history) {
	int indexOnBar = hasPawnsOnBar(game);
	if (indexOnBar >= 0) {
		return moveBarToPoint(game, move, indexOnBar, history);
	} else {
		return movePointToPoint(game, move, history);
	}
}

void reverseMove(Board &game, MoveMade &head, MoveMade *move) {
	switch (move->type) {
		case POINT_TO_POINT:
			movePointToPoint(game, head, move->to, move->from, -1);
			break;
		case POINT_TO_BAR:
			moveBarToPoint(game, head, move->to, move->from, -1);
			break;
		case BAR_TO_POINT:
			movePointToBar(game, head, move->to, -1);
			break;
		case POINT_TO_COURT:
			break;
		case COURT_TO_POINT:
			break;
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
		reverseMove(game, head, tempMove);
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
