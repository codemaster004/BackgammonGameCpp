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
	int points;               ///< Points accumulated by the player.
} Player;


/**
 * @brief Represents players' saved scores.
 *
 * This structure holds information about players score, includes ID, nick,
 * and their overall score
 */
typedef struct ScorePlayer {
	int id;                   ///< Unique identifier for the player.
	char name[MAX_NAME_LENGTH]; ///< Name of the player.
	int points;               ///< Points accumulated by the player.
} ScorePlayer;


void changePlayers(Board &game);

Player *getPlayer(Board &game, int playerId);

Player *getOpponent(Board &game, int playerId);

/// Handle Serialization of Player object
void serialisePlayer(Player player, uint8_t *buffer, size_t &offset);

Player deserializePlayer(const uint8_t *buffer, size_t &index);

/// Handle POINTER serialization to a Player object
void serialisePlayerPointer(Player *player, uint8_t *buffer, size_t &offset);

Player *deserializePlayerPointer(Board &board, const uint8_t *buffer, size_t &offset);

void serializeScorePlayer(ScorePlayer player, uint8_t *buffer, size_t &offset);

ScorePlayer deserializeScorePlayer(const uint8_t *buffer, size_t &index);

#endif //BACKGAMMONGAME_PLAYER_H
