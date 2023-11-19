//
// Created by Filip Dabkowski on 12/11/2023.
//
// TODO: Create a structure for pawn (color?, *player?, location?, isHome, isOnBar, canMoveDirection?, canBeRemoved
// Maybe keep pawn color as enum?

#include "Pawn.h"

bool canBeMoved(Board *game, int pieceIndex, int moveBy) {
	// Index out of range
	if (pieceIndex >= nPoints) return false;
	// Moving Pawn from an empty Point
	if (game->points[pieceIndex].pawnsInside == 0) return false;
	// Moving not your Pawn
	if (*game->points[pieceIndex].pawns[0]->owner != game->currentPlayer) return false;
	// Handle move in both direction based on Pawn "Color"
	short direction = game->points[pieceIndex].pawns[0]->moveDirection;
	int destinationIndex = pieceIndex + moveBy * direction;
	// TODO: Future Filip will put this in a separate function
	// Moving outside of range
	if (destinationIndex >= nPoints || destinationIndex < 0) return false;
	// We know we can access this address
	int destinationSize = game->points[destinationIndex].pawnsInside;
	// Moving to full Point
	if (destinationSize == PAWNS_PER_POINT) return false;
	// Check for empty destination Point
	if (destinationSize == 0) return true;
	// Check if Point is occupied by same player, by now we know the point has at least one empty slot
	if (*game->points[destinationIndex].pawns[0]->owner == game->currentPlayer) return true;
	// Check if Point is blocked by opponent
	if (destinationSize > CAPTURE_THRESHOLD) return false;
	return true; // Opponents Pawn will be captured, but we can normally move there
}
