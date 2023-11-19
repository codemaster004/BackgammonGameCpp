//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstdlib"

#include "configs/GameConfigs.h"
#include "model/Board.h"

void handleGame(Board *game) {
	setClearBoard(game);

	// TODO: move this somewhere where it makes sense
	Pawn white[PAWNS_PER_PLAYER] = {};
	Pawn black[PAWNS_PER_PLAYER] = {};
	for (int i = 0; i < PAWNS_PER_PLAYER; ++i) {
		white[i] = Pawn{.id=i, .isHome=false, .isOnBar=false, .color=WHITE, .moveDirection=1};
		black[i] = Pawn{.id=PAWNS_PER_PLAYER + i, .isHome=false, .isOnBar=false, .color=BLACK, .moveDirection=-1};
	}
	setupGame(game, white, black);
}


void inputController(int input, int *menu, bool *gameEnded, int *dice1, int *dice2) {
	switch (input) {
		// TODO: Probably rewrite for more options
		case '\n':
		case '\r':
			inputController((int) (menuKeys[*menu]), menu, gameEnded, dice1, dice2);
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
			*dice1 = rand() % 6 + 1;
			*dice2 = rand() % 6 + 1;
			break;
		case 'q':
			*gameEnded = true;
			break;
		default:
			break;
	}
}