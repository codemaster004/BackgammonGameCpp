//
// Created by Filip Dabkowski on 12/11/2023.
//
// Maybe keep pawn color as enum?

#include "Pawn.h"

int getOwner(Board *game, int pawnId) {
	for (PlayerPawns &group : game->pawnGroups)
		for (Pawn &pawn : group.pawns)
			if (pawn.id == pawnId)
				return pawn.ownerId;
	return -1;
}

short getMvDir(Board *game, int pawnId) {
	for (PlayerPawns &group : game->pawnGroups)
		for (Pawn &pawn : group.pawns)
			if (pawn.id == pawnId)
				return pawn.moveDirection;
	return 0;
}

Pawn getPawn(Board *game, int pawnId) {
	for (PlayerPawns &group : game->pawnGroups)
		for (Pawn &pawn : group.pawns)
			if (pawn.id == pawnId)
				return pawn;
	return {};
}

bool canBeMoved(Board *game, int pieceIndex, int moveBy) {
	// Index out of range
	if (pieceIndex >= nPoints || pieceIndex < 0)
		return false;
	// Moving Pawn from an empty Point
	if (game->points[pieceIndex].pawnsInside == 0)
		return false;
	// Moving not your Pawn
	if (getOwner(game, game->points[pieceIndex].pawnsId[0]) != game->currentPlayerId)
		return false;

	// Handle move in both direction based on Pawn "Color"
	short direction = getMvDir(game, game->points[pieceIndex].pawnsId[0]);
	int destinationIndex = pieceIndex + moveBy * direction;
	// Moving outside of range
	if (destinationIndex >= nPoints || destinationIndex < 0)
		return false;

	return true; // We check all the index Pawn can be moved by player from pos A to B
}

/*
 * function checks for what kind of move will happen
 * 	BLOCKED - can not move to this Point
 * 	POSSIBLE - can move to this Point
 * 	CAPTURE - can move and a Capture will happen
 */
moveToPoint canMoveTo(Board *game, int fromIndex, int toIndex) {
	// Consider the destination indexes are already check
	int destinationSize = game->points[toIndex].pawnsInside;
	// Moving to full Point
	if (destinationSize == PAWNS_PER_POINT)
		return BLOCKED;
	// Check for empty destination Point
	if (destinationSize == 0)
		return POSSIBLE;
	// Check if Point is occupied by same player, by now we know the point has at least one empty slot
	if (getOwner(game, game->points[toIndex].pawnsId[0]) == getOwner(game, game->points[fromIndex].pawnsId[0]))
		return POSSIBLE;
	// Check if Point is blocked by opponent
	if (destinationSize > CAPTURE_THRESHOLD)
		return BLOCKED;
	return CAPTURE; // We know Point has an opponents Pawn, but we can Capture it
}

bool enumToBool(moveToPoint value) {
	return value != BLOCKED;
}
