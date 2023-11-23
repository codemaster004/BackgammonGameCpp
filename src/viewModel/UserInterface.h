//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_USERINTERFACE_H
#define BACKGAMMONGAME_USERINTERFACE_H

#include "string"
#include "../configs/GameConfigs.h"
#include "../model/Board.h"
#include "Space.h"

enum MenuMode {
	DEFAULT,
	PICK_POINT,
};

typedef struct {
	Placement board;
	Placement dice;
	Pos boardCenter;
	Placement indexesTop;
	Placement indexesBottom;
} GameSpace;

typedef struct {
	std::string gameName;
	std::string authorName;
	int authorId;
	Board board;
	// TODO: create/implement structure for keeping currentPlayersScores
	int currentScores[PLAYERS_PER_GAME];
	MenuMode menuMode;
	GameSpace space;
} UserInterface;

typedef struct {
	int id;
	const char *value;
} MenuElement;

UserInterface initUI();

#endif //BACKGAMMONGAME_USERINTERFACE_H
