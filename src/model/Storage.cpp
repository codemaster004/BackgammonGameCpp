//
// Created by Filip Dabkowski on 25/11/2023.
//

#include "cstring"
#include "cstdint"

#include "Storage.h"
#include "SerializeToFile.h"
#include "Player.h"
#include "Pawn.h"

bool removingFromBar(Board &game, Move move) {
	if (hasPawnsOnBar(game.bar, game.currentPlayerId)) {
		return move.from == nPoints;
	}
	return false;
}

int hasPawnsOnBar(Board &game) {
	if (game.bar.pawnsInside <= 0)
		return -1;

	for (int i = totalPawns - 1; i >= 0 ; --i) {
		if (game.bar.pawns[i] == nullptr)
			continue;
		if (game.bar.pawns[i]->ownerId == game.currentPlayerId)
			return i;
	}
	return -1;
}

bool isHomeBoard(int index, int listSize, int direction) {
	if (direction == 1) {
		return index >= listSize - POINTS_PER_BOARD && index < listSize;
	} else if (direction == -1) {
		return index >= 0 && index < POINTS_PER_BOARD;
	}
	return false;
}

int pawnsOnHomeBoard(Board &game) {
	int count = 0;
	for (auto pawn: game.pawns)
		if (pawn.ownerId == game.currentPlayerId && pawn.isHome)
			count++;
	return count;
}

int hasPawnsOnPoint(Board &game, int index) {
	return game.points[index].pawnsInside && game.points[index].pawns[0]->ownerId == game.currentPlayerId;
}

bool canBeEscaping(Board &game, int index) {
	return pawnsOnHomeBoard(game) >= ESCAPE_THRESHOLD && game.points[index].pawns[0]->isHome;
}

Court *pawnsCourt(Board &game, Pawn *pawn) {
	for (auto & court : game.courts) {
		if (court.owner->id == pawn->ownerId) {
			return &court;
		}
	}
	return nullptr;
}

void movePointToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order) {
	Point *toPoint = &game.points[toIndex];
	Point *fromPoint = &game.points[fromIndex];
	Pawn *pawn = fromPoint->pawns[--fromPoint->pawnsInside];

	if (order >= 0) {
		addAfter({.type=POINT_TO_POINT, .from=fromIndex, .to=toIndex, .moveOrder=order, .pawnId=pawn->id}, &history);
		history.moveOrder++;
	}

	toPoint->pawns[toPoint->pawnsInside++] = pawn;
	fromPoint->pawns[fromPoint->pawnsInside] = nullptr;
	checkNewPoint(toPoint, toIndex);
}

void moveBarToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order) {
	if (order >= 0) {
		addAfter({BAR_TO_POINT, fromIndex, toIndex, order, game.bar.pawns[fromIndex]->id}, &history);
		history.moveOrder++;
	}

	Point *toPoint = &game.points[toIndex];
	toPoint->pawns[toPoint->pawnsInside++] = game.bar.pawns[fromIndex];
	game.bar.pawnsInside--;
	game.bar.pawns[fromIndex] = nullptr;
}

void movePointToCourt(Board &game, MoveMade &history, int fromIndex, int order) {
	Point *point = &game.points[fromIndex];
	Pawn *pawn = point->pawns[--point->pawnsInside];
	Court *court = pawnsCourt(game, pawn);
	court->pawns[court->pawnsInside++] = pawn;

	if (order >= 0) {
		addAfter({POINT_TO_COURT, fromIndex, court->pawnsInside - 1, order, pawn->id}, &history);
		history.moveOrder++;
	}
}

void movePointToBar(Board &game, MoveMade &history, int fromIndex, int order) {
	Point *fromPoint = &game.points[fromIndex];
	for (int i = 0; i < CAPTURE_THRESHOLD; ++i) {
		if (order >= 0) {
			addAfter({.type=POINT_TO_BAR, .from=fromIndex, .to=game.bar.pawnsInside, .moveOrder=order,
						 .pawnId=fromPoint->pawns[i]->id}, &history);
			history.moveOrder++;
		}
		game.bar.pawns[game.bar.pawnsInside++] = fromPoint->pawns[i];
	}
	fromPoint->pawnsInside -= CAPTURE_THRESHOLD;
}

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

