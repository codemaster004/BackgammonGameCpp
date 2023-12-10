//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_GAMECONFIGS_H
#define BACKGAMMONGAME_GAMECONFIGS_H

#include "../Utility.h"
#include "../viewModel/Space.h"

#define DEBUG_MODE false

#define N_BOARDS 2
#define POINTS_PER_BOARD 6
#define PAWNS_PER_POINT 15
#define PAWNS_PER_PLAYER 15
#define N_DICES 2

#define CAPTURE_THRESHOLD 1
#define ESCAPE_THRESHOLD 15

/// Magical Line of PAIN & SUFFERING
/// Do NOT change any settings under this line
/// If you do the world as we know it will cease to exist
/////////////////////////////////////////////////////////

#define N_PLAYERS 2
#define N_ALL_PLAYERS 3

#define MAX_NAME_LENGTH 16

const int nPoints = N_BOARDS * POINTS_PER_BOARD * 2;
const int totalPawns = N_PLAYERS * PAWNS_PER_PLAYER;

#endif //BACKGAMMONGAME_GAMECONFIGS_H
