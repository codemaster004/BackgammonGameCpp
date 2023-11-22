//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstdlib"

#include "configs/GameConfigs.h"
#include "model/Board.h"
#include "Controller.h"
#include "model/Pawn.h"


void handleGame(Board *game) {
	setClearBoard(game);

	// TODO: move this somewhere where it makes sense
	Pawn white[PAWNS_PER_PLAYER] = {};
	Pawn black[PAWNS_PER_PLAYER] = {};
	int player1 = 1;
	int player2 = 2;
	for (int i = 0; i < PAWNS_PER_PLAYER; ++i) {
		white[i] = Pawn{.owner=&player1, .id=i, .isHome=false, .isOnBar=false, .color=WHITE, .moveDirection=1};
		black[i] = Pawn{.owner=&player2, .id=PAWNS_PER_PLAYER + i, .isHome=false, .isOnBar=false, .color=BLACK, .moveDirection=-1};
	}
	setupGame(game, white, black);

	// TODO: SEPARATE
//	game->currentPlayer = &player1;
//	int dice1 = 2, dice2 = 5;
//	movePawn(game, 0, dice1);
//	movePawn(game, 0, dice1);
//	game->currentPlayer = &player2;
}

void numberInputController(int input, int *inputtedNumber) {
	if (input >= '0' && input <= '9') {
		*inputtedNumber *= 10;
		*inputtedNumber += (int) (input - '0');
	}
}

void inputController(int input, Board *game, int *menu, bool *gameEnded, int *dice1, int *dice2, int *inputtedNumber) {
//	numberInputController(input, inputtedNumber);
//	if (input == '\r' || input == '\n') {
//		movePawn(game, *inputtedNumber, *dice1);
//		*inputtedNumber = 0;
//	}
	switch (input) {
		// TODO: Probably rewrite for more options
		case '\n':
		case '\r':
			*dice1 = 0; // TODO: RANDOM
			inputController((int) (menuKeys[*menu]), nullptr, menu, gameEnded, dice1, dice2, nullptr);
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
			*dice1 += 1; // TODO: RANDOM
			*dice2 = rand() % 6 + 1;
			break;
		case 'q':
			*gameEnded = true;
			break;
		default:
			break;
	}
}

// TODO: N pawns to move
// TODO: maybe return 0 and 1 for eero check
void movePawn(Board *game, int fromIndex, int moveBy) {
	// TODO: Separate
	if (!canBeMoved(game, fromIndex, moveBy)) {
		return;
	}
	int toIndex = fromIndex + moveBy;
	moveToPoint moveType = canMoveTo(game, fromIndex, toIndex);
	if (!enumToBool(moveType)) {
		return;
	}
	Point *toPoint = &game->points[toIndex];
	Point *fromPoint = &game->points[fromIndex];
	// TODO: Separate
	if (moveType == CAPTURE) {
		for (int i = 0; i < CAPTURE_THRESHOLD; ++i) {
			game->bar.pawns[game->bar.pawnsInside++] = toPoint->pawns[i];
		}
		toPoint->pawnsInside -= CAPTURE_THRESHOLD;
	}
	toPoint->pawns[toPoint->pawnsInside++] = fromPoint->pawns[--fromPoint->pawnsInside];
	fromPoint->pawns[fromPoint->pawnsInside] = nullptr;
}
