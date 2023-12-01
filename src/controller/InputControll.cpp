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

void pickPointController(int input, UserInterface &ui) {
	switch (input) {
		case ' ':
			ui.currentMove.from = ui.pickedIndex;
			movePawn(ui.board, ui.currentMove);
			break;
		case '-':
			break;
		case KEY_BACKSPACE:
		case 0x7F:
			ui.pickedIndex /= 10;
			break;
		default:
			numberInputController(input, ui.pickedIndex);
			ui.pickedIndex %= nPoints;
			break;
	}
}

void handleMenuModes(int input, UserInterface &ui) {
	switch (ui.menu.mode) {
		case DEFAULT:
			gamePlayController(input, ui);
			break;
		case PICK_POINT:
			pickPointController(input, ui);
			break;
		case PICK_DICE:
			pickDiceController(input, ui);
			break;
		case STARTING_GAME:
			newGameController(input, ui);
			break;
	}
}

int newSelected(int selected, int change, int count) {
	selected = selected + change;
	if (selected < 0)
		selected = count - 1;
	return selected % count;
}

void inputController(int input, UserInterface &ui) {
	switch (input) {
		case '\n':
		case '\r':
			inputController(ui.menu.elements[ui.menu.selected].key, ui);
			break;
		case KEY_UP:
			if (ui.menu.mode == PICK_POINT) {
				ui.pickedIndex = newSelected(ui.pickedIndex, 1, nPoints);
			}
			break;
		case KEY_DOWN:
			if (ui.menu.mode == PICK_POINT) {
				ui.pickedIndex = newSelected(ui.pickedIndex, -1, nPoints);
			}
			break;
		case KEY_LEFT:
			if (ui.menu.mode != PICK_POINT) {
				ui.menu.selected = newSelected(ui.menu.selected, -1, ui.menu.elementsCount);
			}
			break;
		case KEY_RIGHT:
			if (ui.menu.mode != PICK_POINT) {
				ui.menu.selected = newSelected(ui.menu.selected, 1, ui.menu.elementsCount);
			}
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
void movePawn(Board &game, Move move) {
	moveToPoint moveType = determineMoveType(game, move.from, move.by);
	if (!enumToBool(moveType)) {
		return;
	}
	int toIndex = move.from + move.by * game.points[move.from].pawns[0]->moveDirection;
	Point *toPoint = &game.points[toIndex];
	Point *fromPoint = &game.points[move.from];
	if (moveType == CAPTURE) {
		pawnCapture(game, toPoint);
	}
	toPoint->pawns[toPoint->pawnsInside++] = fromPoint->pawns[--fromPoint->pawnsInside];
	fromPoint->pawns[fromPoint->pawnsInside] = nullptr;
}
