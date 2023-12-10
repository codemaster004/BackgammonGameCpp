//
// Created by Filip Dabkowski on 18/11/2023.
//

#include "ncurses.h"
#include "cstring"

#include "../configs/UIConfigs.h"
#include "../configs/MenuConfigs.h"
#include "UserInterface.h"
#include "../view/Handeler.h"
#include "../controller/InputControll.h"

UserInterface initUI() {
	auto ui = UserInterface{
		.gameName="Backgammon",
		.authorId="s197712",
		.authorName="Filip Dabkowski",
		.space=GameSpace{.gameSpace=initGameSpace()},
		.currentMove=initMove(),
		.pickedIndex=-1,
		.history=MoveMade{},
	};

	// Initialize Placements for Board Elements
	ui.space.board = initBoard(ui.space.gameSpace);
	Placement board = ui.space.board;
	ui.space.boardCenter = initCenter(board, ui.space.gameSpace);
	ui.space.dice = initDice(ui.space.gameSpace);
	ui.space.indexesTop = initIndex(board, board.min.y - INDEX_OFFSET);
	ui.space.indexesBottom = initIndex(board, board.max.y + INDEX_OFFSET);
	ui.needToRefresh = true;

	return ui;
}

Move initMove() {
	// Return a Move with all fields set to zero
	return Move{0, 0, 0};
}

void setBasicGameState(UserInterface &ui) {
	clearDices(ui.board.dices);
	ui.currentMove = {0, 0, 0};
	resetMenuTo(ui, DEFAULT);
	resetMessages(ui);
}

void titleArt(int offsetX, int offsetY) {
	attron(COLOR_PAIR(FOREGROUND)); // Turn on the foreground color pair for the title art

	// Print each line of the title art at the specified offsets
	mvprintw(offsetY + 0, offsetX, R"( ____             _     ___)") ;
	mvprintw(offsetY + 1, offsetX, R"(| __ )  __ _  ___| |__ / __|  __ _ _ __ ___  _ __ ___   ____  _ __)");
	mvprintw(offsetY + 2, offsetX, R"(|  _ \ / _` |/ __| / // / __ / _` | '_ ` _ \| '_ ` _ \ / __ \| '_ \)");
	mvprintw(offsetY + 3, offsetX, R"(| |_) | (_| | (__|   \\ \_| | (_| | | | | | | | | | | |  \/  | | | |)");
	mvprintw(offsetY + 4, offsetX, R"(|____/ \__,_|\___|_|\_\\___/ \__,_|_| |_| |_|_| |_| |_|\____/|_| |_|)");

	attroff(COLOR_PAIR(FOREGROUND)); // Turn off
}

void starterScreen(UserInterface &ui) {
	// Display the title art at a specified position
	titleArt(OFFSET_X, OFFSET_Y + boardHeight / 2);

	// Handle and display the game menu at a position relative to the board's center and maximum Y value
	handleMenu(ui.menu, Pos{ui.space.boardCenter.x, ui.space.board.max.y + MENU_TOP_SPACING});
}

int calcDiceLimit(UserInterface &ui) {
	int limit = 1; // Start with a base number of Options 1

	// Increment the limit for each dice that has a value
	for (int dice: ui.board.dices)
		if (dice > 0)
			limit += 1;

	// If the limit equals the number of dices plus one (starter value),
	// increase it further for Option to "Pick All"
	if (limit == N_DICES + 1)
		limit += 1;

	// Return the limit, ensuring it does not exceed 9
	return min(9, limit);
}

/**
 * @brief Creates a dice option string for the UI.
 *
 * Allocates memory for a new string, formats it with the dice index and value,
 * and places it in the options array at the given index. Memory allocation for
 * number conversion is managed within the function.
 *
 * @param options The array of strings where the new option string will be stored.
 * @param index The index in the options array where the new string will be placed.
 * @param value The value of the dice to be displayed in the option.
 */
void createDiceOption(char **&options, int index, int value) {
	// Allocate memory for the new option string
	options[index] = new char[16]; // 16 was good enough
	char *diceNumber = numberToString(index + 1, 1);
	char *diceValue = numberToString(value, 1);

	// Construct the option string & join parts into one
	const char *diceName[4] = {"Dice (", diceNumber, "): ", diceValue};
	joinStrings(options[index], diceName, 4);

	// Free allocated memory
	delete[] diceNumber;
	delete[] diceValue;
}

/**
 * @brief Generates the dice selection menu for the game UI.
 *
 * This function creates a menu for dice selection based on the current state of the dice in the game.
 * It dynamically allocates memory for menu options and keys, populates them with relevant dice information,
 * and manages additional options like using all dices or ending the turn. Memory is allocated for each string
 * in the options array and should be properly managed.
 *
 * @param ui Reference to UserInterface containing game state and menu.
 * @param options Reference to an array of strings for menu options.
 * @param keys Reference to an array of characters for menu keys.
 */
void createDiceMenu(UserInterface &ui, char **&options, char *&keys) {
	// Calculate the limit for dice options & set it in the menu
	int limit = calcDiceLimit(ui);
	ui.menu.count = limit;

	// Allocate memory
	options = new char* [limit];
	keys = new char [limit];

	int index = 0;
	// Loop through the dices and create menu options for each dice
	for (int i = 0; i < N_DICES; ++i) {
		if (ui.board.dices[i]) {
			// Create an option for the dice
			createDiceOption(options, index, ui.board.dices[i]);
			keys[index++] = (char) ('1' + i);
		}
	}

	// Add special option to use all dices if applicable
	if (limit == N_DICES + 2) {
		options[limit - 2] = new char[18];
		strcpy(options[limit - 2], "Use all Dices (0)");
		keys[limit - 2] = '0';
	}

	// Add option to end the turn
	options[limit - 1] = new char[15];
	strcpy(options[limit - 1], "End turn (-)");
	keys[limit - 1] = '-';
}

/**
 * @brief Initializes menu elements for the user interface.
 *
 * This function dynamically allocates memory for the menu elements and initializes each element
 * with its respective ID, key, and value. The values and keys are taken from the provided options
 * and keys arrays. It is important to ensure that the allocated memory is managed properly.
 *
 * @param ui Reference to the UserInterface containing the menu to be initialized.
 * @param options Array of strings representing the menu options.
 * @param keys Array of characters representing the keys for each menu option.
 */
void initMenuElement(UserInterface &ui, char **options, const char keys[]) {
	// Allocate Memory for elements
	ui.menu.elements = new MenuElement[ui.menu.count];

	// Initialize each menu element with its ID, key, and value
	for (int i = 0; i < ui.menu.count; ++i) {
		ui.menu.elements[i] = MenuElement{.id=i, .key=keys[i], .value=options[i]};
	}
}

/**
 * @brief Populates an options array with strings from a source array.
 *
 * Dynamically allocates memory for each string in the options array and copies
 * strings from the origin array into it. It is important to manage the memory
 * allocated for these strings appropriately.
 *
 * @param options Reference to an array of strings to be populated.
 * @param origin Source array containing the strings to copy.
 * @param limit The number of elements to copy from the origin array.
 */
void populateOptions(char **&options, const char *origin[], int limit) {
	// Allocate memory for the array of string pointers
	options = new char *[limit];

	// Loop through each element in the origin array
	for (int i = 0; i < limit; ++i) {
		// Allocate memory for the string
		uint length = len(origin[i]);
		options[i] = new char[length];

		// Copy each character of the string from the origin to options
		for (int j = 0; j < length; ++j) {
			options[i][j] = origin[i][j];
		}
	}
}

/**
 * @brief Populates an array of keys from a source array.
 *
 * Dynamically allocates memory for the keys array and copies characters (keys)
 * from the origin array into it. This function is typically used for setting up
 * menu keys or similar character-based options.
 *
 * @param keys Reference to a character array to be populated.
 * @param origin Source array containing the keys to copy.
 * @param limit The number of elements to copy from the origin array.
 */
void populateKeys(char *&keys, const char origin[], int limit) {
	// Allocate memory for the keys array
	keys = new char [limit];

	// Copy each key from the origin to the keys array
	for (int i = 0; i < limit; ++i) {
		keys[i] = origin[i];
	}
}

/**
 * @brief Creates option strings for different menu modes.
 *
 * Depending on the provided menu mode, this function populates the options array
 * with appropriate options. It uses predefined option sets for different modes
 * and delegates the actual population of the array to the populateOptions function.
 *
 * @param options Reference to an array of string pointers to be populated with options.
 * @param count The number of options to populate.
 * @param mode The current menu mode, which determines the set of options to use.
 */
void createOptions(char **&options, int count, MenuMode mode) {
	const char *basic[] = {"(R)oll", "(U)ndo", "(S)ave", "(Q)uit"};
	const char *start[] = {"(N)ew Game", "(L)oad Game", "(Q)uit"};
	const char *point[] = {"Pick Piece (Space)", "End Turn (-)"};
	const char *win[] = {"(N)ew Game", "(Q)uit"};
	const char *hall[] = {"Select (Space)", "(Q)uit"};

	const char **selected;
	// Populate the options based on the current menu mode
	switch (mode) {
		case DEFAULT:
			selected = basic;
			break;
		case PICK_POINT:
			selected = point;
			break;
		case PICK_DICE:
			// Pick Dice is Handled in main Switch case so in this case do nothing
			return;
		case STARTING_GAME:
			selected = start;
			break;
		case GAME_WON:
			selected = win;
			break;
		case PICK_PLAYERS:
			selected = hall;
			break;
	}
	populateOptions(options, selected, count);
}

/**
 * @brief Creates key characters for different menu modes.
 *
 * Based on the provided menu mode, this function populates the keys array with
 * appropriate key characters. It uses predefined key sets for different modes
 * and utilizes the populateKeys function for actual array population.
 *
 * @param keys Reference to a character array to be populated with keys.
 * @param count The number of keys to populate.
 * @param mode The current menu mode, which determines the set of keys to use.
 */
void createKeys(char *&keys, int count, MenuMode mode) {
	const char basic[] = {'r', 'u', 's', 'q'};
	const char start[] = {'n', 'l', 'q'};
	const char point[] = {' ', '-'};
	const char win[] = {'n', 'q'};
	const char hall[] = {' ', 'q'};

	// Populate the keys array based on the current menu mode
	switch (mode) {
		case DEFAULT:
			populateKeys(keys, basic, count);
			break;
		case PICK_POINT:
			populateKeys(keys, point, count);
			break;
		case PICK_DICE:
			// Pick Dice is Handled in main Switch case so in this case do nothing
			return;
		case STARTING_GAME:
			populateKeys(keys, start, count);
			break;
		case GAME_WON:
			populateKeys(keys, win, count);
			break;
		case PICK_PLAYERS:
			populateKeys(keys, hall, count);
			break;
	}
}

void redefineMenu(UserInterface &ui) {
	ui.menu.selected = 0; // Initialize the selected menu item to 0
	char **options = nullptr;
	char *keys = nullptr;

	// Determine the number of menu options based on the current menu mode
	switch (ui.menu.mode) {
		case DEFAULT:
			ui.menu.count = N_GAME_OPTIONS;
			break;
		case STARTING_GAME:
			ui.menu.count = N_STARTER_OPTIONS;
			break;
		case PICK_POINT:
			ui.menu.count = N_PIECE_OPTIONS;
			// No item selected by default in PICK_POINT mode
			ui.menu.selected = -1;
			ui.pickedIndex = 0;
			break;
		case PICK_DICE:
			createDiceMenu(ui, options, keys);
			break;
		case GAME_WON:
		case PICK_PLAYERS:
			ui.menu.count = N_WIN_OPTIONS;
			ui.menu.selected = -1;
			break;
	}

	// Create the menu options and keys based on the current mode
	createOptions(options, ui.menu.count, ui.menu.mode);
	createKeys(keys, ui.menu.count, ui.menu.mode);

	// Initialize menu elements with the created options and keys
	initMenuElement(ui, options, keys);
}

void resetMessage(char message[MAX_MESSAGE_LEN]) {
	// Fill each character in the message array with a blank space
	for (int i = 0; i < MAX_MESSAGE_LEN; ++i) {
		message[i] = ' ';
	}
}

void resetMessages(UserInterface &ui) {
	resetMessage(ui.infoMess); // Reset the information message
	resetMessage(ui.errorMess); // Reset the error message
}

void messageSet(char message[MAX_MESSAGE_LEN], const char *newMessage) {
	// Determine the length to copy
	int length = min(MAX_MESSAGE_LEN - 1, (int)(len(newMessage)));

	// Copy the new message into the message array up to the determined length
	for (int i = 0; i < length; ++i) {
		message[i] = newMessage[i];
	}

	// Ensure the message is null-terminated
	message[length] = '\0';
}
