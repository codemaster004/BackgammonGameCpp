//
// Created by Filip Dabkowski on 12/11/2023.
//

#include "cstring"
#include "cstdint"

#include "Player.h"
#include "Board.h"
#include "SerializeToFile.h"


void changePlayers(Board &game) {
	game.currentPlayerId = getOpponent(game, game.currentPlayerId)->id;
}

Player *getPlayer(Board &game, int playerId) {
	for (auto &player: game.players)
		if (player.id == playerId)
			return &player;

	return nullptr;
}

Player *getOpponent(Board &game, int playerId) {
	for (auto &player: game.players)
		if (player.id != playerId)
			return &player;

	return nullptr;
}

void serialisePlayer(Player player, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &player, sizeof(Player));
	offset += sizeof(Player);
}

void serialisePlayerPointer(Player *player, uint8_t *buffer, size_t &offset) {
	if (player == nullptr) {
		serializeInt(-1, buffer, offset);
	} else {
		serializeInt(player->id, buffer, offset);
	}
}

void serializeScorePlayer(ScorePlayer player, uint8_t *buffer, size_t &offset) {
	std::memcpy(buffer + offset, &player, sizeof(ScorePlayer));
	offset += sizeof(ScorePlayer);
}

Player deserializePlayer(const uint8_t *buffer, size_t &index) {
	Player player;
	std::memcpy(&player, buffer + index, sizeof(Player));
	index += sizeof(Player);
	return player;
}

Player *deserializePlayerPointer(Board &board, const uint8_t *buffer, size_t &offset) {
	int id = deserializeInt(buffer, offset);
	if (id == -1)
		return nullptr;
	for (Player &player: board.players)
		if (player.id == id)
			return &player;
	return nullptr;
}

ScorePlayer deserializeScorePlayer(const uint8_t *buffer, size_t &index) {
	ScorePlayer player;
	std::memcpy(&player, buffer + index, sizeof(ScorePlayer));
	index += sizeof(ScorePlayer);
	return player;
}
