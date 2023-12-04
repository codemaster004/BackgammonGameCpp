//
// Created by Filip Dabkowski on 25/11/2023.
//

#ifndef BACKGAMMONGAME_STORAGE_H
#define BACKGAMMONGAME_STORAGE_H

#include "../configs/GameConfigs.h"
#include "Pawn.h"

typedef struct Point {
	int pawnsInside;
	Pawn *pawns[PAWNS_PER_POINT];
} Point;

typedef struct Bar {
	int pawnsInside;
	Pawn *pawns[totalPawns];
} Bar;

typedef struct Court {
	int pawnsInside;
	Pawn *pawns[PAWNS_PER_PLAYER];
	Player *owner;
} Court;

bool removingFromBar(Board &game, Move move);

int hasPawnsOnBar(Board &game);

/// Serialize Point
void serialisePoint(Point point, uint8_t *buffer, size_t &offset);

Point deserializePoint(Board &board, const uint8_t *buffer, size_t &index);

/// Serialize Court
void serialiseCourt(Court court, uint8_t *buffer, size_t &offset);

Court deserializeCourt(Board &board, const uint8_t *buffer, size_t &index);

/// Serialize Bar
void serialiseBar(Bar bar, uint8_t *buffer, size_t &offset);

Bar deserializeBar(Board &board, const uint8_t *buffer, size_t &index);

#endif //BACKGAMMONGAME_STORAGE_H
