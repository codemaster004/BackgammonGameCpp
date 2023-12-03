//
// Created by Filip Dabkowski on 12/11/2023.
// Full Project can be vied online at https://github.com/codemaster004/BackgammonGameCpp
//

#include "ncurses.h"
#include "cstdio"
#include "cstdlib"
#include "ctime"

#include "configs/UIConfigs.h"
#include "model/Board.h"
#include "controller/InputControll.h"
#include "view/Handeler.h"
#include "view/Drawing.h"
#include "model/SerializeToFile.h"

void startScreen(int argc, char **argv);

int getInput(int argc, char **argv);

int main(int argc, char **argv) {
	startScreen(argc, argv);

	UserInterface UI = initUI();
	UI.board = Board{};
	setClearBoard(UI.board);

	gameSetUp(UI.board);

	if (DEBUG_MODE) {
		UI.state = GAME_PLAY;
		UI.menu.mode = DEFAULT;
	} else {
		UI.state = WELCOME_SCREEN;
		UI.menu.mode = STARTING_GAME;
	}
	redefineMenu(UI);

	int ch = 0;
	UI.gameEnded = false;
	do {
		inputController(ch, UI);
		if (UI.gameEnded)
			break;

		if (UI.needToRefresh) {
			delete[] UI.menu.elements;
			clear();
			redefineMenu(UI);
			UI.needToRefresh = false;
		}

		if (UI.state == GAME_PLAY) {
			generateBasicBoard(UI);
			generateInteractiveUI(UI);
		} else if (UI.state == WELCOME_SCREEN) {
			starterScreen(UI);
		}

		// Refresh the screen to show changes
		refresh();
	} while ((ch = getInput(argc, argv)) != 'q');
	 delete[] UI.menu.elements;

	// Finish curses mode
	endwin();

	return 0;
}

int getInput(int argc, char **argv) {
	if (argc > 1)
		return getch();
	else
		return getchar();
}

void startScreen(int argc, char **argv) {
	if (argc > 1)
		initscr();
	cbreak();
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);

	srand(time(nullptr));

	start_color();

	// TODO: CLEAN UP!!!
	setColourTheme(COLOR_THEME);

	// TODO: SEparete function
	init_pair(FOREGROUND, COLOUR_MAIN, COLOR_BLACK);
	init_pair(FOREGROUND_LIGHT, COLOUR_MAIN_LIGHT, COLOR_BLACK);
	init_pair(FOREGROUND_DARK, COLOUR_MAIN_DARK, COLOR_BLACK);
	init_pair(BACKGROUND_LIGHT, COLOR_BLACK, COLOUR_MAIN_LIGHT);
	init_pair(BACKGROUND_DARK, COLOR_BLACK, COLOUR_MAIN_DARK);
}

/**
 * TODO: [x] Basic Board UI (1pt)
 * TODO: [x] Visualisation of game state (1pt)
 * TODO: [x] Saving game state to a file (2pt)
 * TODO: [x] Loading game state from file + ?next move? (2pt)
 * TODO: [ ] Ability to capture pawnsId (1pt)
 * TODO: [ ] Removing pawnsId ?bar or court? (1pt)
 * TODO: [ ] ?Game simulation from any point till the end? (2pt)
 * TODO: [ ] Remember and save game past moves (1pt)
 * TODO: [ ] Visualisation of game play (backwards) from loaded state (2pt)
 * TODO: [ ] Complete UI (1pt)
 * TODO: [ ] Player Rankings "Hall of Fame" (1pt)
 */
