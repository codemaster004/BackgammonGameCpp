//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstring"

#include "../configs/UIConfigs.h"
#include "../configs/MenuConfigs.h"
#include "UserInterface.h"
#include "../view/Handeler.h"
#include "../controller/InputControll.h"

UserInterface initUI() {
	auto ui = UserInterface{
		.gameName="Backgammon",
		.authorId="s197712",
		.authorName="Filip Dabkowski",
		.space=GameSpace{.gameSpace=initGameSpace()},
		.currentMove=initMove(),
		.pickedIndex=-1,
		.history=MoveMade{},
	};
	ui.space.board = initBoard(ui.space.gameSpace);
	Placement board = ui.space.board;
	ui.space.boardCenter = initCenter(board, ui.space.gameSpace);
	ui.space.dice = initDice(ui.space.gameSpace);
	ui.space.indexesTop = initIndex(board, board.min.y - INDEX_OFFSET);
	ui.space.indexesBottom = initIndex(board, board.max.y + INDEX_OFFSET);
	ui.needToRefresh = true;

	return ui;
}

void setBasicGameState(UserInterface &ui) {
	clearDices(ui.board.dices);
	ui.currentMove = {0, 0, 0};
	resetMenuTo(ui, DEFAULT);
	resetMessages(ui);
}

void titleArt(int offsetX, int offsetY) {
	attron(COLOR_PAIR(FOREGROUND));
	mvprintw(offsetY + 0, offsetX, R"( ____             _     ___)") ;
	mvprintw(offsetY + 1, offsetX, R"(| __ )  __ _  ___| |__ / __|  __ _ _ __ ___  _ __ ___   ____  _ __)");
	mvprintw(offsetY + 2, offsetX, R"(|  _ \ / _` |/ __| / // / __ / _` | '_ ` _ \| '_ ` _ \ / __ \| '_ \)");
	mvprintw(offsetY + 3, offsetX, R"(| |_) | (_| | (__|   \\ \_| | (_| | | | | | | | | | | |  \/  | | | |)");
	mvprintw(offsetY + 4, offsetX, R"(|____/ \__,_|\___|_|\_\\___/ \__,_|_| |_| |_|_| |_| |_|\____/|_| |_|)");
	attroff(COLOR_PAIR(FOREGROUND));
}

void starterScreen(UserInterface &ui) {
	titleArt(OFFSET_X, OFFSET_Y + boardHeight / 2);
	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});
}

int calcDiceLimit(UserInterface &ui) {
	int limit = 1;
	for (int i = 0; i < N_DICES; ++i)
		if (ui.board.dices[i] > 0)
			limit += 1;
	if (limit == N_DICES + 1)
		limit += 1;
	return min(9, limit);
}

void createDiceOption(UserInterface ui, char **&options, int index, int value) {
	options[index] = new char[20];
	char *diceNumber = numberToString(index + 1, 1);
	char *diceValue = numberToString(value, 1);

	const char *diceName[4] = {"Dice (", diceNumber, "): ", diceValue};
	joinStrings(options[index], diceName, 4);

	delete[] diceNumber;
	delete[] diceValue;
}

void createDiceMenu(UserInterface &ui, char **&options, char *&keys) {
	int limit = calcDiceLimit(ui);
	ui.menu.count = limit;
	options = new char* [limit];
	keys = new char [limit];

	int index = 0;
	for (int i = 0; i < N_DICES; ++i) {
		if (ui.board.dices[i]) {
			createDiceOption(ui, options, index, ui.board.dices[i]);
			keys[index++] = (char) ('1' + i);
		}
	}

	if (limit == N_DICES + 2) {
		options[limit - 2] = new char[18];
		strcpy(options[limit - 2], "Use all Dices (0)");
		keys[limit - 2] = '0';
	}

	options[limit - 1] = new char[15];
	strcpy(options[limit - 1], "End turn (-)");
	keys[limit - 1] = '-';
}

void initMenuElement(UserInterface &ui, char **options, const char keys[]) {
	ui.menu.elements = new MenuElement[ui.menu.count];
	for (int i = 0; i < ui.menu.count; ++i) {
		ui.menu.elements[i] = MenuElement{.id=i, .key=keys[i], .value=options[i]};
	}
}

void populateOptions(char **&options, const char *origin[], int limit) {
	options = new char *[limit];
	for (int i = 0; i < limit; ++i) {
		uint length = len(origin[i]);
		options[i] = new char[length];
		for (int j = 0; j < length; ++j) {
			options[i][j] = origin[i][j];
		}
	}
}

void populateKeys(char *&keys, const char origin[], int limit) {
	keys = new char [limit];
	for (int i = 0; i < limit; ++i) {
		keys[i] = origin[i];
	}
}

void createOptions(char **&opts, int n, MenuMode mode) {
	const char *basic[] = {"(R)oll", "(U)ndo", "(S)ave", "(Q)uit"};
	const char *start[] = {"(N)ew Game", "(L)oad Game", "(Q)uit"};
	const char *point[] = {"Pick Piece (Space)", "End Turn (-)"};

	switch (mode) {
		case DEFAULT:
			populateOptions(opts, basic, n);
			break;
		case PICK_POINT:
			populateOptions(opts, point, n);
			break;
		case PICK_DICE:
			break;
		case STARTING_GAME:
			populateOptions(opts, start, n);
			break;
	}
}

void createKeys(char *&keys, int n, MenuMode mode) {
	const char basic[] = {'r', 'u', 's', 'q'};
	const char start[] = {'n', 'l', 'q'};
	const char point[] = {' ', '-'};

	switch (mode) {
		case DEFAULT:
			populateKeys(keys, basic, n);
			break;
		case PICK_POINT:
			populateKeys(keys, point, n);
			break;
		case PICK_DICE:
			break;
		case STARTING_GAME:
			populateKeys(keys, start, n);
			break;
	}
}

void redefineMenu(UserInterface &ui) {
	ui.menu.selected = 0;
	char **options = nullptr;
	char *keys = nullptr;

	// TODO: SHORTEN SEPARATE
	switch (ui.menu.mode) {
		case DEFAULT:
			ui.menu.count = N_GAME_MENU_OPTIONS;
			break;
		case STARTING_GAME:
			ui.menu.count = N_STARTER_MENU_OPTIONS;
			break;
		case PICK_POINT:
			ui.menu.count = N_PIECE_MENU_OPTIONS;
			ui.menu.selected = -1;
			ui.pickedIndex = 0;
			break;
		case PICK_DICE:
			createDiceMenu(ui, options, keys);
			break;
	}
	createOptions(options, ui.menu.count, ui.menu.mode);
	createKeys(keys, ui.menu.count, ui.menu.mode);
	// Convert to objects
	initMenuElement(ui, options, keys);
}

Move initMove() {
	return Move{0, 0};
}

void resetMessage(char message[MAX_MESSAGE_LEN]) {
	for (int i = 0; i < MAX_MESSAGE_LEN; ++i) {
		message[i] = ' ';
	}
}

void resetMessages(UserInterface &ui)  {
	resetMessage(ui.infoMess);
	resetMessage(ui.errorMess);
}

void messageSet(char message[MAX_MESSAGE_LEN], const char *newMessage) {
	int length = min(MAX_MESSAGE_LEN - 1, (int)(len(newMessage)));
	for (int i = 0; i < length; ++i) {
		message[i] = newMessage[i];
	}
}
