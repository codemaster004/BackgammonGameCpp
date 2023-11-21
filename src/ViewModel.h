//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_VIEWMODEL_H
#define BACKGAMMONGAME_VIEWMODEL_H

#include "string"
#include "model/Board.h"

enum MenuMode {
	DEFAULT,
	PICK_POINT,
};

typedef struct {
	std::string gameName;
	std::string authorName;
	int authorId;
	Board board;
	// TODO: create/implement structure for keeping currentPlayersScores
	int currentScores[PLAYERS_PER_GAME];
	MenuMode menuMode;
} UserInterface;

typedef struct {
	int id;
	const char *value;
} MenuElement;

#endif //BACKGAMMONGAME_VIEWMODEL_H
