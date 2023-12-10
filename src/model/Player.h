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


/**
 * @brief Changes the current player to the opponent.
 *
 * @param game Reference to the Board structure representing the game state.
 */
void changePlayers(Board &game);

/**
 * @brief Retrieves a pointer to a Player structure based on its ID.
 *
 * @param game Reference to the Board structure.
 * @param playerId The ID of the player to retrieve.
 * @return Player* Pointer to the Player structure, or nullptr if not found.
 */
Player *getPlayer(Board &game, int playerId);

/**
 * @brief Retrieves a pointer to the opponent Player structure.
 *
 * @param game Reference to the Board structure.
 * @param playerId The ID of the current player.
 * @return Player* Pointer to the opponent Player structure.
 */
Player *getOpponent(Board &game, int playerId);

/**
 * @brief Serializes a Player structure into a buffer.
 *
 * @param player The Player structure to serialize.
 * @param buffer Pointer to the buffer for storing serialized data.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialisePlayer(Player player, uint8_t *buffer, size_t &offset);

/**
 * @brief Serializes a pointer to a Player structure into a buffer.
 *
 * @param player Pointer to the Player structure to serialize.
 * @param buffer Pointer to the buffer for storing serialized data.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serialisePlayerPointer(Player *player, uint8_t *buffer, size_t &offset);

/**
 * @brief Serializes a ScorePlayer structure into a buffer.
 *
 * @param player The ScorePlayer structure to serialize.
 * @param buffer Pointer to the buffer for storing serialized data.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 */
void serializeScorePlayer(ScorePlayer player, uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes a Player structure from a buffer.
 *
 * @param buffer Pointer to the buffer containing serialized data.
 * @param index Reference to the size_t variable indicating the current index in the buffer.
 * @return Player The deserialized Player structure.
 */
Player deserializePlayer(const uint8_t *buffer, size_t &index);

/**
 * @brief Deserializes a pointer to a Player structure from a buffer.
 *
 * @param board Reference to the Board containing Player structures.
 * @param buffer Pointer to the buffer containing serialized data.
 * @param offset Reference to the size_t variable indicating the current offset in the buffer.
 * @return Player* Pointer to the deserialized Player structure, or nullptr if not found.
 */
Player *deserializePlayerPointer(Board &board, const uint8_t *buffer, size_t &offset);

/**
 * @brief Deserializes a ScorePlayer structure from a buffer.
 *
 * @param buffer Pointer to the buffer containing serialized data.
 * @param index Reference to the size_t variable indicating the current index in the buffer.
 * @return ScorePlayer The deserialized ScorePlayer structure.
 */
ScorePlayer deserializeScorePlayer(const uint8_t *buffer, size_t &index);

#endif //BACKGAMMONGAME_PLAYER_H
