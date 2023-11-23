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

void startScreen(int argc, char **argv);

int getInput(int argc, char **argv);

Placement initBoard() {
	Placement space{.min={OFFSET_X, OFFSET_Y}};
	space.max = {space.min.x + N_BOARDS * (boardWidth + borders + BAR_WIDTH) - BAR_WIDTH - 1,
				 space.min.y + boardHeight + borders - 1};
	return space;
}

Placement initDice(Placement board) {
	return Placement{
		.min={board.max.x, board.min.y},
		.max={board.max.x + DICE_WIDTH + borders - 1, board.max.y},
	};
}

Placement initIndex(Placement board, int offset) {
	return Placement {
		.min={
			board.min.x + BORDER_WIDTH + pieceSpacing / 2,
			offset,
		},
		.max={
			board.max.x - BORDER_WIDTH - pieceSpacing / 2,
		}
	};
}

Pos initCenter(Placement space) {
	return Pos{
		.x=space.min.x + (space.max.x - space.min.x) / 2,
		.y=space.min.y + (space.max.y - space.min.y) / 2
	};
}

UserInterface initUI() {
	UserInterface ui = UserInterface{
		.authorId=197712,
		.menuMode=DEFAULT,
		.space=GameSpace{initBoard(),
		}
	};
	Placement board = ui.space.board;
	ui.space.boardCenter = initCenter(board);
	ui.space.dice = initDice(board);
	ui.space.indexesTop = initIndex(board, board.min.y - 1);
	ui.space.indexesBottom = initIndex(board, board.max.y + 1);

	return ui;
}

int main(int argc, char **argv) {
	startScreen(argc, argv);

	UserInterface UI = initUI();
	UI.board = Board{};
	handleGame(&UI.board);

	int dice1 = 0, dice2 = 0;
	int menuSelected = 0;

	generateBasicBoard(&UI, &menuSelected, &dice1, &dice2);
	generateInteractiveUI(&UI, &menuSelected);

	int ch;
	bool gameEnded = false;
	int pickedPiece = 0;
	while((ch = getInput(argc, argv)) != 'q') {

		inputController(ch, &UI.board, &menuSelected, &gameEnded, &dice1, &dice2, &pickedPiece);
		if (gameEnded)
			break;

		generateBasicBoard(&UI, &menuSelected, &dice1, &dice2);
		generateInteractiveUI(&UI, &menuSelected);

		// Refresh the screen to show changes
		refresh();
	}

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
 */
