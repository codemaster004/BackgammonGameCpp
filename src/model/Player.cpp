//
// Created by Filip Dabkowski on 12/11/2023.
//

#include "cstring"
#include "cstdint"

#include "Player.h"
#include "Board.h"
#include "SerializeToFile.h"


// PRIVATE FUNCTION HEADERS //

// PUBLIC FUNCTION DECLARATIONS //

void changePlayers(Board &game) {
	// Change the current player to the opponent
	game.currentPlayerId = getOpponent(game, game.currentPlayerId)->id;
}

Player *getPlayer(Board &game, int playerId) {
	// Iterate through players to find the one with the matching ID
	for (auto &player: game.players)
		if (player.id == playerId)
			return &player;

	// Return nullptr if no matching player is found
	return nullptr;
}

Player *getOpponent(Board &game, int playerId) {
	// Iterate through players to find the opponent
	for (auto &player: game.players)
		if (player.id != playerId)
			return &player;

	// Since this is a two-player game, this condition will always be met
	return nullptr;
}

void serialisePlayer(Player player, uint8_t *buffer, size_t &offset) {
	// Copy the player structure into the buffer
	std::memcpy(buffer + offset, &player, sizeof(Player));
	offset += sizeof(Player);
}

void serialisePlayerPointer(Player *player, uint8_t *buffer, size_t &offset) {
	// Serialize the player ID or -1 if the pointer is null
	if (player == nullptr) {
		serializeInt(-1, buffer, offset);
	} else {
		serializeInt(player->id, buffer, offset);
	}
}

void serializeScorePlayer(ScorePlayer player, uint8_t *buffer, size_t &offset) {
	// Copy the ScorePlayer structure into the buffer
	std::memcpy(buffer + offset, &player, sizeof(ScorePlayer));
	offset += sizeof(ScorePlayer);
}

Player deserializePlayer(const uint8_t *buffer, size_t &index) {
	Player player;
	// Copy data from the buffer into the player structure
	std::memcpy(&player, buffer + index, sizeof(Player));
	index += sizeof(Player);
	return player;
}

Player *deserializePlayerPointer(Board &board, const uint8_t *buffer, size_t &offset) {
	// Deserialize the player ID
	int id = deserializeInt(buffer, offset);

	// Return nullptr if ID is -1, indicating no player
	if (id == -1)
		return nullptr;

	// Find and return the player with the deserialized ID
	for (Player &player: board.players)
		if (player.id == id)
			return &player;

	return nullptr;
}

ScorePlayer deserializeScorePlayer(const uint8_t *buffer, size_t &index) {
	ScorePlayer player;
	// Copy data from the buffer into the ScorePlayer structure
	std::memcpy(&player, buffer + index, sizeof(ScorePlayer));
	index += sizeof(ScorePlayer);
	return player;
}

// PRIVATE FUNCTION DECLARATIONS //
