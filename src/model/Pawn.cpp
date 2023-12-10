//
// Created by Filip Dabkowski on 12/11/2023.
//
// Maybe keep pawn color as enum?

#include "cstring"

#include "Pawn.h"
#include "SerializeToFile.h"
#include "History.h"
#include "Storage.h"


// PRIVATE FUNCTION HEADERS //

bool canBeMoved(Board &game, int pointIndex);

int calculateDestination(Board &game, int index, int moveBy);

/**
 * @brief function checks for what kind of move will happen
 *
 * BLOCKED - can not move to this Point
 * POSSIBLE - can move to this Point
 * CAPTURE - can move and a Capture will happen
 */
MoveType canMoveTo(Board &game, Pawn *pawn, int toIndex);

int *findPlayerIndexes(Board &game, int playerId, int &count, int limit=0);

void findCapturingMoves(const int *playerIndexes, int playerN, const int *opponentIndexes, int opponentN, int *&capturingMoves, int &count, int moveBy);

int *forceCapture(Board &game, int moveBy, int direction, int &count);

MoveType checkForCapture(Board &game, int from, int moveBy, int destination);

MoveType checkForcingEscape(Board &game, int fromIndex, int toIndex);

int minHomeIndex(int fromIndex);

void checkForEscape(Board &game, int &minIndex, bool &canEscape, int &minPoint, int &maxPoint, int change);

MoveType determineMoveType(Board &game, int pointIndex, int moveBy);

bool enumToBool(MoveType value);

void reverseMove(Board &game, MoveMade &head, MoveMade *move);

MoveStatus handleMoveBarToPoint(Board &game, Move move, int indexOnBar, MoveMade &history);

MoveStatus handleMovePointToPoint(Board &game, Move move, MoveMade &history);

// PUBLIC FUNCTION DECLARATIONS //

Pawn *getPawn(Board &game, int id) {
	// Iterate through pawns in the game
	for (auto &pawn : game.pawns) {
		// Return a pointer to the pawn if the ID matches
		if (pawn.id == id)
			return &pawn;
	}
	// Return nullptr if no pawn with the specified ID is found
	return nullptr;
}

bool statusToBool(MoveStatus type) {
	// Return true for successful move statuses, false for others
	return type == MOVE_SUCCESSFUL || type == MOVE_TO_COURT;
}

void checkNewPoint(Point *toPoint, int pointIndex) {
	// Update the 'isHome' status of the last pawn in the point
	toPoint->pawns[toPoint->pawnsInside - 1]->isHome = isHomeBoard(pointIndex, nPoints, toPoint->pawns[toPoint->pawnsInside - 1]->moveDirection);
}

MoveStatus handlePawnMovement(Board &game, Move move, MoveMade &history) {
	// Check if any pawn of the current player is on the bar
	int indexOnBar = pawnIndexOnBar(game, game.currentPlayerId);
	if (indexOnBar >= 0) {
		// Move pawn from bar to point
		return handleMoveBarToPoint(game, move, indexOnBar, history);
	} else {
		// Move pawn from one point to another
		return handleMovePointToPoint(game, move, history);
	}
}

void reverseMoves(Board &game, MoveMade &head) {
	// Return if there are no moves to reverse
	if (!head.moveOrder)
		return;

	// Retrieve the pawn involved in the last move and set the current player ID
	Pawn *pawn = getPawn(game, head.prevMove->pawnId);
	if (pawn != nullptr)
		game.currentPlayerId = pawn->ownerId;

	// Start from the most recent move
	MoveMade *tempMove = head.prevMove;
	int totalMoves = tempMove->moveOrder;

	// Reverse each move in the history
	for (int i = 0; i <= totalMoves; ++i) {
		reverseMove(game, head, tempMove);
		tempMove = tempMove->prevMove;
		removeAfter(&head);
	}
}

int hasPawnsOnBar(Bar &bar, int playerId) {
	int count = 0; // Initialize count of pawns
	// Iterate through each pawn on the bar
	for (auto &pawn : bar.pawns) {
		if (pawn == nullptr) // Skip if there is no pawn
			continue;
		if (pawn->ownerId == playerId) // Increment count if pawn belongs to the specified player
			count++;
	}
	return count; // Return the total count of pawns
}


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

// PRIVATE FUNCTION DECLARATIONS //

bool canBeMoved(Board &game, int pointIndex) {
	// Index out of range
	if (pointIndex >= nPoints || pointIndex < 0)
		return false;
	// Moving Pawn from an empty Point
	if (game.points[pointIndex].pawnsInside == 0)
		return false;
	// Moving not your Pawn
	if (game.points[pointIndex].pawns[0]->ownerId != game.currentPlayerId)
		return false;

	return true;
}

int calculateDestination(Board &game, int index, int moveBy) {
	// Handle move in both direction based on Pawn "Color"
	short direction = game.points[index].pawns[0]->moveDirection;
	int destinationIndex = index + moveBy * direction;

	// We check all the index Pawn can be moved by player from pos A to B
	return destinationIndex;
}

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

int *findPlayerIndexes(Board &game, int playerId, int &count, int limit) {
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

void findCapturingMoves(const int *playerIndexes, int playerN, const int *opponentIndexes, int opponentN, int *&capturingMoves, int &count, int moveBy) {
	for (int i = 0; i < playerN; ++i) {
		for (int j = 0; j < opponentN; ++j) {
			// Checking all possibilities
			if (playerIndexes[i] + moveBy == opponentIndexes[j]) {
				resizeTable(capturingMoves, count, 1);
				capturingMoves[count - 1] = playerIndexes[i];
			}
		}
	}
}

int *forceCapture(Board &game, int moveBy, int direction, int &count) {
	// search for all indexes of the player pawns
	int playerN = 0;
	int *playerIndexes = findPlayerIndexes(game, game.currentPlayerId, playerN);

	// find opponents indexes with < CAPTURE THRESHOLD
	int opponentN = 0;
	int opponentId = getOpponent(game, game.currentPlayerId)->id;
	int *opponentIndexes = findPlayerIndexes(game, opponentId, opponentN, CAPTURE_THRESHOLD);

	// allocate here maybe return count
	auto capturingMoves = new int [count];
	findCapturingMoves(playerIndexes, playerN, opponentIndexes, opponentN, capturingMoves, count, moveBy * direction);

	delete[] playerIndexes;
	delete[] opponentIndexes;

	return capturingMoves;
}

MoveType checkForCapture(Board &game, int from, int moveBy, int destination) {
	int count = 0;
	int multiplier = from > destination ? -1 : 1;
	int *indexes = forceCapture(game, moveBy, multiplier, count);

	if (count > 0) {
		// capturing a pawn is possible in this move
		int extremeIndex = indexes[0];
		for (int i = 1; i < count; ++i)
			if (indexes[i] * multiplier < extremeIndex * multiplier)
				extremeIndex = indexes[i];
		delete[] indexes;

		if (extremeIndex != from)
			// the player does not want to capture so make him
			return CAPTURE_POSSIBLE;
	}
	return POSSIBLE;
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
			return fromIndex == minPoint ? ESCAPE_BOARD : ESCAPE_POSSIBLE;
		} else {
			return fromIndex == maxPoint ? ESCAPE_BOARD : ESCAPE_POSSIBLE;
		}
	}
	return POSSIBLE;
}

MoveType determineMoveType(Board &game, int pointIndex, int moveBy) {
	int destination;
	if (canBeMoved(game, pointIndex))
		destination = calculateDestination(game, pointIndex, moveBy);
	else return NOT_ALLOWED;

	MoveType escapeMode = checkForcingEscape(game, pointIndex, destination);
	if (escapeMode != POSSIBLE)
		return escapeMode;


	MoveType forceMove = checkForCapture(game, pointIndex, moveBy, destination);
	if (forceMove == CAPTURE_POSSIBLE)
		return CAPTURE_POSSIBLE;

	return canMoveTo(game, game.points[pointIndex].pawns[0], destination);
}

bool enumToBool(MoveType value) {
	return !(value == BLOCKED || value == NOT_ALLOWED);
}

MoveStatus handleMoveBarToPoint(Board &game, Move move, int indexOnBar, MoveMade &history) {
	if (!removingFromBar(game, move))
		return PAWNS_ON_BAR;

	short direction = game.bar.pawns[indexOnBar]->moveDirection;
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

void checkWinningCondition(Board &game, int pointIndex) {
	Court *playerCourt = pawnsCourt(game, game.points[pointIndex].pawns[0]);
	if (playerCourt->pawnsInside == PAWNS_PER_PLAYER)
		game.winnerPlayerId = game.currentPlayerId;
}

MoveStatus handleMoving(Board &game, MoveMade &history, MoveType moveType, Move move, int toIndex) {
	if (moveType == ESCAPE_BOARD) {
		movePointToCourt(game, history, move.from);
		checkWinningCondition(game, move.from);
		return MOVE_TO_COURT;
	}

	int additionalMove = 0;
	if (moveType == CAPTURE && ++additionalMove) {
		movePointToBar(game, history, toIndex);
	}

	movePointToPoint(game, history, move.from, toIndex, additionalMove);
	return MOVE_SUCCESSFUL;
}

MoveStatus handleMovePointToPoint(Board &game, Move move, MoveMade &history) {
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
			moveCourtToPoint(game, head, move->from, move->pawnId, -1);
			break;
		case COURT_TO_POINT:
			break;
	}
}
