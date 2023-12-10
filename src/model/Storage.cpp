//
// Created by Filip Dabkowski on 25/11/2023.
//

#include "cstring"
#include "cstdint"

#include "Storage.h"
#include "SerializeToFile.h"
#include "Player.h"
#include "Pawn.h"

// PRIVATE FUNCTION HEADERS //

/**
 * @brief Finds the first free index on a game bar to place a pawn.
 *
 * @param bar The game bar being checked for a free slot.
 * @return The index of the first free slot, or -1 if none is available.
 */
int getFreeIndexOnBar(Bar bar);

// PUBLIC FUNCTION DECLARATIONS //

bool removingFromBar(Board &game, Move move) {
	// Check if the current player has any pawns on the bar
	if (hasPawnsOnBar(game.bar, game.currentPlayerId)) {
		// Return true if the move is from the bar
		return move.from == nPoints;
	}
	// Return false if there are no pawns on the bar or the move is not from the bar
	return false;
}

int pawnIndexOnBar(Board &game, int playerId) {
	// Check if there are any pawns on the bar
	if (game.bar.pawnsInside <= 0)
		return -1;

	// Iterate through the pawns on the bar in reverse order
	for (int i = totalPawns - 1; i >= 0 ; --i) {
		// Skip if there is no pawn at the current index
		if (game.bar.pawns[i] == nullptr)
			continue;
		// Return the index if the pawn belongs to the specified player
		if (game.bar.pawns[i]->ownerId == playerId)
			return i;
	}
	// Return -1 if no pawn belonging to the player is found
	return -1;
}

bool isHomeBoard(int index, int listSize, int direction) {
	// Check for a player moving in the positive direction
	if (direction == 1) {
		return index >= listSize - POINTS_PER_BOARD && index < listSize;
	} else if (direction == -1) {
		// Check for a player moving in the negative direction
		return index >= 0 && index < POINTS_PER_BOARD;
	}
	// Return false if the index is not within the home board range for either direction
	return false;
}

int pawnsOnHomeBoard(Board &game) {
	int count = 0;
	// Iterate through each pawn in the game
	for (auto pawn: game.pawns) {
		// Check if the pawn belongs to the current player and is on the home board
		if (pawn.ownerId == game.currentPlayerId && pawn.isHome)
			// Increment the count for each pawn on the home board
			count++;
	}
	// Return the total count
	return count;
}

int hasPawnsOnPoint(Board &game, int index) {
	// Check if there are pawns on the point and if the first pawn belongs to the current player
	return game.points[index].pawnsInside && game.points[index].pawns[0]->ownerId == game.currentPlayerId;
}

bool canBeEscaping(Board &game, int index) {
	// Check if the number of pawns on the home board meets the threshold and if the pawn at the index is home
	return pawnsOnHomeBoard(game) >= ESCAPE_THRESHOLD && game.points[index].pawns[0]->isHome;
}

Court *pawnsCourt(Board &game, Pawn *pawn) {
	// Iterate through each court in the game
	for (auto &court : game.courts) {
		// Check if the court belongs to the pawn's owner
		if (court.owner->id == pawn->ownerId) {
			// Return a pointer to the owner's court
			return &court;
		}
	}
	// Return nullptr if no matching court is found
	return nullptr;
}

Court *playersCourt(Board &game, int playerId) {
	// Iterate through each court in the game
	for (auto &court : game.courts) {
		// Check if the court belongs to the specified player
		if (court.owner->id == playerId) {
			// Return a pointer to the player's court
			return &court;
		}
	}
	// Return nullptr if no matching court is found
	return nullptr;
}

void movePointToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order) {
	// Get pointers to the starting and destination points
	Point *toPoint = &game.points[toIndex];
	Point *fromPoint = &game.points[fromIndex];

	// Move the pawn from the starting point
	Pawn *pawn = fromPoint->pawns[--fromPoint->pawnsInside];

	// Record the move in history if the order is non-negative
	if (order >= 0) {
		addAfter({.type=POINT_TO_POINT, .from=fromIndex, .to=toIndex, .moveOrder=order, .pawnId=pawn->id}, &history);
		history.moveOrder++;
	}

	// Place the pawn in the destination point and update the points
	toPoint->pawns[toPoint->pawnsInside++] = pawn;
	fromPoint->pawns[fromPoint->pawnsInside] = nullptr;

	// Check and update the 'isHome' status of the pawn at the new point
	checkNewPoint(toPoint, toIndex);
}

void moveBarToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order) {
	// Record the move in history if the order is non-negative
	if (order >= 0) {
		addAfter({BAR_TO_POINT, fromIndex, toIndex, order, game.bar.pawns[fromIndex]->id}, &history);
		history.moveOrder++;
	}

	// Get a pointer to the destination point
	Point *toPoint = &game.points[toIndex];

	// Move the pawn from the bar to the destination point
	toPoint->pawns[toPoint->pawnsInside++] = game.bar.pawns[fromIndex];
	game.bar.pawnsInside--;
	game.bar.pawns[fromIndex] = nullptr;
}

void movePointToCourt(Board &game, MoveMade &history, int fromIndex, int order) {
	// Retrieve the point and remove the pawn from it
	Point *point = &game.points[fromIndex];
	Pawn *pawn = point->pawns[--point->pawnsInside];

	// Retrieve the court of the pawn's owner and add the pawn to it
	Court *court = pawnsCourt(game, pawn);
	court->pawns[court->pawnsInside++] = pawn;

	// Record the move in history if the order is non-negative
	if (order >= 0) {
		addAfter({POINT_TO_COURT, fromIndex, court->pawnsInside - 1, order, pawn->id}, &history);
		history.moveOrder++;
	}
}

void movePointToBar(Board &game, MoveMade &history, int fromIndex, int order) {
	// Retrieve the starting point
	Point *fromPoint = &game.points[fromIndex];

	// Move up to CAPTURE_THRESHOLD pawns from the point to the bar
	for (int i = 0; i < CAPTURE_THRESHOLD; ++i) {
		int toIndex = getFreeIndexOnBar(game.bar);
		// Record each move in history if the order is non-negative
		if (order >= 0) {
			addAfter({.type=POINT_TO_BAR, .from=fromIndex, .to=toIndex, .moveOrder=order,
						 .pawnId=fromPoint->pawns[i]->id}, &history);
			history.moveOrder++;
		}
		// Move the pawn to the bar
		game.bar.pawns[toIndex] = fromPoint->pawns[i];
		game.bar.pawnsInside++;
	}

	// Update the number of pawns remaining at the starting point
	fromPoint->pawnsInside -= CAPTURE_THRESHOLD;
}

void moveCourtToPoint(Board &game, MoveMade &history, int toIndex, int pawnId, int order) {
	// Get a pointer to the destination point
	Point *point = &game.points[toIndex];

	// Retrieve the pawn and its owner's court
	Pawn *pawn = getPawn(game, pawnId);
	Court *court = pawnsCourt(game, pawn);

	// Move the pawn from the court to the point
	point->pawns[point->pawnsInside++] = court->pawns[--court->pawnsInside];
	court->pawns[court->pawnsInside] = nullptr;

	// Record the move in history if the order is non-negative
	if (order >= 0) {
		addAfter({COURT_TO_POINT, court->pawnsInside, toIndex, order, pawn->id}, &history);
		history.moveOrder++;
	}
}


void serialisePoint(Point point, uint8_t *buffer, size_t &offset) {
	// Serialize the number of pawns inside the point
	serializeInt(point.pawnsInside, buffer, offset);
	// Serialize pointers to each pawn in the point
	for (auto &pawn : point.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
}

Point deserializePoint(Board &board, const uint8_t *buffer, size_t &index) {
	Point point;
	// Deserialize the number of pawns inside the point
	point.pawnsInside = deserializeInt(buffer, index);
	// Deserialize pointers to each pawn in the point
	for (auto &pawn : point.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	return point;
}

void serialiseCourt(Court court, uint8_t *buffer, size_t &offset) {
	// Serialize the number of pawns inside the court
	serializeInt(court.pawnsInside, buffer, offset);
	// Serialize pointers to each pawn in the court
	for (auto &pawn : court.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
	// Serialize pointer to the owner of the court
	serialisePlayerPointer(court.owner, buffer, offset);
}

Court deserializeCourt(Board &board, const uint8_t *buffer, size_t &index) {
	Court court;
	// Deserialize the number of pawns inside the court
	court.pawnsInside = deserializeInt(buffer, index);
	// Deserialize pointers to each pawn in the court
	for (auto &pawn : court.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	// Deserialize pointer to the owner of the court
	court.owner = deserializePlayerPointer(board, buffer, index);
	return court;
}

void serialiseBar(Bar bar, uint8_t *buffer, size_t &offset) {
	// Serialize the number of pawns inside the bar
	serializeInt(bar.pawnsInside, buffer, offset);
	// Serialize pointers to each pawn in the bar
	for (auto &pawn : bar.pawns) {
		serialisePawnPointer(pawn, buffer, offset);
	}
}

Bar deserializeBar(Board &board, const uint8_t *buffer, size_t &index) {
	Bar bar;
	// Deserialize the number of pawns inside the bar
	bar.pawnsInside = deserializeInt(buffer, index);
	// Deserialize pointers to each pawn in the bar
	for (auto &pawn : bar.pawns) {
		pawn = deserializePawnPointer(board, buffer, index);
	}
	return bar;
}

// PRIVATE FUNCTION DECLARATIONS //

int getFreeIndexOnBar(Bar bar) {
	for (int i = 0; i < totalPawns; i++) {
		// Check if the current slot is free (null)
		if (bar.pawns[i] == nullptr)
			// Return the index of the free slot
			return i;
	}
	// Return -1 if no free slot is found
	return -1;
}
