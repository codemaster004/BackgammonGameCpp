//
// Created by Filip Dabkowski on 05/12/2023.
//

#ifndef BACKGAMMONGAME_HISTORY_H
#define BACKGAMMONGAME_HISTORY_H

#include "cstdint"

/**
 * @brief Enumerates the possible directions of a move in the game.
 */
enum MoveDirection {
	POINT_TO_POINT = 1, ///< Move from one point to another on the board.
	POINT_TO_BAR,       ///< Move from a point to the bar.
	BAR_TO_POINT,       ///< Move from the bar to a point on the board.
	POINT_TO_COURT,     ///< Move from a point to the player's court.
	COURT_TO_POINT,     ///< Move from the court to a point on the board.
};

/**
 * @brief Represents a move that has been made in the game.
 *
 * This structure holds details about a completed move, including its type,
 * starting and ending points, the order in which the move was made, the ID of
 * the pawn moved, and a pointer to the previous move.
 */
typedef struct MoveMade {
	MoveDirection type;    ///< Type of the move.
	int from;              ///< Starting point of the move.
	int to;                ///< Ending point of the move.
	int moveOrder;         ///< Order in which the move was made.
	int pawnId;            ///< ID of the pawn moved.
	MoveMade *prevMove;    ///< Pointer to the previous move.
} MadeMove;


void addAfter(MoveMade data, MoveMade *lastMove);

void removeAfter(MoveMade *latestMove);

MoveMade *getFist(MoveMade head);

void serializeHistory(MoveMade &head, uint8_t *buffer, size_t &offset);

void deserializeHistory(const uint8_t *buffer, size_t limit, MoveMade &history);

void loadHistoryFromFile(char filename[], MoveMade &head);

void saveHistoryToFile(char filename[], MoveMade &head);

#endif //BACKGAMMONGAME_HISTORY_H
