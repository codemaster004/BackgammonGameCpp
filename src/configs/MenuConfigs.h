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

#define N_DICE_MENU_OPTIONS 4
const char *menuDiceOptions[N_DICE_MENU_OPTIONS] = {"Use Dice (1)", "Use Dice (2)", "Use Both Dices (0)", "End Turn (-)"};
const char menuDiceKeys[N_DICE_MENU_OPTIONS] = {'1', '2', '0', '-'};

#define N_PIECE_MENU_OPTIONS 2
const char *menuPieceOptions[N_PIECE_MENU_OPTIONS] = {"Pick Piece (Space)", "End Turn (-)"};
const char menuPieceKeys[N_PIECE_MENU_OPTIONS] = {' ', '-'};

#endif //BACKGAMMONGAME_MENUCONFIGS_H
