//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

#include "cstdint"

#include "History.h"

typedef struct Player Player;
typedef struct Board Board;
typedef struct Bar Bar;
typedef struct Point Point;

/**
 * @brief Represents a move in the game.
 *
 * This structure holds details about a player's move, including the starting point,
 * the dice value used, and the remaining moves in the turn.
 */
typedef struct Move {
	int from;        ///< Starting point of the move.
	int by;          ///< Dice value used for the move.
	int movesLeft;   ///< Number of remaining moves in the turn.
} Move;

/**
 * @brief Enumerates possible types of moves in the game.
 */
enum MoveType {
	BLOCKED,          ///< Move is blocked by opponent's pawns.
	POSSIBLE,         ///< Move is possible.
	CAPTURE,          ///< Move results in a capture.
	NOT_ALLOWED,      ///< Move is not allowed.
	ESCAPE_BOARD,     ///< Move leads to escaping the board.
	CAPTURE_POSSIBLE, ///< Capture is possible.
	ESCAPE_POSSIBLE,  ///< Escape from board is possible.
};

/**
 * @brief Enumerates the colors of pawns in the game.
 */
enum PawnColor {
	PAWN_WHITE, ///< White pawn.
	PAWN_BLACK, ///< Black pawn.
};

/**
 * @brief Enumerates the status of a move after being played.
 */
enum MoveStatus {
	MOVE_SUCCESSFUL, ///< Move was successful.
	PAWNS_ON_BAR,    ///< Player has pawns on the bar that must be moved first.
	MOVE_FAILED,     ///< Move failed.
	MOVE_TO_COURT,   ///< Move towards the player's court.
	FORCE_CAPTURE,   ///< Player is forced to capture an opponent's pawn.
	FORCE_ESCAPE,    ///< Player is forced to escape a pawn from the board.
};

/**
 * @brief Represents a pawn in the game.
 *
 * This structure contains details about a pawn, including its owner, ID, home status,
 * color, and the direction it moves on the board.
 */
typedef struct Pawn {
	int ownerId;          ///< ID of the pawn's owner.
	int id;               ///< Unique ID of the pawn.
	bool isHome;          ///< Indicates if the pawn is in its home area.
	PawnColor color;      ///< Color of the pawn.
	short moveDirection;  ///< Direction the pawn moves on the board.
} Pawn;


Pawn *getPawn(Board &game, int id);

bool statusToBool(MoveStatus type);

int canBeMoved(Board &game, int pointIndex, int moveBy);

MoveType canMoveTo(Board &game, Pawn *pawn, int toIndex);

MoveType determineMoveType(Board &game, int pointIndex, int moveBy);

MoveStatus moveBarToPoint(Board &game, Move move, int indexOnBar, MoveMade &history);

MoveStatus movePointToPoint(Board &game, Move move, MoveMade &history);

MoveStatus handlePawnMovement(Board &game, Move move, MoveMade &history);

bool enumToBool(MoveType value);

int hasPawnsOnBar(Bar &bar, int playerId);

short findMoveDirection(Pawn **pawns, int count, int playerId);

void reverseMove(Board &game, MoveMade &head);

void checkNewPoint(Point *toPoint, int pointIndex);

/// Handle Serialization of Pawn object
void serialisePawn(Pawn pawn, uint8_t *buffer, size_t &offset);

Pawn deserializePawn(const uint8_t *buffer, size_t &index);

/// Handle POINTER serialization to a Pawn object
void serialisePawnPointer(Pawn *pawn, uint8_t* buffer, size_t &offset);

Pawn *deserializePawnPointer(Board &board, const uint8_t *buffer, size_t &offset);

#endif //BACKGAMMONGAME_PAWN_H
