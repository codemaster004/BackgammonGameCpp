//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PLAYER_H
#define BACKGAMMONGAME_PLAYER_H

#include "../configs/GameConfigs.h"
#include "../configs/UIConfigs.h"


typedef struct {
	int id;
	char name[MAX_NAME_LENGTH];
	bool isHisTurn;
	uint points;
} Player;

#endif //BACKGAMMONGAME_PLAYER_H
