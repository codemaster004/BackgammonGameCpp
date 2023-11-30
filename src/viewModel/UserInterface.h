//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_USERINTERFACE_H
#define BACKGAMMONGAME_USERINTERFACE_H

#include "../configs/GameConfigs.h"
#include "../model/Board.h"
#include "Space.h"

enum MenuMode {
	DEFAULT,
	PICK_POINT,
	PICK_DICE,
	STARTING_GAME,
};

enum UiState {
	WELCOME_SCREEN,
	PICK_SAVE,
	GAME_PLAY
};

typedef struct {
	Placement board;
	Placement dice;
	Pos boardCenter;
	Placement indexesTop;
	Placement indexesBottom;
} GameSpace;

typedef struct {
	int id;
	char key;
	const char *value;
} MenuElement;

typedef struct Menu {
	MenuMode mode;
	MenuElement *elements;
	int elementsCount;
	int selected;
} Menu;

typedef struct {
	const char *gameName;
	const char *authorId;
	const char *authorName;
	Board board;
	// TODO: create/implement structure for keeping currentPlayersScores
	int currentScores[PLAYERS_PER_GAME];
	Menu menu;
	GameSpace space;
	bool needToRefresh;
	bool gameEnded;
	UiState state;
} UserInterface;

UserInterface initUI();

void starterScreen(UserInterface &ui);

void titleArt(int offsetX, int offsetY);

void createStarterMenu(Menu &menu);

void redefineMenu(UserInterface &ui);

#endif //BACKGAMMONGAME_USERINTERFACE_H
