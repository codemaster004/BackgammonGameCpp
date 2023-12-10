//
// Created by Filip Dabkowski on 25/11/2023.
//

#ifndef BACKGAMMONGAME_STORAGE_H
#define BACKGAMMONGAME_STORAGE_H

#include "../configs/GameConfigs.h"
#include "Pawn.h"

/**
 * @brief Represents a point on the game board.
 *
 * Each point can hold a certain number of pawns. This structure tracks the number of pawns
 * currently at this point and stores references to these pawns.
 */
typedef struct Point {
	int pawnsInside;               ///< Number of pawns currently at this point.
	Pawn *pawns[PAWNS_PER_POINT];  ///< Array of pointers to pawns at this point.
} Point;

/**
 * @brief Represents the bar area on the game board.
 *
 * The bar is a special area where pawns are placed after being hit. This structure tracks
 * the number of pawns on the bar and stores references to these pawns.
 */
typedef struct Bar {
	int pawnsInside;          ///< Number of pawns currently on the bar.
	Pawn *pawns[totalPawns];  ///< Array of pointers to pawns on the bar.
} Bar;

/**
 * @brief Represents a player's court in the game.
 *
 * The court is an area where a player collects their pawns that have escaped the Board. This
 * structure tracks the number of pawns in the court and stores references to these pawns,
 * as well as a reference to the court's owner.
 */
typedef struct Court {
	int pawnsInside;               ///< Number of pawns in the court.
	Pawn *pawns[PAWNS_PER_PLAYER]; ///< Array of pointers to pawns in the court.
	Player *owner;                 ///< Pointer to the player who owns this court.
} Court;


bool removingFromBar(Board &game, Move move);

int pawnIndexOnBar(Board &game, int playerId);

bool isHomeBoard(int index, int listSize, int direction);

int pawnsOnHomeBoard(Board &game);

Court *pawnsCourt(Board &game, Pawn *pawn);

Court *playersCourt(Board &game, int playerId);

void moveBarToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order=0);

void movePointToPoint(Board &game, MoveMade &history, int fromIndex, int toIndex, int order=0);

void movePointToCourt(Board &game, MoveMade &history, int fromIndex, int order=0);

void movePointToBar(Board &game, MoveMade &history, int fromIndex, int order=0);

void moveCourtToPoint(Board &game, MoveMade &history, int toIndex, int pawnId, int order);

int hasPawnsOnPoint(Board &game, int index);

bool canBeEscaping(Board &game, int index);

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
