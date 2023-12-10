//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_USERINTERFACE_H
#define BACKGAMMONGAME_USERINTERFACE_H

#include "../configs/GameConfigs.h"
#include "../configs/UIConfigs.h"
#include "../model/Board.h"
#include "Space.h"
#include "../model/History.h"

/**
 * @enum MenuMode
 * @brief Represents the various modes of the game menu.
 */
enum MenuMode {
	DEFAULT,        ///< Default mode.
	PICK_POINT,     ///< Mode for picking a point on the board.
	PICK_DICE,      ///< Mode for picking dice.
	STARTING_GAME,  ///< Mode for starting the game.
	GAME_WON,       ///< Mode after the game was won.
	PICK_PLAYERS,   ///< Mode for picking playing users.
};

/**
 * @enum UiState
 * @brief Represents different states of the user interface.
 */
enum UiState {
	WELCOME_SCREEN, ///< Welcome screen state.
	PICK_USER,      ///< State for picking a playing user.
	GAME_PLAY       ///< State during game play.
};

/**
 * @struct GameSpace
 * @brief Holds the layout of the game space.
 *
 * This structure defines the placements and positions of various elements in the game space.
 */
typedef struct GameSpace {
	Placement board;        ///< Placement of the board.
	Placement dice;         ///< Placement of the dice.
	Pos boardCenter;        ///< Center position of the board.
	Placement indexesTop;   ///< Placement of indexes at the top.
	Placement indexesBottom;///< Placement of indexes at the bottom.
	Placement gameSpace;    ///< Placement of the entire game space.
} GameSpace;

/**
 * @struct MenuElement
 * @brief Represents an element in the game menu.
 *
 * This structure holds the information about a single menu element, including its ID, key, and value.
 */
typedef struct MenuElement {
	int id;               ///< The ID of the menu element.
	char key;             ///< The key associated with this menu element.
	const char *value;    ///< The value or label of the menu element.
} MenuElement;

/**
 * @struct Menu
 * @brief Represents the game menu.
 *
 * This structure holds details about the game menu, including its mode, elements, count, and selected element.
 */
typedef struct Menu {
	MenuMode mode;          ///< The current mode of the menu.
	MenuElement *elements;  ///< Pointer to the array of menu elements.
	int count;              ///< The number of elements in the menu.
	int selected;           ///< Index of the currently selected menu element.
} Menu;

/**
 * @struct UserInterface
 * @brief Represents the entire user interface of the game.
 *
 * This structure encompasses all elements of the user interface, including the game board, menu, game space, and various states and messages.
 */
typedef struct UserInterface {
	const char *gameName;                 ///< Name of the game.
	const char *authorId;                 ///< ID of the author.
	const char *authorName;               ///< Name of the author.
	Board board;                          ///< Game board.
	int nowPickedPlayer;                  ///< Then index of selected player to join game.
	ScorePlayer playersScores[N_ALL_PLAYERS]; ///< Current scores of the players.
	Menu menu;                            ///< Game menu.
	GameSpace space;                      ///< Game space layout.
	bool needToRefresh;                   ///< Flag indicating if the UI needs to be refreshed.
	bool gameEnded;                       ///< Flag indicating if the game has ended.
	UiState state;                        ///< Current state of the UI.
	Move currentMove;                     ///< The current move.
	int pickedIndex;                      ///< The index picked by the player.
	char infoMess[MAX_MESSAGE_LEN];       ///< Information message.
	char errorMess[MAX_MESSAGE_LEN];      ///< Error message.
	MadeMove history;                     ///< History of moves made.
} UserInterface;

/**
 * @brief Initializes the User Interface for the game.
 *
 * This function sets up the initial state of the User Interface for the game, including setting up
 * the game name, author details, and initializing various components of the game space such as the
 * game board, dice, and index positions. It also sets the initial game state, like the current move
 * and game history.
 *
 * @return UserInterface The initialized User Interface object.
 */
UserInterface initUI();

/**
 * @brief Initializes a Move structure with default values.
 *
 * Creates and returns a Move structure with all its fields set to zero. This function
 * is typically used to initialize the state of a move at the beginning of a game or
 * when resetting the move state.
 *
 * @return Move A Move structure with initialized values.
 */
Move initMove();

/**
 * @brief Displays the starter screen of the game.
 *
 * This function is responsible for displaying the initial screen when the game starts.
 * It displays the title art at a specific position on the screen and handles the display
 * and interaction of the game's menu. The menu is positioned relative to the game board's
 * dimensions and the predefined offsets.
 *
 * @param ui Reference to the UserInterface object representing the game's UI state.
 */
void starterScreen(UserInterface &ui);

/**
 * @brief Sets the basic state of the game.
 *
 * This function is responsible for resetting the game state to its basic configuration. It
 * clears the dices on the board, resets the current move, resets the game menu to its default
 * state, and clears any messages on the user interface.
 *
 * @param ui Reference to the UserInterface object representing the game's UI state.
 */
void setBasicGameState(UserInterface &ui);

/**
 * @brief Displays the title art of the game.
 *
 * This function is responsible for printing the title art of the game on the screen.
 * It uses the ncurses library to position and display the title art at a specified
 * location. The title art is displayed using a specific color pair to enhance visibility.
 *
 * @param offsetX The horizontal offset (in characters) from the left where the title art begins.
 * @param offsetY The vertical offset (in lines) from the top where the title art begins.
 */
void titleArt(int offsetX, int offsetY);

/**
 * @brief Calculates the limit for the number of dice Options to show
 *
 * This function determines the limit on the number of dice based on the current state
 * of the dice in the user interface. It increments the limit for each dice with a value
 * greater than zero. If all dices can be used add one to the limit.
 *
 * @param ui Reference to the UserInterface object representing the game's UI state.
 * @return int The calculated limit for the number of dice.
 */
int calcDiceLimit(UserInterface &ui);

/**
 * @brief Redefines the game menu based on the current menu mode.
 *
 * Sets the initial selected menu item, determines the number of menu options based on the
 * current menu mode, and populates the options and keys for the menu. It also initializes
 * menu elements with these options and keys.
 *
 * @param ui Reference to the UserInterface containing the menu to be redefined.
 */
void redefineMenu(UserInterface &ui);

/**
 * @brief Resets a message string to blank spaces.
 *
 * Fills the entire message array with blank spaces. This function is used to clear
 * any previous message content, effectively resetting it.
 *
 * @param message Array of characters representing the message to be reset.
 */
void resetMessage(char message[MAX_MESSAGE_LEN]);

/**
 * @brief Resets both information and error messages in the user interface.
 *
 * Clears the content of both information and error message strings in the UserInterface
 * structure by resetting them to blank spaces. This function is typically called when
 * messages need to be cleared from the UI.
 *
 * @param ui Reference to the UserInterface containing the messages to be reset.
 */
void resetMessages(UserInterface &ui);

/**
 * @brief Sets a new message in a fixed-length message array.
 *
 * Copies a new message into a fixed-length character array, ensuring that the length
 * does not exceed the maximum allowed. It truncates the new message if it's longer
 * than the maximum length.
 *
 * @param message Array of characters where the new message will be set.
 * @param newMessage Pointer to the string representing the new message to be copied.
 */
void messageSet(char message[MAX_MESSAGE_LEN], const char *newMessage);

#endif //BACKGAMMONGAME_USERINTERFACE_H
