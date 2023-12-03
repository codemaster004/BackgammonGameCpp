//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstring"

#include "../configs/UIConfigs.h"
#include "../configs/MenuConfigs.h"
#include "UserInterface.h"
#include "../view/Handeler.h"

UserInterface initUI() {
	auto ui = UserInterface{
		.gameName=gameName,
		.authorId=creatorId,
		.authorName=creatorName,
		.space=GameSpace{.gameSpace=initGameSpace()},
		.currentMove=initMove(),
		.pickedIndex=-1,
		.infoMess="",
		.errorMess="",
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
	ui.menu.mode = DEFAULT;
	ui.needToRefresh = true;
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

void joinString(char *&buffer, const char **data, int count) {
	int index = 0;
	for (int i = 0; i < count; ++i) {

		uint length = len(data[i]);
		for (int j = 0; j < length - 1; ++j)
			buffer[index++] = data[i][j];

	}
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
	joinString(options[index], diceName, 4);

	delete[] diceNumber;
	delete[] diceValue;
}

void createDiceMenu(UserInterface &ui, char **&options, char *&keys) {
	int limit = calcDiceLimit(ui);
	ui.menu.elementsCount = limit;
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

void initMenuElement(UserInterface &ui, char **options, const char keys[]) {}

void redefineMenu(UserInterface &ui) {
	ui.menu.selected = 0;
	char **options = nullptr;
	char *keys = nullptr;

	switch (ui.menu.mode) {
		case DEFAULT:
			ui.menu.elementsCount = N_GAME_MENU_OPTIONS;
			options = (char **)(menuGameOptions);
			keys = (char *) (menuGameKeys);
			break;
		case STARTING_GAME:
			ui.menu.elementsCount = N_STARTER_MENU_OPTIONS;
			options = (char **)(menuStarterOptions);
			keys = (char *) (menuStarterKeys);
			break;
		case PICK_POINT:
			ui.menu.elementsCount = N_PIECE_MENU_OPTIONS;
			options = (char **)(menuPieceOptions);
			keys = (char *) (menuPieceKeys);
			ui.menu.selected = -1;
			ui.pickedIndex = 0;
			break;
		case PICK_DICE:
			createDiceMenu(ui, options, keys);
			break;
	}
	// Convert to objects
	initMenuElement(ui, options, keys);
	ui.menu.elements = new MenuElement [ui.menu.elementsCount];
	for (int i = 0; i < ui.menu.elementsCount; ++i) {
		ui.menu.elements[i] = MenuElement {.id=i, .key=keys[i], .value=options[i]};
	}
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
