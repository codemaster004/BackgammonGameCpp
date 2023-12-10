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


void clearPlayers(Board &game);

void setClearBoard(Board &gameBoard);

void clearDices(int *dices);

void placePawns(Board &gameBoard);

void serialiseBoard(Board &board, uint8_t *buffer, size_t &offset);

Board deserializeBoard(const uint8_t *buffer, size_t &offset, Board &board);


#endif //BACKGAMMONGAME_BOARD_H
