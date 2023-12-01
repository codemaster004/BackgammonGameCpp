//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstdlib"

#include "../configs/GameConfigs.h"
#include "../model/Board.h"
#include "InputControll.h"


void gameSetUp(Board &game) {
	game.players[0] = Player {
		0, {"Me"}, true, 0
	};
	game.players[1] = Player {
		1, {"You"}, false, 0
	};
	game.currentPlayerId = 0;
	for (int & dice : game.dices) {
		dice = 0;
	}

	for (int i = 0; i < PAWNS_PER_PLAYER; ++i) {
		game.pawns[i] = Pawn{.ownerId=game.players[0].id, .id=i, .isHome=false, .isOnBar=false, .color=PAWN_WHITE, .moveDirection=1};
		game.pawns[i + PAWNS_PER_PLAYER] = Pawn{.ownerId=game.players[1].id, .id=PAWNS_PER_PLAYER + i, .isHome=false, .isOnBar=false, .color=PAWN_BLACK, .moveDirection=-1};
	}
	placePawns(game);

	// TODO: SEPARATE
//	game->currentPlayerId = &player1;
//	int dice1 = 2, dice2 = 5;
//	movePawn(game, 0, dice1);
//	movePawn(game, 0, dice1);
//	game->currentPlayerId = &player2;
}

void numberInputController(int input, int &inputtedNumber) {
	if (input >= '0' && input <= '9') {
		inputtedNumber *= 10;
		inputtedNumber += (int) (input - '0');
	}
}


void resetMenuTo(UserInterface &ui, MenuMode mode) {
	ui.menu.mode = mode;
	ui.needToRefresh = true;
	ui.menu.selected = 0;
}


void newGameController(int input, UserInterface &ui) {
	switch (input) {
		case 'n':
			ui.state = GAME_PLAY;
			resetMenuTo(ui, DEFAULT);
			break;
		case 'l':
			break;
		default:
			break;
	}
}

void gamePlayController(int input, UserInterface &ui) {
	switch (input) {
		case 'm':
			break;
		case 'u':
			break;
		case 'r':
			for (int & dice : ui.board.dices) {
				dice = rand() % 6 + 1;
			}
			resetMenuTo(ui, PICK_DICE);
			break;
		default:
			break;
	}
}

void pickDiceController(int input, UserInterface &ui) {
	// TODO: rewrite for N dices
	switch (input) {
		case '1':
			ui.currentMove.by = ui.board.dices[0];
			ui.board.dices[0] = 0;
			resetMenuTo(ui, PICK_POINT);
			break;
		case '2':
			ui.currentMove.by = ui.board.dices[1];
			ui.board.dices[1] = 0;
			resetMenuTo(ui, PICK_POINT);
			break;
		case '0':
			ui.currentMove.by = ui.board.dices[0] + ui.board.dices[1];
			ui.board.dices[0] = 0;
			ui.board.dices[1] = 0;
			resetMenuTo(ui, PICK_POINT);
			break;
		case '-':
			break;
		default:
			break;
	}
}

void handleMenuModes(int input, UserInterface &ui) {
	switch (ui.menu.mode) {
		case DEFAULT:
			gamePlayController(input, ui);
			break;
		case PICK_POINT:
			break;
		case PICK_DICE:
			pickDiceController(input, ui);
			break;
		case STARTING_GAME:
			newGameController(input, ui);
			break;
	}
}

void inputController(int input, UserInterface &ui, int &inputtedNumber) {
//	numberInputController(input, inputtedNumber);
//	if (input == '\r' || input == '\n') {
//		movePawn(game, *inputtedNumber, *dice1);
//		*inputtedNumber = 0;
//	}
	switch (input) {
		case '\n':
		case '\r':
			inputController(ui.menu.elements[ui.menu.selected].key, ui, inputtedNumber);
			break;
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			ui.menu.selected = ui.menu.selected - 1;
			if (ui.menu.selected < 0)
				ui.menu.selected = ui.menu.elementsCount - 1;
			ui.menu.selected = ui.menu.selected % ui.menu.elementsCount;
			break;
		case KEY_RIGHT:
			ui.menu.selected = ui.menu.selected + 1;
			ui.menu.selected = ui.menu.selected % ui.menu.elementsCount;
			break;
		default:
			if (ui.menu.elements[ui.menu.selected].key == 'q') {
				ui.gameEnded = true;
			} else {
				handleMenuModes(input, ui);
			}
			break;
	}

}

// TODO: N pawnsId to move
// TODO: maybe return 0 and 1 for eero check
void movePawn(Board &game, int fromIndex, int moveBy) {
	moveToPoint moveType = determineMoveType(game, fromIndex, moveBy);
	if (!enumToBool(moveType)) {
		return;
	}
	int toIndex = fromIndex + moveBy;
	Point *toPoint = &game.points[toIndex];
	Point *fromPoint = &game.points[fromIndex];
	if (moveType == CAPTURE) {
		pawnCapture(game, toPoint);
	}
	toPoint->pawns[toPoint->pawnsInside++] = fromPoint->pawns[--fromPoint->pawnsInside];
	fromPoint->pawns[fromPoint->pawnsInside] = nullptr;
}
