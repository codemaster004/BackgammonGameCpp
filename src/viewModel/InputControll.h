//
// Created by Filip Dabkowski on 18/11/2023.
//

#ifndef BACKGAMMONGAME_INPUTCONTROLL_H
#define BACKGAMMONGAME_INPUTCONTROLL_H

#include "../model/Board.h"
#include "UserInterface.h"


/**
 * Sets up the game board and initializes game elements.
 *
 * @param game Reference to the Board object representing the game state.
 */
void gameSetUp(Board &game);

/**
 * Resets the menu to a specified mode and refreshes the user interface.
 *
 * @param ui Reference to the UserInterface object to be updated.
 * @param mode The new MenuMode to set.
 */
void resetMenuTo(UserInterface &ui, MenuMode mode);

/**
 * Controls the creation of a new game or loading an existing game based on user input.
 *
 * @param input The character input representing the user's choice.
 * @param ui Reference to the UserInterface object to be updated.
 */
void newGameController(int input, UserInterface &ui);

/**
 * Controls the game play actions based on user input during a game session.
 *
 * @param input The character input representing the user's choice.
 * @param ui Reference to the UserInterface object representing the game state.
 */
void gamePlayController(int input, UserInterface &ui);

/**
 * Handles the dice selection process based on user input.
 *
 * @param input The character input representing the user's dice choice.
 * @param ui Reference to the UserInterface object representing the game state.
 */
void pickDiceController(int input, UserInterface &ui);

/**
 * @brief Manages point selection and pawn movement based on user input.
 *
 * @param input The character input representing the user's choice.
 * @param ui Reference to the UserInterface object representing the game state.
 */
void pickPointController(int input, UserInterface &ui);

/**
 * @brief Manages overall input control for the game's user interface.
 *
 * @param input The character input representing the user's choice.
 * @param ui Reference to the UserInterface object representing the game state.
 */
void inputController(int input, UserInterface &ui);

#endif //BACKGAMMONGAME_INPUTCONTROLL_H
