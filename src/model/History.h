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


/**
 * @brief Inserts a new move after a specified move in the moves list.
 *
 * @param data The MoveMade data to be inserted.
 * @param lastMove Pointer to the MoveMade structure after which the new move will be inserted.
 */
void addAfter(MoveMade data, MoveMade *lastMove);

/**
 * @brief Removes the move that immediately follows the specified move in the moves list.
 *
 * @param latestMove Pointer to the MoveMade structure before the move to be removed.
 */
void removeAfter(MoveMade *latestMove);

/**
 * @brief Saves the history of moves to a file.
 *
 * This function serializes the history of moves and then saves it to a specified file.
 * The serialized history is first converted to Base64 format before writing to the file.
 * The file is stored in a predefined directory for saved moves.
 *
 * @param filename Name of the file where the history will be saved.
 * @param head The head of the moves list, containing the total move order.
 */
void saveHistoryToFile(char filename[], MoveMade &head);

/**
 * @brief Loads the history of moves from a file.
 *
 * This function reads a file containing the serialized and encoded history of moves,
 * decodes it from Base64 format, and then deserializes it to reconstruct the history
 * in the game. The file is read from a predefined directory for saved moves.
 *
 * @param filename Name of the file from which the history will be loaded.
 * @param head Reference to the MoveMade structure where the loaded history will be stored.
 */
void loadHistoryFromFile(char filename[], MoveMade &head);

#endif //BACKGAMMONGAME_HISTORY_H
