//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstdlib"

#include "../configs/GameConfigs.h"
#include "../model/Board.h"
#include "InputControll.h"
#include "../model/SerializeToFile.h"


void gameSetUp(Board &game) {
	game.players[0] = Player {
		0, {"Me"}, PAWN_WHITE, true
	};
	game.players[1] = Player {
		1, {"You"}, PAWN_BLACK, false
	};
//	game.currentPlayerId = rand() % 2;
	game.currentPlayerId = 0;
	for (int & dice : game.dices) {
		dice = 0;
	}
	for (int i = 0; i < N_PLAYERS; ++i) {
		game.courts[i].owner = &game.players[i];
	}

	for (int i = 0; i < PAWNS_PER_PLAYER; ++i) {
		game.pawns[i] = Pawn{.ownerId=game.players[0].id, .id=i, .isHome=false, .color=PAWN_WHITE, .moveDirection=1};
		game.pawns[i + PAWNS_PER_PLAYER] = Pawn{.ownerId=game.players[1].id, .id=PAWNS_PER_PLAYER + i, .isHome=false, .color=PAWN_BLACK, .moveDirection=-1};
	}
	placePawns(game);
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
	ui.pickedIndex = -1;
	resetMessages(ui);
}


void newGameController(int input, UserInterface &ui) {
	char name[] = "game0.txt";
	switch (input) {
		case 'n':
			ui.state = GAME_PLAY;
			resetMenuTo(ui, DEFAULT);
			break;
		case 'l':
			ui.state = GAME_PLAY;
			resetMenuTo(ui, DEFAULT);
			loadFromFile(name, ui.board);
			loadHistoryFromFile(name, ui.history);
			break;
		default:
			break;
	}
}

void gamePlayController(int input, UserInterface &ui) {
	char name[] = "game0.txt";
	switch (input) {
		case 's':
			saveToFile(name, ui.board);
			saveHistoryToFile(name, ui.history);
			break;
		case 'u':
			reverseMove(ui.board, ui.history);
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

void createDiceMessage(UserInterface &ui) {
	int digits = (int)(nDigits(ui.currentMove.by, 10));
	const char *moveBy = numberToString(ui.currentMove.by, digits);
	const char *str = joinStrings("Move by: ", 9, moveBy, digits);
	messageSet(ui.infoMess, str);
	delete[] str;
}

void createErrorMessage(UserInterface &ui, MoveStatus errorId) {
	if (errorId == MOVE_FAILED) {
		messageSet(ui.errorMess, "Invalid Point");
	} else if (errorId == PAWNS_ON_BAR) {
		messageSet(ui.errorMess, "Pawns on Bar");
	}
}

void pickDiceController(int input, UserInterface &ui) {
	// TODO: rewrite for N dices
	int *dice = ui.board.dices;
	switch (input) {
		case '1':
			ui.currentMove.by = dice[0];
			ui.currentMove.movesLeft = 1;
			dice[0] = 0;
			resetMenuTo(ui, PICK_POINT);
			break;
		case '2':
			ui.currentMove.by = dice[1];
			ui.currentMove.movesLeft = 1;
			dice[1] = 0;
			resetMenuTo(ui, PICK_POINT);
			break;
		case '0':
			ui.currentMove.by = ui.board.dices[0] + ui.board.dices[1];
			if (dice[0] == dice[1]) {
				ui.currentMove.movesLeft = 4;
				ui.currentMove.by = dice[0];
			} else {
				ui.currentMove.movesLeft = 1;
			}
			dice[0] = 0;
			dice[1] = 0;
			resetMenuTo(ui, PICK_POINT);
			break;
		case '-':
			setBasicGameState(ui);
			clearDices(ui.board.dices);
			changePlayers(ui.board);
			break;
		default:
			break;
	}
	if (ui.currentMove.by != 0) {
		createDiceMessage(ui);
	}
}

void pickPointController(int input, UserInterface &ui) {
	MoveStatus moveError;
	switch (input) {
		case ' ':
			ui.currentMove.from = ui.pickedIndex;
			moveError = handlePawnMovement(ui.board, ui.currentMove, ui.history);
			if(moveError) {
				createErrorMessage(ui, moveError);
			} else if (!--ui.currentMove.movesLeft) {
				resetMenuTo(ui, PICK_DICE);
			} else {
				resetMenuTo(ui, PICK_POINT);
			}
			break;
		case '-':
			ui.currentMove.from = ui.pickedIndex;
			ui.currentMove = {0, 0};
			resetMenuTo(ui, PICK_DICE);
			break;
		case KEY_BACKSPACE:
		case 0x7F:
			ui.pickedIndex /= 10;
			break;
		default:
			numberInputController(input, ui.pickedIndex);
			ui.pickedIndex %= (nPoints + 1);
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
	resetMessage(ui.errorMess);
	switch (input) {
		case '\n':
		case '\r':
			if (ui.menu.selected >= 0) {
				inputController(ui.menu.elements[ui.menu.selected].key, ui);
			}
			break;
		case KEY_UP:
			if (ui.menu.mode == PICK_POINT) {
				ui.pickedIndex = newSelected(ui.pickedIndex, 1, nPoints + 1);
			}
			break;
		case KEY_DOWN:
			if (ui.menu.mode == PICK_POINT) {
				ui.pickedIndex = newSelected(ui.pickedIndex, -1, nPoints + 1);
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
