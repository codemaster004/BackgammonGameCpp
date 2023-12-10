//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstdlib"
#include "cstring"

#include "../configs/GameConfigs.h"
#include "../model/Board.h"
#include "InputControll.h"
#include "../model/SerializeToFile.h"

// PRIVATE FUNCTION HEADERS //

// PUBLIC FUNCTION DECLARATIONS //

// PRIVATE FUNCTION DECLARATIONS //

void gameSetUp(Board &game) {
	game.currentPlayerId = game.players[rand() % N_PLAYERS].id;
	for (int &dice: game.dices) {
		dice = 0;
	}
	for (int i = 0; i < N_PLAYERS; ++i) {
		game.courts[i].owner = &game.players[i];
	}

	for (int i = 0; i < PAWNS_PER_PLAYER; ++i) {
		game.pawns[i] = Pawn{.ownerId=game.players[0].id, .id=PAWNS_PER_PLAYER + i, .color=PAWN_WHITE, .moveDirection=1};
		game.pawns[i + PAWNS_PER_PLAYER] = Pawn{.ownerId=game.players[1].id, .id=i, .color=PAWN_BLACK, .moveDirection=-1};
	}
	placePawns(game);
}

void numberInputController(int input, int &inputtedNumber) {
	if (input >= '0' && input <= '9') {
		inputtedNumber *= 10;
		inputtedNumber += (int) (input - '0');
	}
}

int newSelected(int selected, int change, int count) {
	selected = selected + change;
	if (selected < 0)
		selected = count - 1;
	return selected % count;
}

void resetMenuTo(UserInterface &ui, MenuMode mode) {
	ui.menu.mode = mode;
	ui.needToRefresh = true;
	ui.menu.selected = 0;
	ui.pickedIndex = -1;
	resetMessages(ui);
}

void loadCurrentScores(UserInterface &ui) {
	for (auto &scores : ui.playersScores)
		for (auto &player : ui.board.players)
			if (scores.id == player.id) {
				player.points = scores.points;
				break;
			}
}

void newGameController(int input, UserInterface &ui) {
	char name[] = "game0.txt";
	switch (input) {
		case 'n':
			ui.state = PICK_USER;
			resetMenuTo(ui, PICK_PLAYERS);
			break;
		case 'l':
			ui.state = GAME_PLAY;
			resetMenuTo(ui, DEFAULT);
			loadFromFile(name, ui.board);
			loadHistoryFromFile(name, ui.history);
			loadCurrentScores(ui);
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
			reverseMoves(ui.board, ui.history);
			break;
		case 'r':
			for (int &dice: ui.board.dices) {
				dice = rand() % 6 + 1;
//				dice = 6;
			}
			resetMenuTo(ui, PICK_DICE);
			break;
		default:
			break;
	}
}

void createDiceMessage(UserInterface &ui) {
	int digits = (int) (nDigits(ui.currentMove.by, 10));
	const char *moveBy = numberToString(ui.currentMove.by, digits);
	const char *str = joinStrings1("Move by: ", 9, moveBy, digits);
	messageSet(ui.infoMess, str);
	delete[] str;
	delete[] moveBy;
}

void createErrorMessage(UserInterface &ui, MoveStatus errorId) {
	if (errorId == MOVE_FAILED) {
		messageSet(ui.errorMess, "Invalid Point");
	} else if (errorId == PAWNS_ON_BAR) {
		messageSet(ui.errorMess, "Pawns on Bar");
	} else if (errorId == FORCE_CAPTURE) {
		messageSet(ui.errorMess, "No Free Will");
	} else if (errorId == FORCE_ESCAPE) {
		messageSet(ui.errorMess, "Run You Fool");
	}
}

void pickDiceController(int input, UserInterface &ui) {
	int *dice = ui.board.dices;
	for (int i = 0; i < N_DICES; ++i) {
		if (input == '1' + i) {
			ui.currentMove.by = dice[i];
			ui.currentMove.movesLeft = 1;
			dice[i] = 0;
			resetMenuTo(ui, PICK_POINT);
		}
	}

	if (input == '0') {
		ui.currentMove.by = dice[0] + dice[1];
		if (dice[0] == dice[1]) {
			ui.currentMove.movesLeft = 4;
			ui.currentMove.by = dice[0];
		} else {
			ui.currentMove.movesLeft = 1;
		}
		resetMenuTo(ui, PICK_POINT);
		clearDices(ui.board.dices);
	} else if (input == '-') {
		setBasicGameState(ui);
		changePlayers(ui.board);
		clearDices(ui.board.dices);
	}

	if (ui.currentMove.by != 0) {
		createDiceMessage(ui);
	}
}

void handlePawnResponse(UserInterface &ui, MoveStatus moveError) {
	if (!statusToBool(moveError)) {
		createErrorMessage(ui, moveError);
	} else if (!--ui.currentMove.movesLeft) {
		resetMenuTo(ui, PICK_DICE);
	} else {
		resetMenuTo(ui, PICK_POINT);
	}
}

void pickPointController(int input, UserInterface &ui) {
	MoveStatus moveError;
	switch (input) {
		case ' ':
			ui.currentMove.from = ui.pickedIndex;
			moveError = handlePawnMovement(ui.board, ui.currentMove, ui.history);
			handlePawnResponse(ui, moveError);
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
		case KEY_UP:
		case KEY_DOWN:
			ui.pickedIndex = newSelected(ui.pickedIndex, input == KEY_UP ? 1 : -1, nPoints + 1);
			break;
		default:
			numberInputController(input, ui.pickedIndex);
			ui.pickedIndex %= (nPoints + 1);
			break;
	}
}

void gameWonController(int input, UserInterface &ui) {
	if (input == 'n') {
		ui.state = GAME_PLAY;
		resetMenuTo(ui, DEFAULT);
		setClearBoard(ui.board);
		gameSetUp(ui.board);
	}
}

bool trySettingName(UserInterface &ui) {
	for (int i = 0; i < N_PLAYERS; ++i) {
		ScorePlayer selected = ui.playersScores[ui.nowPickedPlayer];

		if (ui.board.players[i].id == selected.id)
			return false;

		if (ui.board.players[i].id < 0) {
			ui.board.players[i].id = selected.id;
			strcpy((char *)(ui.board.players[i].name), selected.name);
			ui.board.players[i].points = selected.points;

			return i == N_PLAYERS - 1;
		}
	}

	return false;
}

void addPlayerToGame(UserInterface &ui) {
	if (trySettingName(ui)) {
		gameSetUp(ui.board);
		ui.state = GAME_PLAY;
		resetMenuTo(ui, DEFAULT);
	}
}

void pickPlayerController(int input, UserInterface &ui) {
	switch (input) {
		case ' ':
			addPlayerToGame(ui);
			break;
		case KEY_UP:
		case KEY_DOWN:
		case 's':
			ui.nowPickedPlayer = newSelected(ui.nowPickedPlayer, input == KEY_UP ? -1 : 1, N_ALL_PLAYERS);
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
			pickPointController(input, ui);
			break;
		case PICK_DICE:
			pickDiceController(input, ui);
			break;
		case STARTING_GAME:
			newGameController(input, ui);
			break;
		case GAME_WON:
			gameWonController(input, ui);
			break;
		case PICK_PLAYERS:
			pickPlayerController(input, ui);
			break;
	}
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
		case KEY_LEFT:
		case KEY_RIGHT:
			if (!(ui.menu.mode == PICK_POINT || ui.menu.mode == PICK_PLAYERS)) {
				ui.menu.selected = newSelected(ui.menu.selected, input == KEY_RIGHT ? 1 : -1, ui.menu.count);
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
