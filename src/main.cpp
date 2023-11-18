//
// Created by Filip Dabkowski on 12/11/2023.
// Full Project can be vied online at https://github.com/codemaster004/BackgammonGameCpp
//

#include <ncurses.h>

#include "ViewHandeler.h"
#include "configs/UIConfigs.h"
#include "Controller.h"

// TODO: Some main while loop for handling user interaction
int main(int argc, char **argv) {
	initscr();
	cbreak();
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);

	start_color();

	// TODO: CLEAN UP!!!
	setColourTheme(0, 200, 0);

	// TODO: SEparete function
	init_pair(FOREGROUND, COLOUR_MAIN, COLOR_BLACK);
	init_pair(FOREGROUND_LIGHT, COLOUR_MAIN_LIGHT, COLOR_BLACK);
	init_pair(FOREGROUND_DARK, COLOUR_MAIN_DARK, COLOR_BLACK);
	init_pair(BACKGROUND_LIGHT, COLOR_BLACK, COLOUR_MAIN_LIGHT);
	init_pair(BACKGROUND_DARK, COLOR_BLACK, COLOUR_MAIN_DARK);

	int menuSelected = 0;

	uiStaff(&menuSelected);

	int ch;
	bool gameEnded = false;
	while((ch = getch()) != 'q') {
		inputController(ch, &menuSelected, &gameEnded);
		if (gameEnded)
			break;

		uiStaff(&menuSelected);
	}

	// End curses mode
	endwin();

	return 0;
}

/**
 * TODO: [ ] Basic Board UI (1pt)
 * TODO: [ ] Visualisation of game state (1pt)
 * TODO: [ ] Saving game state to a file (2pt)
 * TODO: [ ] Loading game state from file + ?next move? (2pt)
 * TODO: [ ] Ability to capture pawns (1pt)
 * TODO: [ ] Removing pawns ?bar or court? (1pt)
 * TODO: [ ] ?Game simulation from any point till the end? (2pt)
 * TODO: [ ] Remember and save game past moves (1pt)
 * TODO: [ ] Visualisation of game play (backwards) from loaded state (2pt)
 * TODO: [ ] Complete UI (1pt)
 * TODO: [ ] Player Rankings "Hall of Fame" (1pt)
 * use enum somewhere...
 */
