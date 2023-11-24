//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_GAMECONFIGS_H
#define BACKGAMMONGAME_GAMECONFIGS_H

#include "../Utility.h"
#include "../viewModel/Space.h"

#define N_BOARDS 2
#define POINTS_PER_BOARD 6
#define PAWNS_PER_POINT 6
#define PAWNS_PER_PLAYER 15
#define N_PLAYERS 2
#define PLAYERS_PER_GAME 2
#define N_DICES 2
#define N_STARTING_POS 4

#define N_MENU_OPTIONS 4
#define MENU_ELEMENT_LEN 7

#define CAPTURE_THRESHOLD 1

const uint nPoints = N_BOARDS * POINTS_PER_BOARD * 2;
const uint totalPawns = N_PLAYERS * PAWNS_PER_PLAYER;

const char menuOptions[N_MENU_OPTIONS][MENU_ELEMENT_LEN] = {{"(M)ove"}, {"(R)oll"}, {"(U)ndo"}, {"(Q)uit"}};
const char menuKeys[N_MENU_OPTIONS] = {'m', 'r', 'u', 'q'};

const Pos startingPos[N_STARTING_POS][2] = {{0, 2}, {11, 5}, {16, 3}, {18, 5}};

#endif //BACKGAMMONGAME_GAMECONFIGS_H
