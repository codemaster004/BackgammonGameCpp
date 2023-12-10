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


/**
 * @brief Sets up the start screen with ncurses and initializes random seed and color pairs.
 * @param argc The count of command-line arguments.
 */
void startScreen(int argc);

/**
 * @brief Retrieves input from the user. If more than one argument is passed (--no-gui), ncurses getch is used, otherwise getchar.
 * @param argc The count of command-line arguments.
 * @return The character code of the key pressed.
 */
int getInput(int argc);

/**
 * @brief Updates the user interface based on the current game state.
 * @param ui Reference to the UserInterface object.
 */
void interface(UserInterface &ui);

/**
 * @brief Renders the area to pick player and see current scores.
 * @param ui Reference to the UserInterface object.
 */
void renderHallOfFame(UserInterface &ui);

/**
 * @brief Entry point of the application. Initializes the game and handles the main game loop.
 * @param argc Argument count.
 * @param argv Argument table of "strings".
 * @return Integer indicating the exit status.
 */
int main(int argc, char **argv) {
	startScreen(argc);

	UserInterface UI = initUI();
	UI.board = Board{};
	clearPlayers(UI.board);
	setClearBoard(UI.board);

	char filename[] = "hall.txt";
	loadScores(filename, UI);

	UI.state = WELCOME_SCREEN;
	UI.menu.mode = STARTING_GAME;
//	if (DEBUG_MODE) {
//		UI.state = GAME_PLAY;
//		UI.menu.mode = DEFAULT;
//	}

	redefineMenu(UI);

	int ch = 0;
	UI.gameEnded = false;
	do {
		inputController(ch, UI);
		if (UI.gameEnded)
			break;
		if (UI.board.winnerPlayerId != -1)
			handleGameWon(UI);

		interface(UI);
	} while ((ch = getInput(argc)) != 'q');
	 delete[] UI.menu.elements;

	// Finish curses mode
	endwin();

	return 0;
}

void interface(UserInterface &ui) {
	if (ui.needToRefresh) {
		delete[] ui.menu.elements;
		clear();
		redefineMenu(ui);
		ui.needToRefresh = false;
	}

	if (ui.state == GAME_PLAY) {
		generateBasicBoard(ui);
		generateInteractiveUI(ui);
	} else if (ui.state == WELCOME_SCREEN) {
		starterScreen(ui);
	} else if (ui.state == PICK_USER) {
		renderHallOfFame(ui);
	}

	// Refresh the screen to show changes
	refresh();
}

int getInput(int argc) {
	if (argc > 1)
		return getch();
	else
		return getchar();
}

void startScreen(int argc) {
	if (argc > 1)
		initscr();
	cbreak();
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);

	srand(time(nullptr));

	start_color();

	setColourTheme(COLOR_THEME);

	init_pair(FOREGROUND, COLOUR_MAIN, COLOR_BLACK);
	init_pair(FOREGROUND_LIGHT, COLOUR_MAIN_LIGHT, COLOR_BLACK);
	init_pair(FOREGROUND_DARK, COLOUR_MAIN_DARK, COLOR_BLACK);
	init_pair(BACKGROUND_LIGHT, COLOR_BLACK, COLOUR_MAIN_LIGHT);
	init_pair(BACKGROUND_DARK, COLOR_BLACK, COLOUR_MAIN_DARK);
}

void createPlayerHallText(UserInterface ui, char **text, int index) {
	int id = ui.playersScores[index].id;
	int points = ui.playersScores[index].points;

	const char *data[5] = {
		numberToString(id, nDigits(id, 10)),
		". ",
		ui.playersScores[index].name,
		": ",
		numberToString(points, nDigits(points, 10)),
	};

	text[index] = new char [MAX_NAME_LENGTH + 8];
	joinStrings0(text[index], data, 5);
	delete[] data[0];
	delete[] data[4];
}

void renderHallOfFame(UserInterface &ui) {
	Placement pos = createCentersPlacement(ui.space.boardCenter, MAX_NAME_LENGTH + 10, N_ALL_PLAYERS * (TEXT_HEIGHT + VERTICAL_SPACING) + borders + VERTICAL_SPACING);

	drawBorders(pos);
	pos.min.y += BORDER_WIDTH + VERTICAL_SPACING;
	char **text = new char *[N_ALL_PLAYERS];
	for (int i = 0; i < N_ALL_PLAYERS; ++i) {
		createPlayerHallText(ui, text, i);
	}

	drawSpreadTextVertical(pos, text, 3, ui.nowPickedPlayer, VERTICAL_SPACING);

	for (int i = 0; i < N_ALL_PLAYERS; ++i) {
		delete[] text[i];
	}
	delete[] text;

	// Handle and display the game menu at a position relative to the board's center and maximum Y value
	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});
}

/**
 * DONE: [x] Basic Board UI (1pt)
 * DONE: [x] Visualisation of game state (1pt)
 * DONE: [x] Saving game state to a file (2pt)
 * DONE: [x] Loading game state from file + next move (2pt)
 * DONE: [x] Ability to capture pawnsId (1pt)
 * DONE: [x] Removing pawnsId ?bar or court? (1pt)
 * DONE: [x] ?Game simulation from any point till the end? (2pt)
 * DONE: [x] Remember and save game past moves (1pt)
 * DONE: [x] Visualisation of game play (backwards) from loaded state (2pt)
 * DONE: [x] Complete UI (1pt)
 * TODO: [ ] Player Rankings "Hall of Fame" (1pt)
 */
