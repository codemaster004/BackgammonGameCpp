//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PLAYER_H
#define BACKGAMMONGAME_PLAYER_H

#include "../configs/GameConfigs.h"


typedef struct Board Board;

typedef struct Player {
	int id;
	char name[MAX_NAME_LENGTH];
	bool isHisTurn;
	int points;
} Player;

/// Handle Serialization of Player object
void serialisePlayer(Player player, uint8_t *buffer, size_t &offset);

Player deserializePlayer(const uint8_t *buffer, size_t &index);

/// Handle POINTER serialization to a Player object
void serialisePlayerPointer(Player *player, uint8_t *buffer, size_t &offset);

Player *deserializePlayerPointer(Board &board, const uint8_t *buffer, size_t &offset);


#endif //BACKGAMMONGAME_PLAYER_H
