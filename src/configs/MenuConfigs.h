//
// Created by Filip Dabkowski on 28/11/2023.
//

#ifndef BACKGAMMONGAME_MENUCONFIGS_H
#define BACKGAMMONGAME_MENUCONFIGS_H

#define N_GAME_MENU_OPTIONS 5
const char *menuGameOptions[N_GAME_MENU_OPTIONS] = {"(M)ove", "(R)oll", "(U)ndo", "(S)ave", "(Q)uit"};
const char menuGameKeys[N_GAME_MENU_OPTIONS] = {'m', 'r', 'u', 's', 'q'};

#define N_STARTER_MENU_OPTIONS 3
const char *menuStarterOptions[N_STARTER_MENU_OPTIONS] = {"(N)ew Game", "(L)oad Game", "(Q)uit"};
const char menuStarterKeys[N_STARTER_MENU_OPTIONS] = {'n', 'l', 'q'};

#define N_DICE_MENU_OPTIONS 3
const char *menuDiceOptions[N_DICE_MENU_OPTIONS] = {"Use Dice (1)", "Use Dice (2)", "Use Both Dices (3)"};
const char menuDiceKeys[N_DICE_MENU_OPTIONS] = {'1', '2', '3'};

#endif //BACKGAMMONGAME_MENUCONFIGS_H
