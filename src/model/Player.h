//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PLAYER_H
#define BACKGAMMONGAME_PLAYER_H

#include "../configs/GameConfigs.h"
#include "Pawn.h"


typedef struct Board Board;

/**
 * @brief Represents a player in the game.
 *
 * This structure holds information about a player, including their ID, name, color of
 * their pawns, and the points they have accumulated in the game.
 */
typedef struct Player {
	int id;                   ///< Unique identifier for the player.
	char name[MAX_NAME_LENGTH]; ///< Name of the player.
	PawnColor color;         ///< Color of the player's pawns.
	int points;              ///< Points accumulated by the player.
} Player;


void changePlayers(Board &game);

/// Handle Serialization of Player object
void serialisePlayer(Player player, uint8_t *buffer, size_t &offset);

Player deserializePlayer(const uint8_t *buffer, size_t &index);

/// Handle POINTER serialization to a Player object
void serialisePlayerPointer(Player *player, uint8_t *buffer, size_t &offset);

Player *deserializePlayerPointer(Board &board, const uint8_t *buffer, size_t &offset);


#endif //BACKGAMMONGAME_PLAYER_H
