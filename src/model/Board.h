//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_BOARD_H
#define BACKGAMMONGAME_BOARD_H

#include "../configs/GameConfigs.h"
#include "Player.h"
#include "Pawn.h"
#include "Storage.h"

/**
 * @brief Represents the game board in Backgammon.
 *
 * This structure holds the entire state of the Backgammon game board, including the
 * players, pawns, points on the board, courts for each player, a bar, the dices,
 * and the ID of the current player.
 */
typedef struct Board {
	Player players[N_PLAYERS]; ///< Array of players in the game.
	Pawn pawns[totalPawns];    ///< Array of pawns used in the game.
	Point points[nPoints];     ///< Points on the board where pawns can be placed.
	Court courts[N_PLAYERS];   ///< Courts for each player.
	Bar bar;                   ///< The bar used in the game.
	int dices[N_DICES];        ///< Array of dice values.
	int currentPlayerId;       ///< ID of the current player.
	int winnerPlayerId;        ///< ID of the player who won the game.
} Board;


/**
 * @brief Populates Player Table with default values.
 *
 * @param game Reference to Board object that holds players to be cleared.
 */
void clearPlayers(Board &game);

/**
 * @brief Clears the game board to its initial state.
 *
 * @param gameBoard Reference to the Board structure to be cleared and set up.
 */
void setClearBoard(Board &gameBoard);

/**
 * @brief Resets the values of all dices to zero.
 *
 * @param dices Pointer to the array of dice values to be cleared.
 */
void clearDices(int *dices);

/**
 * @brief Calculates the limit for the number of dice Options to show
 *
 * This function determines the limit on the number of dice based on the current state
 * of the dice in the user interface. It increments the limit for each dice with a value
 * greater than zero. If all dices can be used add one to the limit.
 *
 * @param ui Reference to the UserInterface object representing the game's UI state.
 * @return int The calculated limit for the number of dice.
 */
int calcDiceLimit(Board &game);

/**
 * @brief Places pawns on the game board in their starting positions.
 *
 * This function sets up the initial configuration of pawns on the game board at the start of the game.
 * It uses a predefined array of starting positions for the pawns and places them accordingly on the
 * board, updating the number of pawns inside each point and marking whether the pawn is in its home area.
 *
 * @param gameBoard Reference to the Board structure where the pawns will be placed.
 */
void placePawns(Board &gameBoard);

/**
 * @brief Serializes the board state into a buffer.
 *
 * This function converts the current state of the game board into a serialized format
 * and stores it in a provided buffer. It sequentially serializes players, pawns, points,
 * courts, bar, dices, and the ID of the current player and winner.
 *
 * @param board Reference to the Board structure to be serialized.
 * @param buffer Pointer to the buffer where the serialized data will be stored.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialiseBoard(Board &board, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes data from a buffer into a Board structure.
 *
 * This function reconstructs the state of a game board from serialized data in a buffer.
 * It sequentially deserializes players, pawns, points, courts, bar, dices, and the IDs
 * of the current player and winner, updating the provided board structure.
 *
 * @param buffer Pointer to the buffer containing the serialized data.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 * @param board Reference to the Board structure where the deserialized data will be stored.
 */
void deserializeBoard(const uint8_t *buffer, size_t &offset, Board &board);


#endif //BACKGAMMONGAME_BOARD_H
