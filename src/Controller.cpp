//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"

#include "configs/GameConfigs.h"

void inputController(int input, int *menu, bool *gameEnded) {
	switch(input) {
		// TODO: Probably rewrite for more options
		case '\n':
		case '\r':
			inputController((int) (menuKeys[*menu]), menu, gameEnded);
			break;
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			*menu = *menu - 1;
			if (*menu < 0)
				*menu = N_MENU_OPTIONS - 1;
			*menu = *menu % N_MENU_OPTIONS;
			break;
		case KEY_RIGHT:
			*menu = *menu + 1;
			*menu = *menu % N_MENU_OPTIONS;
			break;
		case 'm':
			break;
		case 'u':
			break;
		case 'r':
			break;
		case 'q':
			*gameEnded = true;
			break;
		default:
			break;
	}
}