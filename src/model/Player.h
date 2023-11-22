//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PLAYER_H
#define BACKGAMMONGAME_PLAYER_H

// TODO: Use char
#include "string"

#include "../configs/GameConfigs.h"


typedef struct {
	int id;
	std::string name;
	bool isHisTurn;
	uint points;
	int *numbersRolled[N_DICES];
} Player;

#endif //BACKGAMMONGAME_PLAYER_H
