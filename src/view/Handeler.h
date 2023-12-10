//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_HANDELER_H
#define BACKGAMMONGAME_HANDELER_H

#include "../viewModel/UserInterface.h"
#include "../Utility.h"

/**
 * @brief Draws the outline of each board section in the game.
 *
 * @param space The initial placement area for the first board's outline.
 */
void handleBoardOutline(Placement space);

/**
 * @brief Draws the pieces on each board section in the game.
 *
 * @param space The initial placement area for the pieces, before adjusting for the border width.
 */
void handlePieces(Placement space);

/**
 * @brief Draws the bar area on the board, showing the number of pawns each player has on the bar.
 *
 * @param bar The Bar structure containing the pawns currently on the bar.
 * @param selected The index of the selected area on the board (used to highlight the bar if selected).
 */
void handleBar(Bar bar, int selected);

/**
 * @brief Draws the dices on the board, showing the current roll.
 *
 * @param space The initial placement area for the dice.
 * @param center The center position around which the dice will be aligned.
 * @param dices Array containing the values of the dice.
 */
void handleDices(Placement space, Pos center, int *dices);

/**
 * @brief Displays the game menu with selectable options.
 *
 * @param menu The Menu structure containing the menu elements and selection information.
 * @param center The center position around which the menu will be aligned.
 */
void handleMenu(Menu menu, Pos center);

/**
 * @brief Handles the display of board indexes at the top and bottom of each board section.
 *
 * @param indexes Array of index strings representing the board positions.
 * @param pickedIndex The index of the picked or selected point on the board.
 * @param digits The number of digits in the largest index.
 * @param posTop The placement area for the top row of indexes.
 * @param posBot The placement area for the bottom row of indexes.
 */
void handleIndexes(char **indexes, int pickedIndex, int digits, Placement posTop, Placement posBot);

/**
 * @brief Displays informational and error messages on the user interface.
 *
 * @param ui Reference to the UserInterface structure containing the messages to be displayed.
 */
void handleMessages(UserInterface &ui);

/**
 * @brief Generates the basic layout of the backgammon board on the user interface.
 *
 * @param ui Reference to the UserInterface structure containing the game state and layout information.
 */
void generateBasicBoard(UserInterface &ui);

/**
 * @brief Generates and displays the interactive components of the user interface for the game.
 *
 * @param ui Reference to the UserInterface structure containing the game state and layout information.
 */
void generateInteractiveUI(UserInterface &ui);

/**
 * @brief Arranges and displays the pawns on the board based on their current positions.
 *
 * @param game Reference to the Board structure containing the game state with pawn positions.
 * @param space The initial placement area for the pawns, before adjusting for borders and spacing.
 */
void handlePawnPlacement(Board &game, Placement space);

/**
 * @brief Handles the user interface updates when a game is won.
 *
 * @param ui Reference to the UserInterface structure containing the game state and menu information.
 */
void handleGameWon(UserInterface &ui);

/**
 * @brief Creates a string representing a player's ranking and score for the Hall of Fame.
 *
 * @param ui Reference to the UserInterface structure containing the Hall of Fame data.
 * @param text Reference to an array of strings where the constructed text will be stored.
 * @param index The index of the player in the Hall of Fame whose text is being constructed.
 */
void handlePlayerHallText(UserInterface ui, char **text, int index);

#endif //BACKGAMMONGAME_HANDELER_H
