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
#include "../viewModel//Menu.h"

// PRIVATE FUNCTION HEADERS //

/**
 * @brief Processes numeric input to construct an integer value.
 *
 * @param input The latest character input, expected to be a numeric character ('0' - '9').
 * @param inputtedNumber Reference to the integer that is being constructed or updated with the input.
 */
void numberInputController(int input, int &inputtedNumber);

/**
 * @brief Calculates a new selection index based on a change value.
 *
 * @param selected The current selection index.
 * @param change The amount to change the index (can be positive or negative).
 * @param count The total number of items in the selection range.
 * @return int The updated selection index, wrapped within the range.
 */
int newSelected(int selected, int change, int count);

/**
 * @brief Handles user input after a game is won, specifically for starting a new game.
 *
 * @param input The character input from the user.
 * @param ui Reference to the UserInterface structure representing the game state.
 */
void gameWonController(int input, UserInterface &ui);

/**
 * @brief Attempts to set the name of a player in the game from the user interface's player scores.
 *
 * @param ui Reference to the UserInterface structure representing the game state and player scores.
 * @return bool True if the name is successfully set, false otherwise.
 */
bool trySettingName(UserInterface &ui);

/**
 * @brief Adds a player to the game and sets up the game if the player is successfully added.
 *
 * @param ui Reference to the UserInterface structure representing the game state and player scores.
 */
void addPlayerToGame(UserInterface &ui);

/**
 * Handles player controller selection based on user input.
 *
 * @param input The input character representing the user's choice.
 * @param ui Reference to the UserInterface object.
 */
void pickPlayerController(int input, UserInterface &ui);

/**
 * Manages different menu modes based on user input.
 *
 * @param input The input character representing the user's choice.
 * @param ui Reference to the UserInterface object.
 */
void handleMenuModes(int input, UserInterface &ui);

/**
 * Loads the current scores into the player objects.
 *
 * @param ui Reference to the UserInterface object containing player scores
 *           and the game board.
 */
void loadCurrentScores(UserInterface &ui);

/**
 * Creates a message displaying the dice roll result.
 *
 * @param ui Reference to the UserInterface object containing current move information.
 */
void createDiceMessage(UserInterface &ui);

/**
 * Creates an error message based on the move status.
 *
 * @param ui Reference to the UserInterface object for displaying the message.
 * @param errorId The identifier for the type of move error encountered.
 */
void createErrorMessage(UserInterface &ui, MoveStatus errorId);

/**
 * Handles the response to a pawn move based on the move status.
 *
 * @param ui Reference to the UserInterface object for displaying messages and updating the menu.
 * @param moveError The status of the pawn move.
 */
void handlePawnResponse(UserInterface &ui, MoveStatus moveError);


// PUBLIC FUNCTION DECLARATIONS //


void gameSetUp(Board &game) {
	// Set the current player randomly
	game.currentPlayerId = game.players[rand() % N_PLAYERS].id;

	// Reset all dice values to 0
	for (int &dice: game.dices) {
		dice = 0;
	}

	// Assign owners to each court
	for (int i = 0; i < N_PLAYERS; ++i) {
		game.courts[i].owner = &game.players[i];
	}

	// Initialize pawns for each player
	for (int i = 0; i < PAWNS_PER_PLAYER; ++i) {
		game.pawns[i] = Pawn{.ownerId=game.players[0].id, .id=PAWNS_PER_PLAYER +
															  i, .color=PAWN_WHITE, .moveDirection=1};
		game.pawns[i +
				   PAWNS_PER_PLAYER] = Pawn{.ownerId=game.players[1].id, .id=i, .color=PAWN_BLACK, .moveDirection=-1};
	}

	// Place pawns on the game board
	placePawns(game);
}

void resetMenuTo(UserInterface &ui, MenuMode mode) {
	ui.menu.mode = mode; // Update the menu mode
	ui.needToRefresh = true; // Flag the UI to be refreshed
	ui.menu.selected = 0; // Reset the selected menu item to the first option
	ui.pickedIndex = -1; // Reset the picked index
	resetMessages(ui); // Reset all messages in the UI
}

void newGameController(int input, UserInterface &ui) {
	char name[] = "game0.txt"; // Predefined file name for game data

	switch (input) {
		case 'n':
			// Set to pick user state and reset menu for picking players
			ui.state = PICK_USER;
			resetMenuTo(ui, PICK_PLAYERS);
			break;
		case 'l':
			// Set to game play state and reset menu to default
			ui.state = GAME_PLAY;
			resetMenuTo(ui, DEFAULT);
			// Load game data from file
			loadFromFile(name, ui.board);
			loadHistoryFromFile(name, ui.history);
			loadCurrentScores(ui);
			break;
		default:
			// No action for other inputs
			break;
	}
}

void gamePlayController(int input, UserInterface &ui) {
	char name[] = "game0.txt"; // Predefined file name for saving/loading game state

	switch (input) {
		case 's':
			// Save current game state to file
			saveToFile(name, ui.board);
			saveHistoryToFile(name, ui.history);
			break;
		case 'u':
			// Undo the last set of moves
			reverseMoves(ui.board, ui.history);
			break;
		case 'r':
			// Roll the dice and update the dice values
			for (int &dice: ui.board.dices) {
				dice = rand() % 6 + 1; // Random dice roll between 1 and 6
				// dice = 6; // Uncomment to set all dice rolls to 6
			}
			resetMenuTo(ui, PICK_DICE); // Reset menu to dice picking mode
			break;
		default:
			// No action for other inputs
			break;
	}
}

void pickDiceController(int input, UserInterface &ui) {
	int *dice = ui.board.dices;

	// Handle individual dice selection
	for (int i = 0; i < N_DICES; ++i) {
		if (input == '1' + i) {
			ui.currentMove.by = dice[i];
			ui.currentMove.movesLeft = 1;
			dice[i] = 0;
			resetMenuTo(ui, PICK_POINT);
		}
	}

	// Handle selection of both dice
	if (input == '0') {
		ui.currentMove.by = dice[0] + dice[1];
		if (dice[0] == dice[1]) {
			ui.currentMove.movesLeft = 4; // Special case for a double
			ui.currentMove.by = dice[0];
		} else {
			ui.currentMove.movesLeft = 1;
		}
		resetMenuTo(ui, PICK_POINT);
		clearDices(ui.board.dices);
	}
		// Handle turn change
	else if (input == '-') {
		setBasicGameState(ui);
		changePlayers(ui.board);
		clearDices(ui.board.dices);
	}

	// Create a dice message if a move is set
	if (ui.currentMove.by != 0) {
		createDiceMessage(ui);
	}
}

void pickPointController(int input, UserInterface &ui) {
	MoveStatus moveError;

	switch (input) {
		case ' ':
			// Execute pawn movement
			ui.currentMove.from = ui.pickedIndex;
			moveError = handlePawnMovement(ui.board, ui.currentMove, ui.history);
			handlePawnResponse(ui, moveError);
			break;
		case '-':
			// Reset current move and menu for a new dice roll
			ui.currentMove.from = ui.pickedIndex;
			ui.currentMove = {0, 0};
			resetMenuTo(ui, PICK_DICE);
			break;
		case KEY_BACKSPACE:
		case 0x7F:
			// Handle backspace for index correction
			ui.pickedIndex /= 10;
			break;
		case KEY_UP:
		case KEY_DOWN:
			// Navigate through selectable points
			ui.pickedIndex = newSelected(ui.pickedIndex, input == KEY_UP ? 1 : -1, nPoints + 1);
			break;
		default:
			// Handle numeric input for point selection
			numberInputController(input, ui.pickedIndex);
			ui.pickedIndex %= (nPoints + 1);
			break;
	}
}

void inputController(int input, UserInterface &ui) {
	resetMessage(ui.errorMess); // Reset any existing error messages

	switch (input) {
		case '\n':
		case '\r':
			// Process selection based on currently selected menu element
			if (ui.menu.selected >= 0) {
				inputController(ui.menu.elements[ui.menu.selected].key, ui);
			}
			break;
		case KEY_LEFT:
		case KEY_RIGHT:
			// Navigate through menu elements unless in specific modes
			if (!(ui.menu.mode == PICK_POINT || ui.menu.mode == PICK_PLAYERS)) {
				ui.menu.selected = newSelected(ui.menu.selected, input == KEY_RIGHT ? 1 : -1, ui.menu.count);
			}
			break;
		default:
			// Check for game end command or handle other menu modes
			if (ui.menu.elements[ui.menu.selected].key == 'q') {
				ui.gameEnded = true;
			} else {
				handleMenuModes(input, ui);
			}
			break;
	}
}



// PRIVATE FUNCTION DECLARATIONS //


void numberInputController(int input, int &inputtedNumber) {
	// Check if the input is a numeric character
	if (input >= '0' && input <= '9') {
		// Multiply the current number by 10
		inputtedNumber *= 10;
		// Add the numeric value of the input character
		inputtedNumber += (int) (input - '0');
	}
}

int newSelected(int selected, int change, int count) {
	selected += change; // Update the selected index
	if (selected < 0)
		selected = count - 1; // Wrap around to the last item if the index goes below zero
	return selected % count; // Ensure the index stays within the range
}

void gameWonController(int input, UserInterface &ui) {
	// Check if the input is 'n' to start a new game
	if (input == 'n') {
		ui.state = GAME_PLAY; // Set the game state to active play
		// Reset the menu to its default state
		resetMenuTo(ui, DEFAULT);
		// Clear and Setup board for new game
		setClearBoard(ui.board);
		gameSetUp(ui.board);
	}
}

bool trySettingName(UserInterface &ui) {
	for (int i = 0; i < N_PLAYERS; ++i) {
		ScorePlayer selected = ui.playersScores[ui.nowPickedPlayer];

		// Return false if the player ID already exists in the game
		if (ui.board.players[i].id == selected.id)
			return false;

		// Set the player's details if an empty slot is found
		if (ui.board.players[i].id < 0) {
			ui.board.players[i].id = selected.id;
			strcpy((char *) (ui.board.players[i].name), selected.name);
			ui.board.players[i].points = selected.points;

			// Return true if this is the last player slot
			return i == N_PLAYERS - 1;
		}
	}

	return false; // Return false if no suitable slot is found
}

void addPlayerToGame(UserInterface &ui) {
	// Attempt to add a player to the game
	if (trySettingName(ui)) {
		// If successful, set up the game for play
		gameSetUp(ui.board);
		ui.state = GAME_PLAY; // Change the game state to active play
		resetMenuTo(ui, DEFAULT); // Reset the menu to its default state
	}
}

void pickPlayerController(int input, UserInterface &ui) {
	switch (input) {
		case ' ':
			// Add player to game
			addPlayerToGame(ui);
			break;
		case KEY_UP:
		case KEY_DOWN:
			// Change currently picked player
			ui.nowPickedPlayer = newSelected(ui.nowPickedPlayer, input == KEY_UP ? -1 : 1, N_ALL_PLAYERS);
			break;
		default:
			// No action for other keys
			break;
	}
}

void handleMenuModes(int input, UserInterface &ui) {
	switch (ui.menu.mode) {
		case DEFAULT:
			// Handle game play controls
			gamePlayController(input, ui);
			break;
		case PICK_POINT:
			// Handle point picking controls
			pickPointController(input, ui);
			break;
		case PICK_DICE:
			// Handle dice picking controls
			pickDiceController(input, ui);
			break;
		case STARTING_GAME:
			// Handle new game controls
			newGameController(input, ui);
			break;
		case GAME_WON:
			// Handle game won scenario controls
			gameWonController(input, ui);
			break;
		case PICK_PLAYERS:
			// Handle player picking controls
			pickPlayerController(input, ui);
			break;
	}
}

void loadCurrentScores(UserInterface &ui) {
	// Iterate through each player's scores
	for (auto &scores: ui.playersScores) {
		// Iterate through each player on the board
		for (auto &player: ui.board.players) {
			if (scores.id == player.id) {
				// Update player's points
				player.points = scores.points;
				break;
			}
		}
	}
}

void createDiceMessage(UserInterface &ui) {
	// Calculate the number of digits in the move value
	int digits = (int) nDigits(ui.currentMove.by, 10);
	// Convert the move value to a string
	const char *moveBy = numberToString(ui.currentMove.by, digits);
	// Create the complete message string
	const char *str = joinStrings1("Move by: ", 9, moveBy, digits);

	// Set the message in the user interface
	messageSet(ui.infoMess, str);

	// Clear the allocated memory
	delete[] str;
	delete[] moveBy;
}

void createErrorMessage(UserInterface &ui, MoveStatus errorId) {
	if (errorId == MOVE_FAILED) {
		// Set message for invalid point error
		messageSet(ui.errorMess, "Invalid Point");
	} else if (errorId == PAWNS_ON_BAR) {
		// Set message when pawns are on the bar
		messageSet(ui.errorMess, "Pawns on Bar");
	} else if (errorId == FORCE_CAPTURE) {
		// Set message for forced capture scenario
		messageSet(ui.errorMess, "No Free Will");
	} else if (errorId == FORCE_ESCAPE) {
		// Set message for forced escape scenario
		messageSet(ui.errorMess, "Run You Fool");
	}
}

void handlePawnResponse(UserInterface &ui, MoveStatus moveError) {
	if (!statusToBool(moveError)) {
		// Create an error message if the move was unsuccessful
		createErrorMessage(ui, moveError);
	} else if (!--ui.currentMove.movesLeft) {
		// Reset the menu to PICK_DICE if no moves are left
		resetMenuTo(ui, PICK_DICE);
	} else {
		// Reset the menu to PICK_POINT if there are moves left
		resetMenuTo(ui, PICK_POINT);
	}
}
