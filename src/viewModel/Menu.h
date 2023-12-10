//
// Created by Filip Dabkowski on 10/12/2023.
//

#ifndef BACKGAMMONGAME_MENU_H
#define BACKGAMMONGAME_MENU_H

#include "UserInterface.h"

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

#endif //BACKGAMMONGAME_MENU_H
